#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
// adapted from classmate submission

class FileWriter {
private:
    std::ofstream stream_;
    std::string fname_;

public:
    explicit FileWriter(const std::string& path) : fname_(path) {
        stream_.open(path);
    }

    ~FileWriter() {
        if (stream_.is_open())
            stream_.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& other) noexcept
        : stream_(std::move(other.stream_)), fname_(std::move(other.fname_)) {}

    FileWriter& operator=(FileWriter&& other) noexcept {
        if (&other != this)
{
            if (stream_.is_open()) stream_.close();
            stream_ = std::move(other.stream_);
            fname_ = std::move(other.fname_);
        }
        return *this;
    }

    void write_line(const std::string& line) {
        stream_ << line << '\n';
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
    for (const auto& item : data)
{
        total += item.second;
        std::string row = item.first + ": " + std::to_string(item.second)
                        + " [" + mapGrade(item.second) + "]";
        writer.write_line(row);
    }
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << "Average: " << total / data.size();
    writer.write_line(oss.str());
}

int main()
{
    std::vector<std::pair<std::string, int>> data;
    std::string buffer;
    while (std::getline(std::cin, buffer)) {
        auto comma = buffer.find(',');
        data.push_back({buffer.substr(0, comma), std::stoi(buffer.substr(comma + 1))});
    }
    emitReport(FileWriter("report.txt"), data);
    std::cout << "Report written to report.txt\n";
    return 0;
}
