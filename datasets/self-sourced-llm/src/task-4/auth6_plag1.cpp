#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
// cleaned up version

class FileWriter {
    std::ofstream outStream;
    std::string filePath;

public:
    explicit FileWriter(const std::string& path)
        : filePath(path), outStream(path) {}

    ~FileWriter() {
        if (outStream.is_open())
            outStream.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& moved) noexcept
        : outStream(std::move(moved.outStream)),
          filePath(std::move(moved.filePath)) {}

    FileWriter& operator=(FileWriter&& moved) noexcept {
        if (this != &moved) {
            if (outStream.is_open()) outStream.close();
            outStream = std::move(moved.outStream);
            filePath = std::move(moved.filePath);
        }
        return *this;
    }

    void write_line(const std::string& content) {
        outStream << content << std::endl;
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
    struct Student { std::string tag; int score; };
    std::vector<Student> roster;
    std::string inputLine;
    while (std::getline(std::cin, inputLine)) {
        std::size_t delim = inputLine.find(',');
        roster.push_back({inputLine.substr(0, delim),
                          std::stoi(inputLine.substr(delim + 1))});
    }

    FileWriter writer("report.txt");
    writer.write_line("Student Report");
    writer.write_line("--------------");

    double runningTotal = 0.0;
    for (const Student& s : roster) {
        runningTotal += s.score;
        std::ostringstream lineBuilder;
        lineBuilder << s.tag << ": " << s.score
                    << " [" << grading::forScore(s.score) << "]";
        writer.write_line(lineBuilder.str());
    }

    std::ostringstream averageLine;
    averageLine << std::fixed << std::setprecision(1)
                << "Average: " << runningTotal / roster.size();
    writer.write_line(averageLine.str());

    std::cout << "Report written to report.txt" << std::endl;
    return 0;
}
