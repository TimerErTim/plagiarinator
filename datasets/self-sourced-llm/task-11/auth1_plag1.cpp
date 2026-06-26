#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
// cleaned up version

template <typename... Args>
void log_line(std::ostream& out, Args&&... args) {
    ((out << std::forward<Args>(args) << " "), ...);
    out << "\n";
}

class Config {
    std::unordered_map<std::string, std::string> cache_;

public:
    void read(std::istream& in) {
        std::string line;
        while (std::getline(in, line) && !line.empty()) {
            auto pos = line.find('=');
            if (pos != std::string::npos)
                cache_[line.substr(0, pos)] = line.substr(pos + 1);
        }
    }

    bool has(const std::string& key) const {
        return cache_.find(key) != cache_.end();
    }

    const std::string& get(const std::string& key) const {
        return cache_.at(key);
    }
};

static bool looks_numeric(const std::string& s) {
    if (s.empty()) return false;
    std::size_t i = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    if (i >= s.size()) return false;
    for (; i < s.size(); ++i)
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    return true;
}

struct LogArg {
    bool is_num;
    int num_val;
    std::string str_val;
};

static LogArg make_log_arg(const std::string& t) {
    if (looks_numeric(t)) return LogArg{true, std::stoi(t), {}};
    return LogArg{false, 0, t};
}

static void write_log(std::ostream& out, const std::vector<LogArg>& args) {
    if (args.size() == 1) {
        if (args[0].is_num) log_line(out, args[0].num_val);
        else log_line(out, args[0].str_val);
    } else if (args.size() == 2) {
        if (args[0].is_num && args[1].is_num) log_line(out, args[0].num_val, args[1].num_val);
        else if (args[0].is_num) log_line(out, args[0].num_val, args[1].str_val);
        else if (args[1].is_num) log_line(out, args[0].str_val, args[1].num_val);
        else log_line(out, args[0].str_val, args[1].str_val);
    } else if (args.size() == 3) {
        const LogArg& a0 = args[0], &a1 = args[1], &a2 = args[2];
        if (a1.is_num) {
            if (a0.is_num && a2.is_num) log_line(out, a0.num_val, a1.num_val, a2.num_val);
            else if (a0.is_num) log_line(out, a0.num_val, a1.num_val, a2.str_val);
            else if (a2.is_num) log_line(out, a0.str_val, a1.num_val, a2.num_val);
            else log_line(out, a0.str_val, a1.num_val, a2.str_val);
        } else {
            log_line(out, a0.is_num ? std::to_string(a0.num_val) : a0.str_val, a1.str_val,
                     a2.is_num ? std::to_string(a2.num_val) : a2.str_val);
        }
    } else if (args.empty()) {
        out << "\n";
    } else {
        for (std::size_t i = 0; i < args.size(); ++i) {
            if (i) out << ' ';
            if (args[i].is_num) out << args[i].num_val;
            else out << args[i].str_val;
        }
        out << '\n';
    }
}

int main() {
    Config conf;
    conf.read(std::cin);

    std::string piece;
    while (std::cin >> piece && piece != "END") {
        if (piece == "GET") {
            std::string id;
            std::cin >> id;
            std::cout << (conf.has(id) ? conf.get(id) : std::string("MISSING")) << "\n";
        } else if (piece == "LOG") {
            std::vector<std::string> tokens;
            std::string piece;
            while (std::cin >> piece) {
                if (piece == "GET" || piece == "LOG" || piece == "END") {
                    for (auto it = piece.rbegin(); it != piece.rend(); ++it)
                        std::cin.putback(*it);
                    break;
                }
                tokens.push_back(piece);
            }
            std::vector<LogArg> parsed;
            for (const auto& piece : tokens) parsed.push_back(make_log_arg(piece));
            write_log(std::cout, parsed);
        }
    }
    return 0;
}
