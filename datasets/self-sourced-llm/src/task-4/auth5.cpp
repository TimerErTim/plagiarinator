#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class FileWriter {
    std::ofstream handle_;
    std::string name_;

public:
    FileWriter(const std::string& name) : name_(name) { handle_.open(name_); }
    ~FileWriter() { if (handle_.is_open()) handle_.close(); }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;
    FileWriter(FileWriter&& o) noexcept : handle_(std::move(o.handle_)), name_(std::move(o.name_)) {}
    FileWriter& operator=(FileWriter&& o) noexcept {
        if (this != &o) {
            if (handle_.is_open()) handle_.close();
            handle_ = std::move(o.handle_);
            name_ = std::move(o.name_);
        }
        return *this;
    }

    void write_line(const std::string& s) { handle_ << s << '\n'; }
};

static char grade(int x) {
    return x >= 90 ? 'A' : x >= 80 ? 'B' : x >= 70 ? 'C' : x >= 60 ? 'D' : 'F';
}

int main() {
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
    for (auto& d : data) {
        sm += d.second;
        w.write_line(d.first + ": " + std::to_string(d.second) + " [" + grade(d.second) + "]");
    }
    std::ostringstream o;
    o << std::fixed << std::setprecision(1) << "Average: " << sm / data.size();
    w.write_line(o.str());
    std::cout << "Report written to report.txt\n";
    return 0;
}
