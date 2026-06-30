#include <utility>
#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
// adapted from classmate submission

class FileWriter
{
    std::ofstream out_;
    std::string path_;

public:
    explicit FileWriter(const std::string& path) : path_(path)
    {
        out_.open(path_);
    }

    ~FileWriter()
    {
        if (out_.is_open()) out_.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& other) noexcept
        : out_(std::move(other.out_)), path_(std::move(other.path_)) {}

    FileWriter& operator=(FileWriter&& other) noexcept
    {
        if (this != &other)
        {
            if (out_.is_open()) out_.close();
            out_ = std::move(other.out_);
            path_ = std::move(other.path_);
        }
        return *this;
    }

    void write_line(const std::string& line)
    {
        out_ << line << '\n';
    }
};

struct Record { std::string name; int score; };

static char gradeFor(int score)
{
    if (score >= 90) return 'A';
    if (score >= 80) return 'B';
    if (score >= 70) return 'C';
    if (score >= 60) return 'D';
    return 'F';
}

static bool parseRecord(const std::string& line, Record& out)
{
    if (line.empty()) return false;
    std::size_t comma = line.find(',');
    if (comma == std::string::npos) return false;
    out.name = line.substr(0, comma);
    out.score = std::stoi(line.substr(comma + 1));
    return true;
}

int main()
{
    std::vector<Record> records;
    std::string line;
    while (std::getline(std::cin, line))
    {
        Record rec;
        if (parseRecord(line, rec))
            records.push_back(rec);
    }

    FileWriter writer("report.txt");
    writer.write_line("Student Report");
    writer.write_line("--------------");
    double runningSum = 0.0;
    for (std::size_t i = 0; i < records.size(); ++i)
    {
        runningSum += records[i].score;
        std::ostringstream oss;
        oss << records[i].name << ": " << records[i].score
            << " [" << gradeFor(records[i].score) << "]";
        writer.write_line(oss.str());
    }
    std::ostringstream avg;
    avg << std::fixed << std::setprecision(1) << "Average: " << (runningSum / records.size());
    writer.write_line(avg.str());

    std::cout << "Report written to report.txt\n";
    return 0;
}
