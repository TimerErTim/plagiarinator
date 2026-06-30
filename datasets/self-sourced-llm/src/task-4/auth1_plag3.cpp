#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
// borrowed from roommate's homework

class FileWriter
{
    std::ofstream streamOut_;
    std::string destPath_;

public:
    explicit FileWriter(const std::string& path) : destPath_(path)
    {
        streamOut_.open(destPath_);
    }

    ~FileWriter()
    {
        if (streamOut_.is_open()) streamOut_.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& rhs) noexcept
        : streamOut_(std::move(rhs.streamOut_)), destPath_(std::move(rhs.destPath_)) {}

    FileWriter& operator=(FileWriter&& rhs) noexcept
    {
        if (this != &rhs)
        {
            if (streamOut_.is_open()) streamOut_.close();
            streamOut_ = std::move(rhs.streamOut_);
            destPath_ = std::move(rhs.destPath_);
        }
        return *this;
    }

    void write_line(const std::string& text)
    {
        streamOut_ << text << std::endl;
    }
};

static char letterFromScore(int pts)
{
    if (pts >= 90) return 'A';
    if (pts >= 80) return 'B';
    if (pts >= 70) return 'C';
    if (pts >= 60) return 'D';
    return 'F';
}

int main()
{
    struct Entry { std::string name; int score; };
    std::vector<Entry> entries;
    std::string raw;
    while (std::getline(std::cin, raw))
    {
        if (raw.empty()) continue;
        std::size_t sep = raw.find(',');
        if (sep == std::string::npos) continue;
        Entry e;
        e.name = raw.substr(0, sep);
        e.score = std::stoi(raw.substr(sep + 1));
        entries.push_back(e);
    }

    FileWriter fw("report.txt");
    fw.write_line("Student Report");
    fw.write_line("--------------");
    double acc = 0.0;
    for (const auto& e : entries)
    {
        acc += e.score;
        std::ostringstream row;
        row << e.name << ": " << e.score << " [" << letterFromScore(e.score) << "]";
        fw.write_line(row.str());
    }
    std::ostringstream footer;
    footer << std::fixed << std::setprecision(1) << "Average: " << (acc / entries.size());
    fw.write_line(footer.str());

    std::cout << "Report written to report.txt\n";
    return 0;
}
