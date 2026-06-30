#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

template <typename... Args>
void log_line(std::ostream& out, Args&&... args) {
    ((out << std::forward<Args>(args) << " "), ...);
    out << '\n';
}

class Config {
    std::unordered_map<std::string, std::string> entries_;

public:
    void read(std::istream& in) {
        std::string line;
        while (std::getline(in, line) && !line.empty()) {
            auto pos = line.find('=');
            if (pos != std::string::npos)
                entries_[line.substr(0, pos)] = line.substr(pos + 1);
        }
    }

    bool has(const std::string& key) const {
        return entries_.find(key) != entries_.end();
    }

    const std::string& get(const std::string& key) const {
        return entries_.at(key);
    }
};

static bool is_integer_token(const std::string& s) {
    if (s.empty()) return false;
    std::size_t i = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    if (i >= s.size()) return false;
    for (; i < s.size(); ++i)
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    return true;
}

struct LogArg {
    bool is_int;
    int iv;
    std::string sv;
};

static LogArg parse_log_arg(const std::string& t) {
    if (is_integer_token(t)) return LogArg{true, std::stoi(t), {}};
    return LogArg{false, 0, t};
}

static void emit_log(std::ostream& out, const std::vector<LogArg>& args) {
    if (args.size() == 1) {
        if (args[0].is_int) log_line(out, args[0].iv);
        else log_line(out, args[0].sv);
    } else if (args.size() == 2) {
        if (args[0].is_int && args[1].is_int) log_line(out, args[0].iv, args[1].iv);
        else if (args[0].is_int) log_line(out, args[0].iv, args[1].sv);
        else if (args[1].is_int) log_line(out, args[0].sv, args[1].iv);
        else log_line(out, args[0].sv, args[1].sv);
    } else if (args.size() == 3) {
        const LogArg& a0 = args[0], &a1 = args[1], &a2 = args[2];
        if (a1.is_int) {
            if (a0.is_int && a2.is_int) log_line(out, a0.iv, a1.iv, a2.iv);
            else if (a0.is_int) log_line(out, a0.iv, a1.iv, a2.sv);
            else if (a2.is_int) log_line(out, a0.sv, a1.iv, a2.iv);
            else log_line(out, a0.sv, a1.iv, a2.sv);
        } else {
            log_line(out, a0.is_int ? std::to_string(a0.iv) : a0.sv, a1.sv,
                     a2.is_int ? std::to_string(a2.iv) : a2.sv);
        }
    } else if (args.empty()) {
        out << '\n';
    } else {
        for (std::size_t i = 0; i < args.size(); ++i) {
            if (i) out << ' ';
            if (args[i].is_int) out << args[i].iv;
            else out << args[i].sv;
        }
        out << '\n';
    }
}

int main() {
    Config settings;
    settings.read(std::cin);

    std::string op;
    while (std::cin >> op && op != "END") {
        if (op == "GET") {
            std::string k;
            std::cin >> k;
            std::cout << (settings.has(k) ? settings.get(k) : std::string("MISSING")) << '\n';
        } else if (op == "LOG") {
            std::vector<std::string> words;
            std::string w;
            while (std::cin >> w) {
                if (w == "GET" || w == "LOG" || w == "END") {
                    for (auto it = w.rbegin(); it != w.rend(); ++it)
                        std::cin.putback(*it);
                    break;
                }
                words.push_back(w);
            }
            std::vector<LogArg> parsed;
            for (const auto& t : words) parsed.push_back(parse_log_arg(t));
            emit_log(std::cout, parsed);
        }
    }
    return 0;
}
