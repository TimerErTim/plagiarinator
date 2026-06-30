#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <utility>
#include <vector>
// borrowed from roommate's homework

using std::cin;
using std::cout;
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
    ofstream outHandle;
    string fileName;

public:
    explicit FileWriter(const string& path): fileName(path)
    {
        outHandle.open(fileName);
    }

    ~FileWriter() { if (outHandle.is_open()) outHandle.close(); }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    FileWriter(FileWriter&& donor) noexcept
        : outHandle(std::move(donor.outHandle)), fileName(std::move(donor.fileName)) {}

    FileWriter& operator=(FileWriter&& donor) noexcept
    {
        if (this != &donor)
        {
            if (outHandle.is_open()) outHandle.close();
            outHandle = std::move(donor.outHandle);
            fileName = std::move(donor.fileName);
        }
        return *this;
    }

    void write_line(const string& txt) { outHandle << txt << '\n'; }
};

char scoreLetter(int pts)
{
    if (pts >= 90) return 'A';
    if (pts >= 80) return 'B';
    if (pts >= 70) return 'C';
    if (pts >= 60) return 'D';
    return 'F';
}

int main()
{
    vector<pair<string,int>> data;
    string ln;
    while (getline(cin, ln))
    {
        auto k = ln.find(',');
        if (k == string::npos) continue;
        data.emplace_back(ln.substr(0, k), stoi(ln.substr(k + 1)));
    }

    FileWriter writer("report.txt");
    writer.write_line("Student Report");
    writer.write_line("--------------");
    double sum = 0;
    for (const auto& item : data)
    {
        sum += item.second;
        writer.write_line(item.first + ": " + to_string(item.second)
                          + " [" + scoreLetter(item.second) + "]");
    }
    ostringstream avg;
    avg << fixed << setprecision(1) << "Average: " << sum / data.size();
    writer.write_line(avg.str());

    cout << "Report written to report.txt\n";
    return 0;
}
