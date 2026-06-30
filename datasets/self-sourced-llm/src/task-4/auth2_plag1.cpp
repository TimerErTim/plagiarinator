#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>
// cleaned up version

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
    ofstream fileStream;
    string outPath;

public:
    explicit FileWriter(const string& path): outPath(path)
    {
        fileStream.open(outPath);
    }

    ~FileWriter() { if (fileStream.is_open()) fileStream.close(); }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& src) noexcept
        : fileStream(std::move(src.fileStream)), outPath(std::move(src.outPath)) {}

    FileWriter& operator=(FileWriter&& src) noexcept
    {
        if (this != &src) {
            if (fileStream.is_open()) fileStream.close();
            fileStream = std::move(src.fileStream);
            outPath = std::move(src.outPath);
        }
        return *this;
    }

    void write_line(const string& text) { fileStream << text << endl; }
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
    vector<pair<string,int>> entries;
    string row;
    while (getline(cin, row)) {
        auto pos = row.find(',');
        if (pos == string::npos) continue;
        entries.emplace_back(row.substr(0, pos), stoi(row.substr(pos + 1)));
    }

    FileWriter fw("report.txt");
    fw.write_line("Student Report");
    fw.write_line("--------------");
    double total = 0;
    for (auto& p : entries) {
        total += p.second;
        fw.write_line(p.first + ": " + to_string(p.second) + " [" + letterGrade(p.second) + "]");
    }
    ostringstream avgLine;
    avgLine << fixed << setprecision(1) << "Average: " << total / entries.size();
    fw.write_line(avgLine.str());

    cout << "Report written to report.txt" << endl;
    return 0;
}
