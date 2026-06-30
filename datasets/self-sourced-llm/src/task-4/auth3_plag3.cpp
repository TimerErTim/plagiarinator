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
    std::ofstream outHandle_;
    std::string pathName_;

    void closeIfOpen()
    {
        if (outHandle_.is_open()) outHandle_.close();
    }

public:
    explicit FileWriter(const std::string& fname) : pathName_(fname)
    {
        outHandle_.open(pathName_);
    }

    ~FileWriter() { closeIfOpen(); }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& mv) noexcept
        : outHandle_(std::move(mv.outHandle_)), pathName_(std::move(mv.pathName_)) {}

    FileWriter& operator=(FileWriter&& mv) noexcept
    {
        if (this != &mv)
        {
            closeIfOpen();
            outHandle_ = std::move(mv.outHandle_);
            pathName_ = std::move(mv.pathName_);
        }
        return *this;
    }

    void write_line(const std::string& text)
    {
        outHandle_ << text << std::endl;
    }
};

static std::string gradeToken(int pts)
{
    if (pts >= 90) return "A";
    if (pts >= 80) return "B";
    if (pts >= 70) return "C";
    if (pts >= 60) return "D";
    return "F";
}

int main()
{
    std::vector<std::pair<std::string, int>> roster;
    for (std::string raw; std::getline(std::cin, raw);)
    {
        std::size_t sep = raw.find(',');
        roster.emplace_back(raw.substr(0, sep), std::stoi(raw.substr(sep + 1)));
    }

    FileWriter report("report.txt");
    report.write_line("Student Report");
    report.write_line("--------------");

    double acc = 0.0;
    for (const auto& entry : roster)
    {
        acc += entry.second;
        std::ostringstream row;
        row << entry.first << ": " << entry.second << " [" << gradeToken(entry.second) << "]";
        report.write_line(row.str());
    }

    std::ostringstream footer;
    footer << std::fixed << std::setprecision(1)
           << "Average: " << (acc / static_cast<double>(roster.size()));
    report.write_line(footer.str());

    std::cout << "Report written to report.txt" << std::endl;
    return 0;
}
