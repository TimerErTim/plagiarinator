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
    void emit(T&& v) {
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
void log_line(std::ostream& out, Args&&... args) {
    StreamLogger helper{out};
    (helper.emit(std::forward<Args>(args)), ...);
    helper.newline();
}

class Config {
    std::unordered_map<std::string, std::string> table_;

public:
    void read_section(std::istream& in) {
        std::string line;
        while (std::getline(in, line) && !line.empty()) {
            auto eq = line.find('=');
            table_.emplace(line.substr(0, eq), line.substr(eq + 1));
        }
    }

    bool has(const std::string& key) const { return table_.find(key) != table_.end(); }

    const std::string& get(const std::string& key) const { return table_.at(key); }
};

bool looks_like_int(const std::string& s) {
    if (s.empty()) return false;
    std::size_t i = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    if (i >= s.size()) return false;
    for (; i < s.size(); ++i)
        if (s[i] < '0' || s[i] > '9') return false;
    return true;
}

void log_values(std::ostream& out, const std::vector<std::string>& raw) {
    if (raw.empty()) { out << '\n'; return; }
    if (raw.size() == 1) {
        if (looks_like_int(raw[0])) log_line(out, std::stoi(raw[0]));
        else log_line(out, raw[0]);
    } else if (raw.size() == 2) {
        if (looks_like_int(raw[1])) log_line(out, raw[0], std::stoi(raw[1]));
        else log_line(out, raw[0], raw[1]);
    } else if (raw.size() == 3) {
        if (looks_like_int(raw[1]))
            log_line(out, raw[0], std::stoi(raw[1]), raw[2]);
        else
            log_line(out, raw[0], raw[1], raw[2]);
    } else {
        StreamLogger lg{out};
        for (const auto& t : raw) {
            if (looks_like_int(t)) lg.emit(std::stoi(t));
            else lg.emit(t);
        }
        lg.newline();
    }
}

int main() {
    Config cfg;
    cfg.read_section(std::cin);

    std::string token;
    while (std::cin >> token) {
        if (token == "END") break;
        if (token == "GET") {
            std::string key;
            std::cin >> key;
            std::cout << (cfg.has(key) ? cfg.get(key) : std::string("MISSING")) << '\n';
        } else if (token == "LOG") {
            std::vector<std::string> raw;
            std::string t;
            while (std::cin >> t) {
                if (t == "GET" || t == "LOG" || t == "END") {
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
