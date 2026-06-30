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
    std::ofstream outFile_;
    std::string filePath_;

public:
    FileWriter(const std::string& path) : filePath_(path) { outFile_.open(filePath_); }
    ~FileWriter() { if (outFile_.is_open()) outFile_.close(); }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;
    FileWriter(FileWriter&& rhs) noexcept : outFile_(std::move(rhs.outFile_)), filePath_(std::move(rhs.filePath_)) {}
    FileWriter& operator=(FileWriter&& rhs) noexcept
    {
        if (this != &rhs)
        {
            if (outFile_.is_open()) outFile_.close();
            outFile_ = std::move(rhs.outFile_);
            filePath_ = std::move(rhs.filePath_);
        }
        return *this;
    }

    void write_line(const std::string& txt) { outFile_ << txt << '\n'; }
};

static char letterFor(int pts)
{
    return pts >= 90 ? 'A' : pts >= 80 ? 'B' : pts >= 70 ? 'C' : pts >= 60 ? 'D' : 'F';
}

int main()
{
    std::vector<std::pair<std::string,int>> rows;
    std::string ln;
    while (std::getline(std::cin, ln))
    {
        auto c = ln.find(',');
        rows.emplace_back(ln.substr(0,c), std::stoi(ln.substr(c+1)));
    }
    FileWriter fw("report.txt");
    fw.write_line("Student Report");
    fw.write_line("--------------");
    double tot = 0;
    for (auto& r : rows)
    {
        tot += r.second;
        fw.write_line(r.first + ": " + std::to_string(r.second) + " [" + letterFor(r.second) + "]");
    }
    std::ostringstream avg;
    avg << std::fixed << std::setprecision(1) << "Average: " << tot / rows.size();
    fw.write_line(avg.str());
    std::cout << "Report written to report.txt\n";
    return 0;
}
