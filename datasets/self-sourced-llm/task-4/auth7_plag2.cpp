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
    explicit FileWriter(const std::string& p) : path_(p) { stream_.open(p); }
    ~FileWriter() { if (stream_) stream_.close(); }
    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;
    FileWriter(FileWriter&& x) noexcept : stream_(std::move(x.stream_)), path_(std::move(x.path_)) {}
    FileWriter& operator=(FileWriter&& x) noexcept {
        if (this != &x)
{
            if (stream_) stream_.close();
            stream_ = std::move(x.stream_);
            path_ = std::move(x.path_);
        }
        return *this;
    }
    void write_line(const std::string& l) { stream_ << l << '\n'; }
};

int main()
{
    std::vector<std::pair<std::string,int>> recs;
    for (std::string s; std::getline(std::cin, s);) {
        auto k = s.find(',');
        recs.emplace_back(s.substr(0,k), std::stoi(s.substr(k+1)));
    }
    FileWriter fw = FileWriter("report.txt");
    fw.write_line("Student Report");
    fw.write_line("--------------");
    double tot = 0;
    for (auto& r : recs)
{
        tot += r.second;
        char g = r.second >= 90 ? 'A' : r.second >= 80 ? 'B'
               : r.second >= 70 ? 'C' : r.second >= 60 ? 'D' : 'F';
        fw.write_line(r.first + ": " + std::to_string(r.second) + " [" + g + "]");
    }
    std::ostringstream a;
    a << std::fixed << std::setprecision(1) << "Average: " << tot / recs.size();
    fw.write_line(a.str());
    std::cout << "Report written to report.txt\n";
    return 0;
}
