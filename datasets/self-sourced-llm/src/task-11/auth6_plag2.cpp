#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>

struct ConfigStore {
    std::unordered_map<std::string, std::string> values;

    bool has(const std::string& key) const
    {
        return values.find(key) != values.end();
    }

    const std::string& get(const std::string& key) const
    {
        return values.at(key);
    }
};

void print_one(std::ostream& out, double v)
{
    out << std::fixed << std::setprecision(2) << v;
}

template <typename T>
void print_one(std::ostream& out, T&& v)
{
    out << std::forward<T>(v);
}

struct LogWriter {
    std::ostream& os;
    bool sep = false;

    template <typename U>
    void step(U&& u)
    {
        if (sep) os << ' ';
        else sep = true;
        print_one(os, std::forward<U>(u));
    }
};

template <typename... Args>
void log_line(std::ostream& out, Args&&... args)
{
    LogWriter w{out};
    (w.step(std::forward<Args>(args)), ...);
    out << '\n';
}

static bool numeric_token(const std::string& s)
{
    if (s.empty()) return false;
    std::size_t k = 0;
    if (s[0] == '-' || s[0] == '+') k = 1;
    if (k >= s.size()) return false;
    for (; k < s.size(); ++k)
        if (s[k] < '0' || s[k] > '9') return false;
    return true;
}

static void load_config(ConfigStore& cfg, std::istream& in)
{
    std::string line;
    while (std::getline(in, line) && !line.empty())
    {
        auto p = line.find('=');
        cfg.values[line.substr(0, p)] = line.substr(p + 1);
    }
}

static void handle_log(std::istream& in)
{
    std::string t1, t2, t3;
    if (!(in >> t1)) return;
    if (in >> t2)
    {
        if (in >> t3)
        {
            if (numeric_token(t2))
                log_line(std::cout, t1, std::stoi(t2), t3);
            else
                log_line(std::cout, t1, t2, t3);
        }
        else
        {
            if (numeric_token(t2))
                log_line(std::cout, t1, std::stoi(t2));
            else
                log_line(std::cout, t1, t2);
            in.clear();
        }
    }
    else
    {
        if (numeric_token(t1)) log_line(std::cout, std::stoi(t1));
        else log_line(std::cout, t1);
    }
}

int main()
{
    ConfigStore cfg;
    load_config(cfg, std::cin);

    std::string verb;
    while (std::cin >> verb)
    {
        if (verb == "END") break;
        if (verb == "GET")
        {
            std::string k;
            std::cin >> k;
            log_line(std::cout, cfg.has(k) ? cfg.get(k) : std::string("MISSING"));
        }
        else if (verb == "LOG")
        {
            handle_log(std::cin);
        }
    }
    return 0;
}
