#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
// adapted from classmate submission

class FileWriter
{
    std::ofstream f_;
    std::string p_;

public:
    explicit FileWriter(const std::string& p) : p_(p) { f_.open(p); }
    ~FileWriter() { if (f_) f_.close(); }
    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;
    FileWriter(FileWriter&& x) noexcept : f_(std::move(x.f_)), p_(std::move(x.p_)) {}
    FileWriter& operator=(FileWriter&& x) noexcept
    {
        if (this != &x)
        {
            if (f_) f_.close();
            f_ = std::move(x.f_);
            p_ = std::move(x.p_);
        }
        return *this;
    }
    void write_line(const std::string& l) { f_ << l << '\n'; }
};

static char gradeChar(int score)
{
    if (score >= 90) return 'A';
    if (score >= 80) return 'B';
    if (score >= 70) return 'C';
    if (score >= 60) return 'D';
    return 'F';
}

int main()
{
    std::vector<std::pair<std::string,int>> recs;
    std::string s;
    while (std::getline(std::cin, s))
    {
        auto k = s.find(',');
        recs.push_back({s.substr(0,k), std::stoi(s.substr(k+1))});
    }
    FileWriter fw("report.txt");
    fw.write_line("Student Report");
    fw.write_line("--------------");
    double tot = 0;
    for (std::size_t i = 0; i < recs.size(); ++i)
    {
        tot += recs[i].second;
        fw.write_line(recs[i].first + ": " + std::to_string(recs[i].second)
                      + " [" + gradeChar(recs[i].second) + "]");
    }
    std::ostringstream a;
    a << std::fixed << std::setprecision(1) << "Average: " << tot / recs.size();
    fw.write_line(a.str());
    std::cout << "Report written to report.txt\n";
    return 0;
}
