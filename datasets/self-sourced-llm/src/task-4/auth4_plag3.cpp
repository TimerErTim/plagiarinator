#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
// borrowed from roommate's homework

class FileWriter
{
private:
    std::ofstream outStream_;
    std::string destPath_;

public:
    explicit FileWriter(const std::string& path) : destPath_(path)
    {
        outStream_.open(destPath_);
    }

    ~FileWriter()
    {
        if (outStream_.is_open())
            outStream_.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& donor) noexcept
        : outStream_(std::move(donor.outStream_)), destPath_(std::move(donor.destPath_)) {}

    FileWriter& operator=(FileWriter&& donor) noexcept
    {
        if (&donor != this)
        {
            if (outStream_.is_open()) outStream_.close();
            outStream_ = std::move(donor.outStream_);
            destPath_ = std::move(donor.destPath_);
        }
        return *this;
    }

    void write_line(const std::string& txt)
    {
        outStream_ << txt << '\n';
    }
};

char gradeChar(int pts)
{
    if (pts >= 90) return 'A';
    if (pts >= 80) return 'B';
    if (pts >= 70) return 'C';
    if (pts >= 60) return 'D';
    return 'F';
}

void emitReport(FileWriter writer, const std::vector<std::pair<std::string, int>>& data)
{
    writer.write_line("Student Report");
    writer.write_line("--------------");
    double total = 0.0;
    for (const auto& item : data)
    {
        total += item.second;
        writer.write_line(item.first + ": " + std::to_string(item.second)
                          + " [" + gradeChar(item.second) + "]");
    }
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << "Average: " << total / data.size();
    writer.write_line(oss.str());
}

int main()
{
    std::vector<std::pair<std::string, int>> rows;
    std::string line;
    while (std::getline(std::cin, line))
    {
        auto comma = line.find(',');
        rows.emplace_back(line.substr(0, comma), std::stoi(line.substr(comma + 1)));
    }
    emitReport(FileWriter("report.txt"), rows);
    std::cout << "Report written to report.txt\n";
    return 0;
}
