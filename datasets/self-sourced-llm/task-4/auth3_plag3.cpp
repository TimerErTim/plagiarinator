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

    void closeIfOpen() {
        if (body.is_open()) body.close();
    }

public:
    explicit FileWriter(const std::string& fname) : tag(fname) {
        body.open(tag);
    }

    ~FileWriter() { closeIfOpen(); }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& mv) noexcept
        : body(std::move(mv.body)), tag(std::move(mv.tag)) {}

    FileWriter& operator=(FileWriter&& mv) noexcept {
        if (this != &mv) {
            closeIfOpen();
            body = std::move(mv.body);
            tag = std::move(mv.tag);
        }
        return *this;
    }

    void write_line(const std::string& text) {
        body << text << std::endl;
    }
};

static std::string gradeLabel(int pts) {
    if (pts >= 90) return "A";
    if (pts >= 80) return "B";
    if (pts >= 70) return "C";
    if (pts >= 60) return "D";
    return "F";
}

int main() {
    std::vector<std::pair<std::string, int>> students;
    for (std::string str; std::getline(std::cin, str);) {
        std::size_t sep = str.find(',');
        students.emplace_back(str.substr(0, sep), std::stoi(str.substr(sep + 1)));
    }

    FileWriter report("report.txt");
    report.write_line("Student Report");
    report.write_line("--------------");

    double acc = 0.0;
    for (const auto& entry : students) {
        acc += entry.second;
        std::ostringstream row;
        row << entry.first << ": " << entry.second << " [" << gradeLabel(entry.second) << "]";
        report.write_line(row.str());
    }

    std::ostringstream footer;
    footer << std::fixed << std::setprecision(1)
           << "Average: " << (acc / static_cast<double>(students.size()));
    report.write_line(footer.str());

    std::cout << "Report written to report.txt" << '\n';
    return 0;
}
