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

    void closeIfOpen() {
        if (stream_.is_open()) stream_.close();
    }

public:
    explicit FileWriter(const std::string& fname) : fname_(fname) {
        stream_.open(fname_);
    }

    ~FileWriter() { closeIfOpen(); }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& mv) noexcept
        : stream_(std::move(mv.stream_)), fname_(std::move(mv.fname_)) {}

    FileWriter& operator=(FileWriter&& mv) noexcept {
        if (this != &mv)
{
            closeIfOpen();
            stream_ = std::move(mv.stream_);
            fname_ = std::move(mv.fname_);
        }
        return *this;
    }

    void write_line(const std::string& text) {
        stream_ << text << std::endl;
    }
};

static std::string gradeLabel(int pts) {
    if (pts >= 90) return "A";
    if (pts >= 80) return "B";
    if (pts >= 70) return "C";
    if (pts >= 60) return "D";
    return "F";
}

int main()
{
    std::vector<std::pair<std::string, int>> students;
    for (std::string ln; std::getline(std::cin, ln);) {
        std::size_t sep = ln.find(',');
        students.emplace_back(ln.substr(0, sep), std::stoi(ln.substr(sep + 1)));
    }

    FileWriter fw("report.txt");
    fw.write_line("Student Report");
    fw.write_line("--------------");

    double acc = 0.0;
    for (const auto& entry : students)
{
        acc += entry.second;
        std::ostringstream row;
        row << entry.first << ": " << entry.second << " [" << gradeLabel(entry.second) << "]";
        fw.write_line(row.str());
    }

    std::ostringstream footer;
    footer << std::fixed << std::setprecision(1)
           << "Average: " << (acc / static_cast<double>(students.size()));
    fw.write_line(footer.str());

    std::cout << "Report written to report.txt" << std::endl;
    return 0;
}
