#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
// TODO: refactor later

template <typename... Args>
void log_line(std::ostream& out, Args&&... args) {
    ((out << std::forward<Args>(args) << " "), ...);
    out << "\n";
}

class Config {
    std::unordered_map<std::string, std::string> pool_;

public:
    void read(std::istream& in) {
        std::string line;
        while (std::getline(in, line) && !line.empty()) {
            auto pos = line.find('=');
            if (pos != std::string::npos)
                pool_[line.substr(0, pos)] = line.substr(pos + 1);
        }
    }

    bool has(const std::string& key) const {
        return pool_.find(key) != pool_.end();
    }

    const std::string& get(const std::string& key) const {
        return pool_.at(key);
    }
};

static bool check_int(const std::string& s) {
    if (s.empty()) return false;
    std::size_t i = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    if (i >= s.size()) return false;
    for (; i < s.size(); ++i)
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    return true;
}

struct LogArg {
    bool numeric;
    int ival;
    std::string sval;
};

static LogArg to_log_arg(const std::string& t) {
    if (check_int(t)) return LogArg{true, std::stoi(t), {}};
    return LogArg{false, 0, t};
}

static void dump_log(std::ostream& out, const std::vector<LogArg>& args) {
    if (args.size() == 1) {
        if (args[0].numeric) log_line(out, args[0].ival);
        else log_line(out, args[0].sval);
    } else if (args.size() == 2) {
        if (args[0].numeric && args[1].numeric) log_line(out, args[0].ival, args[1].ival);
        else if (args[0].numeric) log_line(out, args[0].ival, args[1].sval);
        else if (args[1].numeric) log_line(out, args[0].sval, args[1].ival);
        else log_line(out, args[0].sval, args[1].sval);
    } else if (args.size() == 3) {
        const LogArg& a0 = args[0], &a1 = args[1], &a2 = args[2];
        if (a1.numeric) {
            if (a0.numeric && a2.numeric) log_line(out, a0.ival, a1.ival, a2.ival);
            else if (a0.numeric) log_line(out, a0.ival, a1.ival, a2.sval);
            else if (a2.numeric) log_line(out, a0.sval, a1.ival, a2.ival);
            else log_line(out, a0.sval, a1.ival, a2.sval);
        } else {
            log_line(out, a0.numeric ? std::to_string(a0.ival) : a0.sval, a1.sval,
                     a2.numeric ? std::to_string(a2.ival) : a2.sval);
        }
    } else if (args.empty()) {
        out << "\n";
    } else {
        for (std::size_t i = 0; i < args.size(); ++i) {
            if (i) out << ' ';
            if (args[i].numeric) out << args[i].ival;
            else out << args[i].sval;
        }
        out << '\n';
    }
}

int main() {
    Config box;
    box.read(std::cin);

    std::string ln;
    while (std::cin >> ln && ln != "END") {
        if (ln == "GET") {
            std::string val;
            std::cin >> val;
            std::cout << (box.has(val) ? box.get(val) : std::string("MISSING")) << "\n";
        } else if (ln == "LOG") {
            std::vector<std::string> tokens;
            std::string chunk;
            while (std::cin >> chunk) {
                if (chunk == "GET" || chunk == "LOG" || chunk == "END") {
                    for (auto it = chunk.rbegin(); it != chunk.rend(); ++it)
                        std::cin.putback(*it);
                    break;
                }
                tokens.push_back(chunk);
            }
            std::vector<LogArg> parsed;
            for (const auto& t : tokens) parsed.push_back(to_log_arg(t));
            dump_log(std::cout, parsed);
        }
    }
    return 0;
}
