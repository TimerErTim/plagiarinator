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
    std::ofstream out_;
    std::string path_;

public:
    explicit FileWriter(const std::string& path) : path_(path) { out_.open(path); }
    ~FileWriter() { if (out_) out_.close(); }
    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;
    FileWriter(FileWriter&& rhs) noexcept : out_(std::move(rhs.out_)), path_(std::move(rhs.path_)) {}
    FileWriter& operator=(FileWriter&& rhs) noexcept
    {
        if (this != &rhs)
        {
            if (out_) out_.close();
            out_ = std::move(rhs.out_);
            path_ = std::move(rhs.path_);
        }
        return *this;
    }
    void write_line(const std::string& txt) { out_ << txt << '\n'; }
};

int main()
{
    std::vector<std::pair<std::string,int>> records;
    for (std::string line; std::getline(std::cin, line);)
    {
        auto sep = line.find(',');
        records.emplace_back(line.substr(0, sep), std::stoi(line.substr(sep + 1)));
    }
    FileWriter fw("report.txt");
    fw.write_line("Student Report");
    fw.write_line("--------------");
    double total = 0;
    for (auto& r : records)
    {
        total += r.second;
        char g = r.second >= 90 ? 'A' : r.second >= 80 ? 'B'
               : r.second >= 70 ? 'C' : r.second >= 60 ? 'D' : 'F';
        fw.write_line(r.first + ": " + std::to_string(r.second) + " [" + g + "]");
    }
    std::ostringstream avg;
    avg << std::fixed << std::setprecision(1) << "Average: " << total / records.size();
    fw.write_line(avg.str());
    std::cout << "Report written to report.txt\n";
    return 0;
}
