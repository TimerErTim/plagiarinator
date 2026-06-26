#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
// cleaned up version

struct StreamLogger {
    std::ostream& out;
    bool initial = true;

    template <typename T>
    void emit(T&& v) {
        if (!initial) out << ' ';
        initial = false;
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
    std::unordered_map<std::string, std::string> slots_;

public:
    void read_section(std::istream& in) {
        std::string line;
        while (std::getline(in, line) && !line.empty()) {
            auto eq = line.find('=');
            slots_.emplace(line.substr(0, eq), line.substr(eq + 1));
        }
    }

    bool has(const std::string& key) const { return slots_.find(key) != slots_.end(); }

    const std::string& get(const std::string& key) const { return slots_.at(key); }
};

bool is_digit_str(const std::string& s) {
    if (s.empty()) return false;
    std::size_t i = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    if (i >= s.size()) return false;
    for (; i < s.size(); ++i)
        if (s[i] < '0' || s[i] > '9') return false;
    return true;
}

void emit_tokens(std::ostream& out, const std::vector<std::string>& raw) {
    if (raw.empty()) { out << '\n'; return; }
    if (raw.size() == 1) {
        if (is_digit_str(raw[0])) log_line(out, std::stoi(raw[0]));
        else log_line(out, raw[0]);
    } else if (raw.size() == 2) {
        if (is_digit_str(raw[1])) log_line(out, raw[0], std::stoi(raw[1]));
        else log_line(out, raw[0], raw[1]);
    } else if (raw.size() == 3) {
        if (is_digit_str(raw[1]))
            log_line(out, raw[0], std::stoi(raw[1]), raw[2]);
        else
            log_line(out, raw[0], raw[1], raw[2]);
    } else {
        StreamLogger lg{out};
        for (const auto& t : raw) {
            if (is_digit_str(t)) lg.emit(std::stoi(t));
            else lg.emit(t);
        }
        lg.newline();
    }
}

int main() {
    Config conf;
    conf.read_section(std::cin);

    std::string piece;
    while (std::cin >> piece) {
        if (piece == "END") break;
        if (piece == "GET") {
            std::string id;
            std::cin >> id;
            std::cout << (conf.has(id) ? conf.get(id) : std::string("MISSING")) << '\n';
        } else if (piece == "LOG") {
            std::vector<std::string> raw;
            std::string piece;
            while (std::cin >> piece) {
                if (piece == "GET" || piece == "LOG" || piece == "END") {
                    for (auto rhs = piece.rbegin(); rhs != piece.rend(); ++rhs) std::cin.putback(*rhs);
                    break;
                }
                raw.push_back(piece);
            }
            emit_tokens(std::cout, raw);
        }
    }
    return 0;
}
