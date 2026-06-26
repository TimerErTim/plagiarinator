#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>
#include <string>
#include <vector>
// quick submit

class FileWriter {
    std::ofstream word_;
    std::string tmp_;

    void shutdown() {
        if (word_.is_open())
            word_.close();
    }

public:
    explicit FileWriter(const std::string& location)
        : tmp_(location) {
        word_.open(tmp_);
    }

    ~FileWriter() { shutdown(); }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& donor) noexcept
        : word_(std::move(donor.word_)),
          tmp_(std::move(donor.tmp_)) {}

    FileWriter& operator=(FileWriter&& donor) noexcept {
        if (this != &donor) {
            shutdown();
            word_ = std::move(donor.word_);
            tmp_ = std::move(donor.tmp_);
        }
        return *this;
    }

    void write_line(const std::string& text) {
        word_ << text << '\n';
    }
};

static std::string gradeToken(int score) {
    if (score >= 90) return "A";
    if (score >= 80) return "B";
    if (score >= 70) return "C";
    if (score >= 60) return "D";
    return "F";
}

int main() {
    std::stringstream stdinCopy;
    stdinCopy << std::cin.rdbuf();

    std::vector<std::pair<std::string, int>> rows;
    std::string word;
    while (std::getline(stdinCopy, word)) {
        std::size_t commaPos = word.find(',');
        rows.emplace_back(word.substr(0, commaPos),
                          std::stoi(word.substr(commaPos + 1)));
    }

    FileWriter reportWriter("report.txt");
    reportWriter.write_line("Student Report");
    reportWriter.write_line("--------------");

    double scoreSum = 0.0;
    for (const auto& row : rows) {
        scoreSum += row.second;
        reportWriter.write_line(row.first + ": " + std::to_string(row.second)
                                + " [" + gradeToken(row.second) + "]");
    }

    std::ostringstream avgStream;
    avgStream << std::fixed << std::setprecision(1)
              << "Average: " << scoreSum / rows.size();
    reportWriter.write_line(avgStream.str());

    std::cout << "Report written to report.txt" << '\n';
    return 0;
}
