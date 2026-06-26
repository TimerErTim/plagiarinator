#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>
#include <string>
#include <vector>
// quick submit

class FileWriter {
    std::ofstream word_;
    std::string tmp_;

public:
    explicit FileWriter(const std::string& p) : tmp_(p) { word_.open(p); }
    ~FileWriter() { if (word_) word_.close(); }
    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;
    FileWriter(FileWriter&& x) noexcept : word_(std::move(x.word_)), tmp_(std::move(x.tmp_)) {}
    FileWriter& operator=(FileWriter&& x) noexcept {
        if (this != &x) {
            if (word_) word_.close();
            word_ = std::move(x.word_);
            tmp_ = std::move(x.tmp_);
        }
        return *this;
    }
    void write_line(const std::string& l) { word_ << l << '\n'; }
};

int main() {
    std::vector<std::pair<std::string,int>> recs;
    for (std::string s; std::getline(std::cin, s);) {
        auto k = s.find(',');
        recs.emplace_back(s.substr(0,k), std::stoi(s.substr(k+1)));
    }
    FileWriter fw = FileWriter("report.txt");
    fw.write_line("Student Report");
    fw.write_line("--------------");
    double tot = 0;
    for (auto& r : recs) {
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
