#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct StreamLogger {
    std::ostream& out;
    bool first = true;

    template <typename T>
    void emit(T&& v)
    {
        if (!first) out << ' ';
        first = false;
        using D = std::decay_t<T>;
        if constexpr (std::is_same_v<D, double>)
            out << std::fixed << std::setprecision(2) << v;
        else
            out << std::forward<T>(v);
    }

    void newline() { out << '\n'; }
};

template <typename... Args>
void log_line(std::ostream& out, Args&&... args)
{
    StreamLogger helper{out};
    (helper.emit(std::forward<Args>(args)), ...);
    helper.newline();
}

class Config
{
    std::unordered_map<std::string, std::string> table_;

public:
    void read_section(std::istream& in)
    {
        std::string line;
        while (std::getline(in, line) && !line.empty())
        {
            auto eq = line.find('=');
            table_.emplace(line.substr(0, eq), line.substr(eq + 1));
        }
    }

    bool has(const std::string& key) const { return table_.find(key) != table_.end(); }

    const std::string& get(const std::string& key) const { return table_.at(key); }
};

static bool looks_like_int(const std::string& s)
{
    if (s.empty()) return false;
    std::size_t i = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    if (i >= s.size()) return false;
    for (; i < s.size(); ++i)
        if (s[i] < '0' || s[i] > '9') return false;
    return true;
}

static void log_one(std::ostream& out, const std::string& s)
{
    if (looks_like_int(s)) log_line(out, std::stoi(s));
    else log_line(out, s);
}

static void log_pair(std::ostream& out, const std::string& a, const std::string& b)
{
    if (looks_like_int(b)) log_line(out, a, std::stoi(b));
    else log_line(out, a, b);
}

static void log_triple(std::ostream& out, const std::string& a, const std::string& b, const std::string& c)
{
    if (looks_like_int(b)) log_line(out, a, std::stoi(b), c);
    else log_line(out, a, b, c);
}

void log_values(std::ostream& out, const std::vector<std::string>& raw)
{
    if (raw.empty()) { out << '\n'; return; }
    if (raw.size() == 1) log_one(out, raw[0]);
    else if (raw.size() == 2) log_pair(out, raw[0], raw[1]);
    else if (raw.size() == 3) log_triple(out, raw[0], raw[1], raw[2]);
    else
    {
        StreamLogger lg{out};
        for (std::size_t k = raw.size(); k-- > 0;)
        {
            if (looks_like_int(raw[k])) lg.emit(std::stoi(raw[k]));
            else lg.emit(raw[k]);
        }
        lg.newline();
    }
}

int main()
{
    Config cfg;
    cfg.read_section(std::cin);

    std::string token;
    while (std::cin >> token)
    {
        if (token == "END") break;
        if (token == "GET")
        {
            std::string key;
            std::cin >> key;
            std::cout << (cfg.has(key) ? cfg.get(key) : std::string("MISSING")) << '\n';
        }
        else if (token == "LOG")
        {
            std::vector<std::string> raw;
            std::string t;
            while (std::cin >> t)
            {
                if (t == "GET" || t == "LOG" || t == "END")
                {
                    for (auto r = t.rbegin(); r != t.rend(); ++r) std::cin.putback(*r);
                    break;
                }
                raw.push_back(t);
            }
            log_values(std::cout, raw);
        }
    }
    return 0;
}
