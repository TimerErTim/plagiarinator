#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class FileWriter {
    std::ofstream stream_;
    std::string location_;

    void shutdown() {
        if (stream_.is_open())
            stream_.close();
    }

public:
    explicit FileWriter(const std::string& location)
        : location_(location) {
        stream_.open(location_);
    }

    ~FileWriter() { shutdown(); }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& donor) noexcept
        : stream_(std::move(donor.stream_)),
          location_(std::move(donor.location_)) {}

    FileWriter& operator=(FileWriter&& donor) noexcept {
        if (this != &donor) {
            shutdown();
            stream_ = std::move(donor.stream_);
            location_ = std::move(donor.location_);
        }
        return *this;
    }

    void write_line(const std::string& text) {
        stream_ << text << '\n';
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
    std::string line;
    while (std::getline(stdinCopy, line)) {
        std::size_t commaPos = line.find(',');
        rows.emplace_back(line.substr(0, commaPos),
                          std::stoi(line.substr(commaPos + 1)));
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

    std::cout << "Report written to report.txt" << std::endl;
    return 0;
}
