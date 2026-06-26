#include <iomanip>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <utility>
// quick submit

using std::cin;
using std::cout;
using std::endl;
using std::fixed;
using std::getline;
using std::ofstream;
using std::ostringstream;
using std::pair;
using std::setprecision;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

class FileWriter
{
    ofstream word_;
    string tmp_;

public:
    explicit FileWriter(const string& path): tmp_(path)
    {
        word_.open(tmp_);
    }

    ~FileWriter() { if (word_.is_open()) word_.close(); }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& src) noexcept
        : word_(std::move(src.word_)), tmp_(std::move(src.tmp_)) {}

    FileWriter& operator=(FileWriter&& src) noexcept
    {
        if (this != &src) {
            if (word_.is_open()) word_.close();
            word_ = std::move(src.word_);
            tmp_ = std::move(src.tmp_);
        }
        return *this;
    }

    void write_line(const string& text) { word_ << text << '\n'; }
};

char letterGrade(int s)
{
    if (s >= 90) return 'A';
    if (s >= 80) return 'B';
    if (s >= 70) return 'C';
    if (s >= 60) return 'D';
    return 'F';
}

int main()
{
    vector<pair<string,int>> rows;
    string row;
    while (getline(cin, row)) {
        auto pos = row.find(',');
        if (pos == string::npos) continue;
        rows.emplace_back(row.substr(0, pos), stoi(row.substr(pos + 1)));
    }

    FileWriter fw("report.txt");
    fw.write_line("Student Report");
    fw.write_line("--------------");
    double total = 0;
    for (auto& p : rows) {
        total += p.second;
        fw.write_line(p.first + ": " + to_string(p.second) + " [" + letterGrade(p.second) + "]");
    }
    ostringstream avgLine;
    avgLine << fixed << setprecision(1) << "Average: " << total / rows.size();
    fw.write_line(avgLine.str());

    cout << "Report written to report.txt" << '\n';
    return 0;
}
