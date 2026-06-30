#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
// adapted from classmate submission

class FileWriter
{
    std::ofstream outStream;
    std::string filePath;

public:
    explicit FileWriter(const std::string& path)
        : filePath(path), outStream(path) {}

    ~FileWriter()
    {
        if (outStream.is_open())
            outStream.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& moved) noexcept
        : outStream(std::move(moved.outStream)),
          filePath(std::move(moved.filePath)) {}

    FileWriter& operator=(FileWriter&& moved) noexcept
    {
        if (this != &moved)
        {
            if (outStream.is_open()) outStream.close();
            outStream = std::move(moved.outStream);
            filePath = std::move(moved.filePath);
        }
        return *this;
    }

    void write_line(const std::string& content)
    {
        outStream << content << std::endl;
    }
};

namespace grading
{
    char forScore(int score)
    {
        if (score >= 90) return 'A';
        if (score >= 80) return 'B';
        if (score >= 70) return 'C';
        if (score >= 60) return 'D';
        return 'F';
    }
}

static void writeReportBody(FileWriter& w, const std::vector<std::pair<std::string,int>>& roster)
{
    double runningTotal = 0.0;
    for (std::size_t i = 0; i < roster.size(); ++i)
    {
        runningTotal += roster[i].second;
        std::ostringstream lineBuilder;
        lineBuilder << roster[i].first << ": " << roster[i].second
                    << " [" << grading::forScore(roster[i].second) << "]";
        w.write_line(lineBuilder.str());
    }
    std::ostringstream averageLine;
    averageLine << std::fixed << std::setprecision(1)
                << "Average: " << runningTotal / roster.size();
    w.write_line(averageLine.str());
}

int main()
{
    std::vector<std::pair<std::string,int>> roster;
    std::string inputLine;
    while (std::getline(std::cin, inputLine))
    {
        std::size_t delim = inputLine.find(',');
        roster.push_back({inputLine.substr(0, delim),
                          std::stoi(inputLine.substr(delim + 1))});
    }

    FileWriter writer("report.txt");
    writer.write_line("Student Report");
    writer.write_line("--------------");
    writeReportBody(writer, roster);

    std::cout << "Report written to report.txt" << std::endl;
    return 0;
}
