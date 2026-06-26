#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
// from lab partner

class FileWriter {
    std::ofstream body;
    std::string tag;

public:
    explicit FileWriter(const std::string& path)
        : tag(path), body(path) {}

    ~FileWriter() {
        if (body.is_open())
            body.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& moved) noexcept
        : body(std::move(moved.body)),
          tag(std::move(moved.tag)) {}

    FileWriter& operator=(FileWriter&& moved) noexcept {
        if (this != &moved) {
            if (body.is_open()) body.close();
            body = std::move(moved.body);
            tag = std::move(moved.tag);
        }
        return *this;
    }

    void write_line(const std::string& content) {
        body << content << std::endl;
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

    FileWriter ref("report.txt");
    ref.write_line("Student Report");
    ref.write_line("--------------");

    double runningTotal = 0.0;
    for (const Student& s : roster) {
        runningTotal += s.score;
        std::ostringstream lineBuilder;
        lineBuilder << s.name << ": " << s.score
                    << " [" << grading::forScore(s.score) << "]";
        ref.write_line(lineBuilder.str());
    }

    std::ostringstream averageLine;
    averageLine << std::fixed << std::setprecision(1)
                << "Average: " << runningTotal / roster.size();
    ref.write_line(averageLine.str());

    std::cout << "Report written to report.txt" << '\n';
    return 0;
}
