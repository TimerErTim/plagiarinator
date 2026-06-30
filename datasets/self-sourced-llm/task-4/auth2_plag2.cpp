#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
// adapted from classmate submission

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
    ofstream stream;
    string filename;

public:
    explicit FileWriter(const string& path): filename(path)
    {
        stream.open(filename);
    }

    ~FileWriter() { if (stream.is_open()) stream.close(); }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& src) noexcept
        : stream(std::move(src.stream)), filename(std::move(src.filename)) {}

    FileWriter& operator=(FileWriter&& src) noexcept
    {
        if (this != &src)
        {
            if (stream.is_open()) stream.close();
            stream = std::move(src.stream);
            filename = std::move(src.filename);
        }
        return *this;
    }

    void write_line(const string& text) { stream << text << '\n'; }
};

char letterGrade(int s)
{
    if (s >= 90) return 'A';
    if (s >= 80) return 'B';
    if (s >= 70) return 'C';
    if (s >= 60) return 'D';
    return 'F';
}

static bool parseRow(const string& row, string& name, int& score)
{
    auto pos = row.find(',');
    if (pos == string::npos) return false;
    name = row.substr(0, pos);
    score = stoi(row.substr(pos + 1));
    return true;
}

int main()
{
    vector<pair<string,int>> rows;
    string row;
    while (getline(cin, row))
    {
        string name;
        int score = 0;
        if (parseRow(row, name, score))
            rows.push_back({name, score});
    }

    FileWriter fw("report.txt");
    fw.write_line("Student Report");
    fw.write_line("--------------");
    double total = 0;
    for (std::size_t i = 0; i < rows.size(); ++i)
    {
        total += rows[i].second;
        fw.write_line(rows[i].first + ": " + to_string(rows[i].second)
                      + " [" + letterGrade(rows[i].second) + "]");
    }
    ostringstream avgLine;
    avgLine << fixed << setprecision(1) << "Average: " << total / rows.size();
    fw.write_line(avgLine.str());

    cout << "Report written to report.txt" << endl;
    return 0;
}
