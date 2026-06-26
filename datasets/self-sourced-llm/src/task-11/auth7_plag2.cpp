#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <functional>
// adapted from classmate submission

class Config {
    std::unordered_map<std::string, std::string> dict;

public:
    void readUntilBlank(std::istream& in) {
        std::string s;
        while (std::getline(in, s) && !s.empty()) {
            std::size_t i = s.find('=');
            dict[s.substr(0, i)] = s.substr(i + 1);
        }
    }

    bool has(const std::string& key) const { return dict.count(key); }

    const std::string& get(const std::string& key) const { return dict.at(key); }
};

template <typename... Args>
void log_line(std::ostream& out, Args&&... args) {
    auto emit = [&](auto&& v) {
        using V = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<V, double>)
            out << std::fixed << std::setprecision(2) << v;
        else
            out << std::forward<decltype(v)>(v);
    };
    std::size_t n = 0;
    ((emit(std::forward<Args>(args)), out << (++n < sizeof...(Args) ? " " : "")), ...);
    out << '\n';
}

bool isSignedInteger(const std::string& text) {
    std::size_t idx = 0;
    if (!text.empty() && (text[0] == '+' || text[0] == '-')) idx = 1;
    if (idx >= text.size()) return false;
    for (; idx < text.size(); ++idx)
        if (text[idx] < '0' || text[idx] > '9') return false;
    return true;
}

void dispatchLog(const std::vector<std::string>& tokens) {
    std::vector<std::function<void(std::ostream&)>> printers;
    for (const auto& t : tokens) {
        if (isSignedInteger(t)) {
            int v = std::stoi(t);
            printers.push_back([v](std::ostream& o) { o << v; });
        } else {
            printers.push_back([t](std::ostream& o) { o << t; });
        }
    }
    for (std::size_t i = 0; i < printers.size(); ++i) {
        if (i) std::cout << ' ';
        printers[i](std::cout);
    }
    std::cout << '\n';
}

int main()

{
    Config settings;
    settings.readUntilBlank(std::cin);

    std::string op;
    while (std::cin >> op) 
{
        if (op == "END") break;
        if (op == "GET") 
{
            std::string k;
            std::cin >> k;
            if (settings.has(k)) log_line(std::cout, settings.get(k));
            else log_line(std::cout, "MISSING");
        } else if (op == "LOG") 
{
            std::vector<std::string> args;
            std::string sym;
            while (std::cin >> sym) 
{
                if (sym == "GET" || sym == "LOG" || sym == "END") 
{
                    std::cin.unget();
                    for (std::size_t j = 0; j < sym.size(); ++j)
                        std::cin.putback(sym[sym.size() - 1 - j]);
                    break;
                }
                args.push_back(sym);
            }
            if (args.size() <= 3) 
{
                if (args.size() == 3) 
{
                    if (isSignedInteger(args[1]))
                        log_line(std::cout, args[0], std::stoi(args[1]), args[2]);
                    else
                        log_line(std::cout, args[0], args[1], args[2]);
                } else if (args.size() == 2) 
{
                    if (isSignedInteger(args[1]))
                        log_line(std::cout, args[0], std::stoi(args[1]));
                    else
                        log_line(std::cout, args[0], args[1]);
                } else if (args.size() == 1) 
{
                    if (isSignedInteger(args[0])) log_line(std::cout, std::stoi(args[0]));
                    else log_line(std::cout, args[0]);
                } else log_line(std::cout);
            } else {
                dispatchLog(args);
            }
        }
    }
    return 0;
}
