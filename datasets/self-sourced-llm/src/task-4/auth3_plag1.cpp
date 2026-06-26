#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
// cleaned up version

class FileWriter {
    std::ofstream file_;
    std::string fname_;

    void closeIfOpen() {
        if (file_.is_open()) file_.close();
    }

public:
    explicit FileWriter(const std::string& fname) : fname_(fname) {
        file_.open(fname_);
    }

    ~FileWriter() { closeIfOpen(); }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& mv) noexcept
        : file_(std::move(mv.file_)), fname_(std::move(mv.fname_)) {}

    FileWriter& operator=(FileWriter&& mv) noexcept {
        if (this != &mv) {
            closeIfOpen();
            file_ = std::move(mv.file_);
            fname_ = std::move(mv.fname_);
        }
        return *this;
    }

    void write_line(const std::string& text) {
        file_ << text << std::endl;
    }
};

static std::string gradeLabel(int pts) {
    if (pts >= 90) return "A";
    if (pts >= 80) return "B";
    if (pts >= 70) return "C";
    if (pts >= 60) return "D";
    return "F";
}

int main() {
    std::vector<std::pair<std::string, int>> lines;
    for (std::string buf; std::getline(std::cin, buf);) {
        std::size_t sep = buf.find(',');
        lines.emplace_back(buf.substr(0, sep), std::stoi(buf.substr(sep + 1)));
    }

    FileWriter report("report.txt");
    report.write_line("Student Report");
    report.write_line("--------------");

    double acc = 0.0;
    for (const auto& entry : lines) {
        acc += entry.second;
        std::ostringstream row;
        row << entry.first << ": " << entry.second << " [" << gradeLabel(entry.second) << "]";
        report.write_line(row.str());
    }

    std::ostringstream footer;
    footer << std::fixed << std::setprecision(1)
           << "Average: " << (acc / static_cast<double>(lines.size()));
    report.write_line(footer.str());

    std::cout << "Report written to report.txt" << std::endl;
    return 0;
}
