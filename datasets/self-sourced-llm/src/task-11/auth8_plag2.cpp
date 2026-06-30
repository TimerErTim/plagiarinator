#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>

template <typename... Args>
void log_line(std::ostream& out, Args&&... args)
{
    int idx = 0;
    auto put = [&](auto&& item)
    {
        if (idx++) out << ' ';
        using T = std::decay_t<decltype(item)>;
        if constexpr (std::is_same_v<T, double>)
            out << std::fixed << std::setprecision(2) << item;
        else
            out << std::forward<decltype(item)>(item);
    };
    (put(std::forward<Args>(args)), ...);
    out << '\n';
}

class Config
{
    std::unordered_map<std::string, std::string> store_;

public:
    Config(std::istream& in)
    {
        std::string row;
        while (std::getline(in, row) && row.size() > 0)
        {
            std::size_t eq = row.find('=');
            store_.insert({row.substr(0, eq), row.substr(eq + 1)});
        }
    }

    bool has(const std::string& key) const { return store_.count(key) > 0; }

    const std::string& get(const std::string& key) const { return store_.at(key); }
};

static bool parse_int_token(const std::string& tok)
{
    if (tok.empty()) return false;
    std::size_t start = 0;
    if (tok[0] == '+' || tok[0] == '-')
    {
        if (tok.size() == 1) return false;
        start = 1;
    }
    for (; start < tok.size(); ++start)
        if (tok[start] < '0' || tok[start] > '9') return false;
    return true;
}

static void run_log(std::istream& in)
{
    std::string a, b, c;
    in >> a;
    if (!(in >> b))
    {
        if (parse_int_token(a)) log_line(std::cout, std::stoi(a));
        else log_line(std::cout, a);
        return;
    }
    if (!(in >> c))
    {
        if (parse_int_token(b)) log_line(std::cout, a, std::stoi(b));
        else log_line(std::cout, a, b);
        in.clear();
        return;
    }
    if (parse_int_token(b)) log_line(std::cout, a, std::stoi(b), c);
    else log_line(std::cout, a, b, c);
}

int main()
{
    Config cfg(std::cin);
    std::string op;

    while (std::cin >> op)
    {
        if (op == "END") break;
        if (op == "GET")
        {
            std::string k;
            std::cin >> k;
            std::cout << (cfg.has(k) ? cfg.get(k) : std::string("MISSING")) << '\n';
        }
        else if (op == "LOG")
        {
            run_log(std::cin);
        }
    }
    return 0;
}
