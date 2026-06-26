#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
// adapted from classmate submission

class FileWriter {
    std::ofstream stream_;
    std::string fname_;

public:
    explicit FileWriter(const std::string& path)
        : fname_(path), stream_(path) {}

    ~FileWriter() {
        if (stream_.is_open())
            stream_.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& moved) noexcept
        : stream_(std::move(moved.stream_)),
          fname_(std::move(moved.fname_)) {}

    FileWriter& operator=(FileWriter&& moved) noexcept {
        if (this != &moved)
{
            if (stream_.is_open()) stream_.close();
            stream_ = std::move(moved.stream_);
            fname_ = std::move(moved.fname_);
        }
        return *this;
    }

    void write_line(const std::string& content) {
        stream_ << content << std::endl;
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

int main()
{
    struct Student { std::string name; int score; };
    std::vector<Student> roster;
    std::string inputLine;
    while (std::getline(std::cin, inputLine)) {
        std::size_t delim = inputLine.find(',');
        roster.push_back({inputLine.substr(0, delim),
                          std::stoi(inputLine.substr(delim + 1))});
    }

    FileWriter fw("report.txt");
    fw.write_line("Student Report");
    fw.write_line("--------------");

    double runningTotal = 0.0;
    for (const Student& s : roster)
{
        runningTotal += s.score;
        std::ostringstream lineBuilder;
        lineBuilder << s.name << ": " << s.score
                    << " [" << grading::forScore(s.score) << "]";
        fw.write_line(lineBuilder.str());
    }

    std::ostringstream averageLine;
    averageLine << std::fixed << std::setprecision(1)
                << "Average: " << runningTotal / roster.size();
    fw.write_line(averageLine.str());

    std::cout << "Report written to report.txt" << std::endl;
    return 0;
}
