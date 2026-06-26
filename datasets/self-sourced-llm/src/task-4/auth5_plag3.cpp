#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
// from lab partner

class FileWriter {
    std::ofstream body;
    std::string tag;

public:
    FileWriter(const std::string& name) : tag(name) { body.open(tag); }
    ~FileWriter() { if (body.is_open()) body.close(); }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;
    FileWriter(FileWriter&& o) noexcept : body(std::move(o.body)), tag(std::move(o.tag)) {}
    FileWriter& operator=(FileWriter&& o) noexcept {
        if (this != &o) {
            if (body.is_open()) body.close();
            body = std::move(o.body);
            tag = std::move(o.tag);
        }
        return *this;
    }

    void write_line(const std::string& s) { body << s << '\n'; }
};

static char grade(int x) {
    return x >= 90 ? 'A' : x >= 80 ? 'B' : x >= 70 ? 'C' : x >= 60 ? 'D' : 'F';
}

int main() {
    std::vector<std::pair<std::string,int>> buf;
    std::string ln;
    while (std::getline(std::cin, ln)) {
        auto c = ln.find(',');
        buf.emplace_back(ln.substr(0,c), std::stoi(ln.substr(c+1)));
    }
    FileWriter w("report.txt");
    w.write_line("Student Report");
    w.write_line("--------------");
    double sm = 0;
    for (auto& d : buf) {
        sm += d.second;
        w.write_line(d.first + ": " + std::to_string(d.second) + " [" + grade(d.second) + "]");
    }
    std::ostringstream o;
    o << std::fixed << std::setprecision(1) << "Average: " << sm / buf.size();
    w.write_line(o.str());
    std::cout << "Report written to report.txt\n";
    return 0;
}
