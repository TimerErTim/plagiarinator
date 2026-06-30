#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

namespace detail {

inline void write_value(std::ostream& out, double v)
{
    out << std::fixed << std::setprecision(2) << v;
}

template <typename T>
void write_value(std::ostream& out, T&& v)
{
    out << std::forward<T>(v);
}

template <typename... Args>
void log_line(std::ostream& out, Args&&... args)
{
    bool first = true;
    auto emit = [&](auto&& val)
    {
        if (!first) out << ' ';
        first = false;
        write_value(out, std::forward<decltype(val)>(val));
    };
    (emit(std::forward<Args>(args)), ...);
    out << '\n';
}

}  // namespace detail

using detail::log_line;

class Config
{
    std::unordered_map<std::string, std::string> map_;

public:
    bool has(const std::string& key) const { return map_.count(key) > 0; }

    const std::string& get(const std::string& key) const { return map_.at(key); }

    void loadFromStream(std::istream& is)
    {
        std::string row;
        while (std::getline(is, row) && !row.empty())
        {
            std::size_t eq = row.find('=');
            map_.emplace(row.substr(0, eq), row.substr(eq + 1));
        }
    }
};

static bool allDigits(const std::string& text, std::size_t start)
{
    for (; start < text.size(); ++start)
        if (!isdigit(text[start])) return false;
    return start > 0;
}

static bool parseAsInt(const std::string& text, int& value)
{
    if (text.empty()) return false;
    std::size_t idx = 0;
    if (text[0] == '+' || text[0] == '-') idx = 1;
    if (!allDigits(text, idx)) return false;
    value = std::stoi(text);
    return true;
}

static void emitOne(std::ostream& out, const std::string& s)
{
    int n;
    if (parseAsInt(s, n)) log_line(out, n);
    else log_line(out, s);
}

static void emitTwo(std::ostream& out, const std::string& a, const std::string& b)
{
    int n;
    if (parseAsInt(b, n)) log_line(out, a, n);
    else log_line(out, a, b);
}

static void emitThree(std::ostream& out, const std::string& a, const std::string& b, const std::string& c)
{
    int n;
    if (parseAsInt(b, n)) log_line(out, a, n, c);
    else log_line(out, a, b, c);
}

void emitLogLine(std::ostream& out, const std::vector<std::string>& parts)
{
    if (parts.size() == 1) emitOne(out, parts[0]);
    else if (parts.size() == 2) emitTwo(out, parts[0], parts[1]);
    else if (parts.size() == 3) emitThree(out, parts[0], parts[1], parts[2]);
    else if (parts.empty()) out << '\n';
    else
    {
        for (std::size_t i = parts.size(); i-- > 0;)
        {
            if (i + 1 < parts.size()) out << ' ';
            int n;
            if (parseAsInt(parts[i], n)) out << n;
            else out << parts[i];
        }
        out << '\n';
    }
}

int main()
{
    Config config;
    config.loadFromStream(std::cin);

    std::string word;
    while (std::cin >> word)
    {
        if (word == "END") break;
        if (word == "GET")
        {
            std::string k;
            std::cin >> k;
            std::cout << (config.has(k) ? config.get(k) : std::string("MISSING")) << '\n';
        }
        else if (word == "LOG")
        {
            std::string rest;
            std::getline(std::cin, rest);
            std::istringstream iss(rest);
            std::vector<std::string> parts;
            for (std::string p; iss >> p; parts.push_back(p)) {}
            emitLogLine(std::cout, parts);
        }
    }
    return 0;
}
