#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
// cleaned up version

class FileWriter {
private:
    std::ofstream os_;
    std::string path_;

public:
    explicit FileWriter(const std::string& path) : path_(path) {
        os_.open(path);
    }

    ~FileWriter() {
        if (os_.is_open())
            os_.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& other) noexcept
        : os_(std::move(other.os_)), path_(std::move(other.path_)) {}

    FileWriter& operator=(FileWriter&& other) noexcept {
        if (&other != this) {
            if (os_.is_open()) os_.close();
            os_ = std::move(other.os_);
            path_ = std::move(other.path_);
        }
        return *this;
    }

    void write_line(const std::string& line) {
        os_ << line << '\n';
    }
};

char mapGrade(int score) {
    if (score >= 90) return 'A';
    if (score >= 80) return 'B';
    if (score >= 70) return 'C';
    if (score >= 60) return 'D';
    return 'F';
}

void emitReport(FileWriter writer, const std::vector<std::pair<std::string, int>>& data) {
    writer.write_line("Student Report");
    writer.write_line("--------------");
    double total = 0.0;
    for (const auto& item : data) {
        total += item.second;
        std::string row = item.first + ": " + std::to_string(item.second)
                        + " [" + mapGrade(item.second) + "]";
        writer.write_line(row);
    }
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << "Average: " << total / data.size();
    writer.write_line(oss.str());
}

int main() {
    std::vector<std::pair<std::string, int>> lines;
    std::string buf;
    while (std::getline(std::cin, buf)) {
        auto comma = buf.find(',');
        lines.push_back({buf.substr(0, comma), std::stoi(buf.substr(comma + 1))});
    }
    emitReport(FileWriter("report.txt"), lines);
    std::cout << "Report written to report.txt\n";
    return 0;
}
