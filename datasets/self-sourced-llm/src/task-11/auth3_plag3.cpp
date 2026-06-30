#include <vector>
#include <utility>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
// borrowed from roommate's homework

struct LinePrinter {
    std::ostream& sink;
    bool gap = false;

    template <typename T>
    void put(T&& v) {
        if (gap) sink << ' ';
        gap = true;
        using D = std::decay_t<T>;
        if constexpr (std::is_same_v<D, double>)
            sink << std::fixed << std::setprecision(2) << v;
        else
            sink << std::forward<T>(v);
    }

    void finish() { sink << std::endl; }
};

template <typename... Args>
void log_line(std::ostream& sink, Args&&... args) {
    LinePrinter lp{sink};
    (lp.put(std::forward<Args>(args)), ...);
    lp.finish();
}

class Config {
    std::unordered_map<std::string, std::string> tbl_;

public:
    void read_section(std::istream& src) {
        std::string row;
        while (std::getline(src, row) && !row.empty()) {
            auto eq = row.find('=');
            tbl_.emplace(row.substr(0, eq), row.substr(eq + 1));
        }
    }

    bool has(const std::string& k) const { return tbl_.find(k) != tbl_.end(); }

    const std::string& get(const std::string& k) const { return tbl_.at(k); }
};

bool digitish(const std::string& s) {
    if (s.empty()) return false;
    std::size_t p = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    if (p >= s.size()) return false;
    for (std::size_t k = s.size(); k-- > p;)
        if (s[k] < '0' || s[k] > '9') return false;
    return true;
}

void log_values(std::ostream& sink, const std::vector<std::string>& raw) {
    if (raw.empty()) { sink << std::endl; return; }
    if (raw.size() == 1) {
        if (digitish(raw[0])) log_line(sink, std::stoi(raw[0]));
        else log_line(sink, raw[0]);
    } else if (raw.size() == 2) {
        if (digitish(raw[1])) log_line(sink, raw[0], std::stoi(raw[1]));
        else log_line(sink, raw[0], raw[1]);
    } else if (raw.size() == 3) {
        if (digitish(raw[1])) log_line(sink, raw[0], std::stoi(raw[1]), raw[2]);
        else log_line(sink, raw[0], raw[1], raw[2]);
    } else {
        LinePrinter lp{sink};
        for (std::size_t n = raw.size(); n-- > 0;) {
            if (digitish(raw[n])) lp.put(std::stoi(raw[n]));
            else lp.put(raw[n]);
        }
        lp.finish();
    }
}

int main() {
    Config cfg;
    cfg.read_section(std::cin);

    std::string tok;
    while (std::cin >> tok) {
        if (tok == "END") break;
        if (tok == "GET") {
            std::string k;
            std::cin >> k;
            std::cout << (cfg.has(k) ? cfg.get(k) : std::string("MISSING")) << std::endl;
        } else if (tok == "LOG") {
            std::vector<std::string> raw;
            std::string piece;
            while (std::cin >> piece) {
                if (piece == "GET" || piece == "LOG" || piece == "END") {
                    for (std::size_t k = piece.size(); k-- > 0;)
                        std::cin.putback(piece[k]);
                    break;
                }
                raw.push_back(piece);
            }
            log_values(std::cout, raw);
        }
    }
    return 0;
}
