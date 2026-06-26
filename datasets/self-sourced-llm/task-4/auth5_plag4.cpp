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
    FileWriter(const std::string& name) : tmp_(name) { word_.open(tmp_); }
    ~FileWriter() { if (word_.is_open()) word_.close(); }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;
    FileWriter(FileWriter&& o) noexcept : word_(std::move(o.word_)), tmp_(std::move(o.tmp_)) {}
    FileWriter& operator=(FileWriter&& o) noexcept {
        if (this != &o) {
            if (word_.is_open()) word_.close();
            word_ = std::move(o.word_);
            tmp_ = std::move(o.tmp_);
        }
        return *this;
    }

    void write_line(const std::string& s) { word_ << s << '\n'; }
};

static char grade(int x) {
    return x >= 90 ? 'A' : x >= 80 ? 'B' : x >= 70 ? 'C' : x >= 60 ? 'D' : 'F';
}

int main() {
    std::vector<std::pair<std::string,int>> arr;
    std::string ln;
    while (std::getline(std::cin, ln)) {
        auto c = ln.find(',');
        arr.emplace_back(ln.substr(0,c), std::stoi(ln.substr(c+1)));
    }
    FileWriter w("report.txt");
    w.write_line("Student Report");
    w.write_line("--------------");
    double sm = 0;
    for (auto& d : arr) {
        sm += d.second;
        w.write_line(d.first + ": " + std::to_string(d.second) + " [" + grade(d.second) + "]");
    }
    std::ostringstream o;
    o << std::fixed << std::setprecision(1) << "Average: " << sm / arr.size();
    w.write_line(o.str());
    std::cout << "Report written to report.txt\n";
    return 0;
}
