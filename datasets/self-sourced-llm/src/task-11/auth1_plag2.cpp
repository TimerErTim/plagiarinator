#include <vector>
#include <utility>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
// adapted from classmate submission

template <typename... Args>
void log_line(std::ostream& out, Args&&... args) {
    ((out << std::forward<Args>(args) << " "), ...);
    out << "\n";
}

class Config {
    std::unordered_map<std::string, std::string> store_;

public:
    void read(std::istream& in) {
        std::string line;
        while (std::getline(in, line) && !line.empty()) {
            auto pos = line.find('=');
            if (pos != std::string::npos)
                store_[line.substr(0, pos)] = line.substr(pos + 1);
        }
    }

    bool has(const std::string& key) const {
        return store_.find(key) != store_.end();
    }

    const std::string& get(const std::string& key) const {
        return store_.at(key);
    }
};

static bool token_is_int(const std::string& s) {
    if (s.empty()) return false;
    std::size_t i = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    if (i >= s.size()) return false;
    for (; i < s.size(); ++i)
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    return true;
}

struct LogArg {
    bool as_int;
    int i_val;
    std::string s_val;
};

static LogArg build_arg(const std::string& t) {
    if (token_is_int(t)) return LogArg{true, std::stoi(t), {}};
    return LogArg{false, 0, t};
}

static void print_log(std::ostream& out, const std::vector<LogArg>& args) {
    if (args.size() == 1) {
        if (args[0].as_int) log_line(out, args[0].i_val);
        else log_line(out, args[0].s_val);
    } else if (args.size() == 2) {
        if (args[0].as_int && args[1].as_int) log_line(out, args[0].i_val, args[1].i_val);
        else if (args[0].as_int) log_line(out, args[0].i_val, args[1].s_val);
        else if (args[1].as_int) log_line(out, args[0].s_val, args[1].i_val);
        else log_line(out, args[0].s_val, args[1].s_val);
    } else if (args.size() == 3) {
        const LogArg& a0 = args[0], &a1 = args[1], &a2 = args[2];
        if (a1.as_int) {
            if (a0.as_int && a2.as_int) log_line(out, a0.i_val, a1.i_val, a2.i_val);
            else if (a0.as_int) log_line(out, a0.i_val, a1.i_val, a2.s_val);
            else if (a2.as_int) log_line(out, a0.s_val, a1.i_val, a2.i_val);
            else log_line(out, a0.s_val, a1.i_val, a2.s_val);
        } else {
            log_line(out, a0.as_int ? std::to_string(a0.i_val) : a0.s_val, a1.s_val,
                     a2.as_int ? std::to_string(a2.i_val) : a2.s_val);
        }
    } else if (args.empty()) {
        out << "\n";
    } else {
        for (std::size_t i = 0; i < args.size(); ++i) {
            if (i) out << ' ';
            if (args[i].as_int) out << args[i].i_val;
            else out << args[i].s_val;
        }
        out << '\n';
    }
}

int main()

{
    Config settings;
    settings.read(std::cin);

    std::string op;
    while (std::cin >> op && op != "END") 
{
        if (op == "GET") 
{
            std::string k;
            std::cin >> k;
            std::cout << (settings.has(k) ? settings.get(k) : std::string("MISSING")) << "\n";
        } else if (op == "LOG") 
{
            std::vector<std::string> tokens;
            std::string sym;
            while (std::cin >> sym) 
{
                if (sym == "GET" || sym == "LOG" || sym == "END") 
{
                    for (auto it = sym.rbegin(); it != sym.rend(); ++it)
                        std::cin.putback(*it);
                    break;
                }
                tokens.push_back(sym);
            }
            std::vector<LogArg> parsed;
            for (const auto& t : tokens) parsed.push_back(build_arg(t));
            print_log(std::cout, parsed);
        }
    }
    return 0;
}
