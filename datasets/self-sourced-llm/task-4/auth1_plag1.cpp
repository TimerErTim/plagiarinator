#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
// cleaned up version

class FileWriter {
    std::ofstream out_;
    std::string path_;

public:
    explicit FileWriter(const std::string& path) : path_(path) {
        out_.open(path_);
    }

    ~FileWriter() {
        if (out_.is_open()) out_.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& other) noexcept
        : out_(std::move(other.out_)), path_(std::move(other.path_)) {}

    FileWriter& operator=(FileWriter&& other) noexcept {
        if (this != &other) {
            if (out_.is_open()) out_.close();
            out_ = std::move(other.out_);
            path_ = std::move(other.path_);
        }
        return *this;
    }

    void write_line(const std::string& line) {
        out_ << line << '\n';
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
    struct Record { std::string tag; int score; };
    std::vector<Record> records;
    std::string buf;
    while (std::getline(std::cin, buf)) {
        if (buf.empty()) continue;
        std::size_t comma = buf.find(',');
        if (comma == std::string::npos) continue;
        Record r;
        r.tag = buf.substr(0, comma);
        r.score = std::stoi(buf.substr(comma + 1));
        records.emplace_back(r);
    }

    FileWriter writer("report.txt");
    writer.write_line("Student Report");
    writer.write_line("--------------");
    double sum = 0.0;
    for (const auto& rec : records) {
        sum += rec.score;
        std::ostringstream oss;
        oss << rec.tag << ": " << rec.score << " [" << gradeFor(rec.score) << "]";
        writer.write_line(oss.str());
    }
    std::ostringstream avg;
    avg << std::fixed << std::setprecision(1) << "Average: " << (sum / records.size());
    writer.write_line(avg.str());

    std::cout << "Report written to report.txt\n";
    return 0;
}
