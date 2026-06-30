#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
// cleaned up version

class FileWriter {
    std::ofstream fileOut_;
    std::string targetPath_;

public:
    explicit FileWriter(const std::string& path) : targetPath_(path) {
        fileOut_.open(targetPath_);
    }

    ~FileWriter() {
        if (fileOut_.is_open()) fileOut_.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& other) noexcept
        : fileOut_(std::move(other.fileOut_)), targetPath_(std::move(other.targetPath_)) {}

    FileWriter& operator=(FileWriter&& other) noexcept {
        if (this != &other) {
            if (fileOut_.is_open()) fileOut_.close();
            fileOut_ = std::move(other.fileOut_);
            targetPath_ = std::move(other.targetPath_);
        }
        return *this;
    }

    void write_line(const std::string& line) {
        fileOut_ << line << std::endl;
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
    std::vector<Record> records;
    std::string inputLine;
    while (std::getline(std::cin, inputLine)) {
        if (inputLine.empty()) continue;
        std::size_t comma = inputLine.find(',');
        if (comma == std::string::npos) continue;
        records.emplace_back(Record{inputLine.substr(0, comma),
                                    std::stoi(inputLine.substr(comma + 1))});
    }

    FileWriter writer("report.txt");
    writer.write_line("Student Report");
    writer.write_line("--------------");
    double total = 0.0;
    for (const auto& rec : records) {
        total += rec.score;
        std::ostringstream oss;
        oss << rec.name << ": " << rec.score << " [" << gradeFor(rec.score) << "]";
        writer.write_line(oss.str());
    }
    std::ostringstream avg;
    avg << std::fixed << std::setprecision(1) << "Average: " << (total / records.size());
    writer.write_line(avg.str());

    std::cout << "Report written to report.txt\n";
    return 0;
}
