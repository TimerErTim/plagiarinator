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

    void shutdown() {
        if (stream_.is_open())
            stream_.close();
    }

public:
    explicit FileWriter(const std::string& location)
        : fname_(location) {
        stream_.open(fname_);
    }

    ~FileWriter() { shutdown(); }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& donor) noexcept
        : stream_(std::move(donor.stream_)),
          fname_(std::move(donor.fname_)) {}

    FileWriter& operator=(FileWriter&& donor) noexcept {
        if (this != &donor)
{
            shutdown();
            stream_ = std::move(donor.stream_);
            fname_ = std::move(donor.fname_);
        }
        return *this;
    }

    void write_line(const std::string& text) {
        stream_ << text << '\n';
    }
};

static std::string gradeToken(int score) {
    if (score >= 90) return "A";
    if (score >= 80) return "B";
    if (score >= 70) return "C";
    if (score >= 60) return "D";
    return "F";
}

int main()
{
    std::stringstream stdinCopy;
    stdinCopy << std::cin.rdbuf();

    std::vector<std::pair<std::string, int>> data;
    std::string ln;
    while (std::getline(stdinCopy, ln)) {
        std::size_t commaPos = ln.find(',');
        data.emplace_back(ln.substr(0, commaPos),
                          std::stoi(ln.substr(commaPos + 1)));
    }

    FileWriter reportWriter("report.txt");
    reportWriter.write_line("Student Report");
    reportWriter.write_line("--------------");

    double scoreSum = 0.0;
    for (const auto& row : data)
{
        scoreSum += row.second;
        reportWriter.write_line(row.first + ": " + std::to_string(row.second)
                                + " [" + gradeToken(row.second) + "]");
    }

    std::ostringstream avgStream;
    avgStream << std::fixed << std::setprecision(1)
              << "Average: " << scoreSum / data.size();
    reportWriter.write_line(avgStream.str());

    std::cout << "Report written to report.txt" << std::endl;
    return 0;
}
