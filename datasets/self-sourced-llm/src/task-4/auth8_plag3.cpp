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
    std::ofstream fileOut_;
    std::string dest_;

    void shutdown()
    {
        if (fileOut_.is_open())
            fileOut_.close();
    }

public:
    explicit FileWriter(const std::string& dest)
        : dest_(dest)
    {
        fileOut_.open(dest_);
    }

    ~FileWriter() { shutdown(); }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& rhs) noexcept
        : fileOut_(std::move(rhs.fileOut_)),
          dest_(std::move(rhs.dest_)) {}

    FileWriter& operator=(FileWriter&& rhs) noexcept
    {
        if (this != &rhs)
        {
            shutdown();
            fileOut_ = std::move(rhs.fileOut_);
            dest_ = std::move(rhs.dest_);
        }
        return *this;
    }

    void write_line(const std::string& txt)
    {
        fileOut_ << txt << '\n';
    }
};

static std::string letterGrade(int pts)
{
    if (pts >= 90) return "A";
    if (pts >= 80) return "B";
    if (pts >= 70) return "C";
    if (pts >= 60) return "D";
    return "F";
}

int main()
{
    std::stringstream stdinCopy;
    stdinCopy << std::cin.rdbuf();

    std::vector<std::pair<std::string, int>> data;
    std::string ln;
    while (std::getline(stdinCopy, ln))
    {
        std::size_t pos = ln.find(',');
        data.emplace_back(ln.substr(0, pos), std::stoi(ln.substr(pos + 1)));
    }

    FileWriter w("report.txt");
    w.write_line("Student Report");
    w.write_line("--------------");

    double sum = 0.0;
    for (const auto& item : data)
    {
        sum += item.second;
        w.write_line(item.first + ": " + std::to_string(item.second)
                     + " [" + letterGrade(item.second) + "]");
    }

    std::ostringstream avg;
    avg << std::fixed << std::setprecision(1) << "Average: " << sum / data.size();
    w.write_line(avg.str());

    std::cout << "Report written to report.txt" << std::endl;
    return 0;
}
