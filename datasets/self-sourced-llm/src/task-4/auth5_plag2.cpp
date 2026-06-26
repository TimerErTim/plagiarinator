#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
// adapted from classmate submission

class FileWriter {
    std::ofstream stream_;
    std::string path_;

public:
    FileWriter(const std::string& name) : path_(name) { stream_.open(path_); }
    ~FileWriter() { if (stream_.is_open()) stream_.close(); }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;
    FileWriter(FileWriter&& o) noexcept : stream_(std::move(o.stream_)), path_(std::move(o.path_)) {}
    FileWriter& operator=(FileWriter&& o) noexcept {
        if (this != &o)
{
            if (stream_.is_open()) stream_.close();
            stream_ = std::move(o.stream_);
            path_ = std::move(o.path_);
        }
        return *this;
    }

    void write_line(const std::string& s) { stream_ << s << '\n'; }
};

static char grade(int x) {
    return x >= 90 ? 'A' : x >= 80 ? 'B' : x >= 70 ? 'C' : x >= 60 ? 'D' : 'F';
}

int main()
{
    std::vector<std::pair<std::string,int>> data;
    std::string ln;
    while (std::getline(std::cin, ln)) {
        auto c = ln.find(',');
        data.emplace_back(ln.substr(0,c), std::stoi(ln.substr(c+1)));
    }
    FileWriter w("report.txt");
    w.write_line("Student Report");
    w.write_line("--------------");
    double sm = 0;
    for (auto& d : data)
{
        sm += d.second;
        w.write_line(d.first + ": " + std::to_string(d.second) + " [" + grade(d.second) + "]");
    }
    std::ostringstream o;
    o << std::fixed << std::setprecision(1) << "Average: " << sm / data.size();
    w.write_line(o.str());
    std::cout << "Report written to report.txt\n";
    return 0;
}
