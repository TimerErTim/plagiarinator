#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <iomanip>
// borrowed from roommate's homework

class FileWriter
{
    std::ofstream streamOut_;
    std::string pathVal_;

public:
    explicit FileWriter(const std::string& path)
        : pathVal_(path), streamOut_(path) {}

    ~FileWriter()
    {
        if (streamOut_.is_open())
            streamOut_.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& donor) noexcept
        : streamOut_(std::move(donor.streamOut_)),
          pathVal_(std::move(donor.pathVal_)) {}

    FileWriter& operator=(FileWriter&& donor) noexcept
    {
        if (this != &donor)
        {
            if (streamOut_.is_open()) streamOut_.close();
            streamOut_ = std::move(donor.streamOut_);
            pathVal_ = std::move(donor.pathVal_);
        }
        return *this;
    }

    void write_line(const std::string& txt)
    {
        streamOut_ << txt << std::endl;
    }
};

char scoreToLetter(int pts)
{
    if (pts >= 90) return 'A';
    if (pts >= 80) return 'B';
    if (pts >= 70) return 'C';
    if (pts >= 60) return 'D';
    return 'F';
}

int main()
{
    struct Student { std::string name; int score; };
    std::vector<Student> roster;
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

    double runningTotal = 0.0;
    for (const Student& s : roster)
    {
        runningTotal += s.score;
        std::ostringstream lineBuilder;
        lineBuilder << s.name << ": " << s.score
                    << " [" << scoreToLetter(s.score) << "]";
        writer.write_line(lineBuilder.str());
    }

    std::ostringstream averageLine;
    averageLine << std::fixed << std::setprecision(1)
                << "Average: " << runningTotal / roster.size();
    writer.write_line(averageLine.str());

    std::cout << "Report written to report.txt" << std::endl;
    return 0;
}
