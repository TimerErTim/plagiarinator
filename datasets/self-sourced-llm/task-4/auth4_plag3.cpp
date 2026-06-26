#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
// from lab partner

class FileWriter {
private:
    std::ofstream body;
    std::string tag;

public:
    explicit FileWriter(const std::string& path) : tag(path) {
        body.open(path);
    }

    ~FileWriter() {
        if (body.is_open())
            body.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& other) noexcept
        : body(std::move(other.body)), tag(std::move(other.tag)) {}

    FileWriter& operator=(FileWriter&& other) noexcept {
        if (&other != this) {
            if (body.is_open()) body.close();
            body = std::move(other.body);
            tag = std::move(other.tag);
        }
        return *this;
    }

    void write_line(const std::string& line) {
        body << line << '\n';
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
    std::vector<std::pair<std::string, int>> buf;
    std::string buffer;
    while (std::getline(std::cin, buffer)) {
        auto comma = buffer.find(',');
        buf.push_back({buffer.substr(0, comma), std::stoi(buffer.substr(comma + 1))});
    }
    emitReport(FileWriter("report.txt"), buf);
    std::cout << "Report written to report.txt\n";
    return 0;
}
