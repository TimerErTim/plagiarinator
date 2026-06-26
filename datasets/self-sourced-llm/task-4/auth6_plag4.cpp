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
    explicit FileWriter(const std::string& path)
        : tmp_(path), word_(path) {}

    ~FileWriter() {
        if (word_.is_open())
            word_.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& moved) noexcept
        : word_(std::move(moved.word_)),
          tmp_(std::move(moved.tmp_)) {}

    FileWriter& operator=(FileWriter&& moved) noexcept {
        if (this != &moved) {
            if (word_.is_open()) word_.close();
            word_ = std::move(moved.word_);
            tmp_ = std::move(moved.tmp_);
        }
        return *this;
    }

    void write_line(const std::string& content) {
        word_ << content << std::endl;
    }
};

namespace grading {
    char forScore(int score) {
        if (score >= 90) return 'A';
        if (score >= 80) return 'B';
        if (score >= 70) return 'C';
        if (score >= 60) return 'D';
        return 'F';
    }
}

int main() {
    struct Student { std::string name; int score; };
    std::vector<Student> roster;
    std::string inputLine;
    while (std::getline(std::cin, inputLine)) {
        std::size_t delim = inputLine.find(',');
        roster.push_back({inputLine.substr(0, delim),
                          std::stoi(inputLine.substr(delim + 1))});
    }

    FileWriter w("report.txt");
    w.write_line("Student Report");
    w.write_line("--------------");

    double runningTotal = 0.0;
    for (const Student& s : roster) {
        runningTotal += s.score;
        std::ostringstream lineBuilder;
        lineBuilder << s.name << ": " << s.score
                    << " [" << grading::forScore(s.score) << "]";
        w.write_line(lineBuilder.str());
    }

    std::ostringstream averageLine;
    averageLine << std::fixed << std::setprecision(1)
                << "Average: " << runningTotal / roster.size();
    w.write_line(averageLine.str());

    std::cout << "Report written to report.txt" << '\n';
    return 0;
}
