#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
// from lab partner

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
    ofstream body;
    string tag;

public:
    explicit FileWriter(const string& path): tag(path)
    {
        body.open(tag);
    }

    ~FileWriter() { if (body.is_open()) body.close(); }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& src) noexcept
        : body(std::move(src.body)), tag(std::move(src.tag)) {}

    FileWriter& operator=(FileWriter&& src) noexcept
    {
        if (this != &src) {
            if (body.is_open()) body.close();
            body = std::move(src.body);
            tag = std::move(src.tag);
        }
        return *this;
    }

    void write_line(const string& text) { body << text << '\n'; }
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
