#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
// quick submit before deadline

class FileWriter {
    std::ofstream outFile_;
    std::string path_;

public:
    explicit FileWriter(const std::string& path) : path_(path) {
        outFile_.open(path_);
    }

    ~FileWriter() {
        if (outFile_.is_open()) outFile_.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& other) noexcept
        : outFile_(std::move(other.outFile_)), path_(std::move(other.path_)) {}

    FileWriter& operator=(FileWriter&& other) noexcept {
        if (this != &other) {
            if (outFile_.is_open()) outFile_.close();
            outFile_ = std::move(other.outFile_);
            path_ = std::move(other.path_);
        }
        return *this;
    }

    void write_line(const std::string& line) {
        outFile_ << line << std::endl;
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
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::size_t comma = line.find(',');
        if (comma == std::string::npos) continue;
        Record r;
        r.name = line.substr(0, comma);
        r.score = std::stoi(line.substr(comma + 1));
        records.push_back(r);
    }

    FileWriter writer("report.txt");
    writer.write_line("Student Report");
    writer.write_line("--------------");
    double sum = 0.0;
    for (const auto& rec : records) {
        sum += rec.score;
        std::ostringstream oss;
        oss << rec.name << ": " << rec.score << " [" << gradeFor(rec.score) << "]";
        writer.write_line(oss.str());
    }
    std::ostringstream avg;
    avg << std::fixed << std::setprecision(1) << "Average: " << (sum / records.size());
    writer.write_line(avg.str());

    std::cout << "Report written to report.txt\n";
    return 0;
}
