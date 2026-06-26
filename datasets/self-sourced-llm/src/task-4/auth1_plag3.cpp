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
    explicit FileWriter(const std::string& path) : tag(path) {
        body.open(tag);
    }

    ~FileWriter() {
        if (body.is_open()) body.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& other) noexcept
        : body(std::move(other.body)), tag(std::move(other.tag)) {}

    FileWriter& operator=(FileWriter&& other) noexcept {
        if (this != &other) {
            if (body.is_open()) body.close();
            body = std::move(other.body);
            tag = std::move(other.tag);
        }
        return *this;
    }

    void write_line(const std::string& line) {
        body << line << '\n';
    }
};

static char gradeFor(int score) {
    if (score >= 90) return 'A';
    if (score >= 80) return 'B';
    if (score >= 70) return 'C';
    if (score >= 60) return 'D';
    return 'F';
}

int main() {
    struct Record { std::string name; int score; };
    std::vector<Record> buf;
    std::string str;
    while (std::getline(std::cin, str)) {
        if (str.empty()) continue;
        std::size_t comma = str.find(',');
        if (comma == std::string::npos) continue;
        Record r;
        r.name = str.substr(0, comma);
        r.score = std::stoi(str.substr(comma + 1));
        buf.push_back(r);
    }

    FileWriter ref("report.txt");
    ref.write_line("Student Report");
    ref.write_line("--------------");
    double sum = 0.0;
    for (const auto& rec : buf) {
        sum += rec.score;
        std::ostringstream oss;
        oss << rec.name << ": " << rec.score << " [" << gradeFor(rec.score) << "]";
        ref.write_line(oss.str());
    }
    std::ostringstream avg;
    avg << std::fixed << std::setprecision(1) << "Average: " << (sum / buf.size());
    ref.write_line(avg.str());

    std::cout << "Report written to report.txt\n";
    return 0;
}
