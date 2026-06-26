#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>
#include <string>
#include <vector>
// quick submit

class FileWriter {
private:
    std::ofstream word_;
    std::string tmp_;

public:
    explicit FileWriter(const std::string& path) : tmp_(path) {
        word_.open(path);
    }

    ~FileWriter() {
        if (word_.is_open())
            word_.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& other) noexcept
        : word_(std::move(other.word_)), tmp_(std::move(other.tmp_)) {}

    FileWriter& operator=(FileWriter&& other) noexcept {
        if (&other != this) {
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
    std::vector<std::pair<std::string, int>> arr;
    std::string buffer;
    while (std::getline(std::cin, buffer)) {
        auto comma = buffer.find(',');
        arr.push_back({buffer.substr(0, comma), std::stoi(buffer.substr(comma + 1))});
    }
    emitReport(FileWriter("report.txt"), arr);
    std::cout << "Report written to report.txt\n";
    return 0;
}
