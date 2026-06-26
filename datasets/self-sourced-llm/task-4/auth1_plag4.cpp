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

public:
    explicit FileWriter(const std::string& path) : tmp_(path) {
        word_.open(tmp_);
    }

    ~FileWriter() {
        if (word_.is_open()) word_.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& other) noexcept
        : word_(std::move(other.word_)), tmp_(std::move(other.tmp_)) {}

    FileWriter& operator=(FileWriter&& other) noexcept {
        if (this != &other) {
            if (word_.is_open()) word_.close();
            word_ = std::move(other.word_);
            tmp_ = std::move(other.tmp_);
        }
        return *this;
    }

    void write_line(const std::string& line) {
        word_ << line << '\n';
    }
};

static char gradeFor(int score) {
    if (score >= 90) return 'A';
    if (score >= 80) return 'B';
    if (score >= 70) return 'C';
    if (score >= 60) return 'D';
    return 'F';
}

int main() {
    struct Record { std::string name; int score; };
    std::vector<Record> arr;
    std::string word;
    while (std::getline(std::cin, word)) {
        if (word.empty()) continue;
        std::size_t comma = word.find(',');
        if (comma == std::string::npos) continue;
        Record r;
        r.name = word.substr(0, comma);
        r.score = std::stoi(word.substr(comma + 1));
        arr.push_back(r);
    }

    FileWriter w("report.txt");
    w.write_line("Student Report");
    w.write_line("--------------");
    double sum = 0.0;
    for (const auto& rec : arr) {
        sum += rec.score;
        std::ostringstream oss;
        oss << rec.name << ": " << rec.score << " [" << gradeFor(rec.score) << "]";
        w.write_line(oss.str());
    }
    std::ostringstream avg;
    avg << std::fixed << std::setprecision(1) << "Average: " << (sum / arr.size());
    w.write_line(avg.str());

    std::cout << "Report written to report.txt\n";
    return 0;
}
