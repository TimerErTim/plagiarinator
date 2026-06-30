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
    std::ofstream file_;
    std::string fname_;

public:
    explicit FileWriter(const std::string& fname) : fname_(fname)
    {
        file_.open(fname_);
    }

    ~FileWriter()
    {
        if (file_.is_open()) file_.close();
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& mv) noexcept
        : file_(std::move(mv.file_)), fname_(std::move(mv.fname_)) {}

    FileWriter& operator=(FileWriter&& mv) noexcept
    {
        if (this != &mv)
        {
            if (file_.is_open()) file_.close();
            file_ = std::move(mv.file_);
            fname_ = std::move(mv.fname_);
        }
        return *this;
    }

    void write_line(const std::string& text)
    {
        file_ << text << std::endl;
    }
};

static std::string gradeLabel(int pts)
{
    if (pts >= 90) return "A";
    if (pts >= 80) return "B";
    if (pts >= 70) return "C";
    if (pts >= 60) return "D";
    return "F";
}

static void writeStudentRow(FileWriter& w, const std::string& name, int score)
{
    std::ostringstream row;
    row << name << ": " << score << " [" << gradeLabel(score) << "]";
    w.write_line(row.str());
}

int main()
{
    std::vector<std::pair<std::string, int>> students;
    std::string raw;
    while (std::getline(std::cin, raw))
    {
        std::size_t sep = raw.find(',');
        students.push_back({raw.substr(0, sep), std::stoi(raw.substr(sep + 1))});
    }

    FileWriter report("report.txt");
    report.write_line("Student Report");
    report.write_line("--------------");

    double acc = 0.0;
    for (std::size_t i = 0; i < students.size(); ++i)
    {
        acc += students[i].second;
        writeStudentRow(report, students[i].first, students[i].second);
    }

    std::ostringstream footer;
    footer << std::fixed << std::setprecision(1)
           << "Average: " << (acc / static_cast<double>(students.size()));
    report.write_line(footer.str());

    std::cout << "Report written to report.txt" << std::endl;
    return 0;
}
