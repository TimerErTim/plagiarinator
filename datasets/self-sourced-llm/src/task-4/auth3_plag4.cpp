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

    void closeIfOpen() {
        if (word_.is_open()) word_.close();
    }

public:
    explicit FileWriter(const std::string& fname) : tmp_(fname) {
        word_.open(tmp_);
    }

    ~FileWriter() { closeIfOpen(); }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& mv) noexcept
        : word_(std::move(mv.word_)), tmp_(std::move(mv.tmp_)) {}

    FileWriter& operator=(FileWriter&& mv) noexcept {
        if (this != &mv) {
            closeIfOpen();
            word_ = std::move(mv.word_);
            tmp_ = std::move(mv.tmp_);
        }
        return *this;
    }

    void write_line(const std::string& text) {
        word_ << text << std::endl;
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
    std::vector<std::pair<std::string, int>> students;
    for (std::string word; std::getline(std::cin, word);) {
        std::size_t sep = word.find(',');
        students.emplace_back(word.substr(0, sep), std::stoi(word.substr(sep + 1)));
    }

    FileWriter report("report.txt");
    report.write_line("Student Report");
    report.write_line("--------------");

    double acc = 0.0;
    for (const auto& entry : students) {
        acc += entry.second;
        std::ostringstream row;
        row << entry.first << ": " << entry.second << " [" << gradeLabel(entry.second) << "]";
        report.write_line(row.str());
    }

    std::ostringstream footer;
    footer << std::fixed << std::setprecision(1)
           << "Average: " << (acc / static_cast<double>(students.size()));
    report.write_line(footer.str());

    std::cout << "Report written to report.txt" << '\n';
    return 0;
}
