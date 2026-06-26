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
    explicit FileWriter(const std::string& p) : tag(p) { body.open(p); }
    ~FileWriter() { if (body) body.close(); }
    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;
    FileWriter(FileWriter&& x) noexcept : body(std::move(x.body)), tag(std::move(x.tag)) {}
    FileWriter& operator=(FileWriter&& x) noexcept {
        if (this != &x) {
            if (body) body.close();
            body = std::move(x.body);
            tag = std::move(x.tag);
        }
        return *this;
    }
    void write_line(const std::string& l) { body << l << '\n'; }
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
