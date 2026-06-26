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
    std::string fname_;

public:
    explicit FileWriter(const std::string& path) : fname_(path) {
        stream_.open(fname_);
    }

    ~FileWriter() {
        if (stream_.is_open()) stream_.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& other) noexcept
        : stream_(std::move(other.stream_)), fname_(std::move(other.fname_)) {}

    FileWriter& operator=(FileWriter&& other) noexcept {
        if (this != &other)
{
            if (stream_.is_open()) stream_.close();
            stream_ = std::move(other.stream_);
            fname_ = std::move(other.fname_);
        }
        return *this;
    }

    void write_line(const std::string& line) {
        stream_ << line << '\n';
    }
};

static char gradeFor(int score) {
    if (score >= 90) return 'A';
    if (score >= 80) return 'B';
    if (score >= 70) return 'C';
    if (score >= 60) return 'D';
    return 'F';
}

int main()
{
    struct Record { std::string name; int score; };
    std::vector<Record> data;
    std::string ln;
    while (std::getline(std::cin, ln)) {
        if (ln.empty()) continue;
        std::size_t comma = ln.find(',');
        if (comma == std::string::npos) continue;
        Record r;
        r.name = ln.substr(0, comma);
        r.score = std::stoi(ln.substr(comma + 1));
        data.push_back(r);
    }

    FileWriter fw("report.txt");
    fw.write_line("Student Report");
    fw.write_line("--------------");
    double tot = 0.0;
    for (const auto& row : data)
{
        tot += row.score;
        std::ostringstream oss;
        oss << row.name << ": " << row.score << " [" << gradeFor(row.score) << "]";
        fw.write_line(oss.str());
    }
    std::ostringstream avg;
    avg << std::fixed << std::setprecision(1) << "Average: " << (tot / data.size());
    fw.write_line(avg.str());

    std::cout << "Report written to report.txt\n";
    return 0;
}
