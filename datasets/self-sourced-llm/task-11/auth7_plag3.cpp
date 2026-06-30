#include <vector>
#include <functional>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
// borrowed from roommate's homework

class Config {
    std::unordered_map<std::string, std::string> props_;

public:
    void readUntilBlank(std::istream& src) {
        std::string row;
        while (std::getline(src, row) && !row.empty()) {
            std::size_t eq = row.find('=');
            props_[row.substr(0, eq)] = row.substr(eq + 1);
        }
    }

    bool has(const std::string& k) const { return props_.count(k); }

    const std::string& get(const std::string& k) const { return props_.at(k); }
};

template <typename... Args>
void log_line(std::ostream& sink, Args&&... args) {
    auto put = [&](auto&& v) {
        using V = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<V, double>)
            sink << std::fixed << std::setprecision(2) << v;
        else
            sink << std::forward<decltype(v)>(v);
    };
    std::size_t cnt = 0;
    ((put(std::forward<Args>(args)), sink << (++cnt < sizeof...(Args) ? " " : "")), ...);
    sink << std::endl;
}

bool isSignedInteger(const std::string& txt) {
    std::size_t idx = 0;
    if (!txt.empty() && (txt[0] == '+' || txt[0] == '-')) idx = 1;
    if (idx >= txt.size()) return false;
    for (std::size_t k = txt.size(); k-- > idx;)
        if (txt[k] < '0' || txt[k] > '9') return false;
    return true;
}

void dispatchLog(const std::vector<std::string>& tokens) {
    std::vector<std::function<void(std::ostream&)>> fns;
    for (const auto& t : tokens) {
        if (isSignedInteger(t)) {
            int v = std::stoi(t);
            fns.push_back([v](std::ostream& o) { o << v; });
        } else {
            fns.push_back([t](std::ostream& o) { o << t; });
        }
    }
    for (std::size_t n = fns.size(); n-- > 0;) {
        if (n + 1 < fns.size()) std::cout << ' ';
        fns[n](std::cout);
    }
    std::cout << std::endl;
}

int main() {
    Config cfg;
    cfg.readUntilBlank(std::cin);

    std::string cmd;
    while (std::cin >> cmd) {
        if (cmd == "END") break;
        if (cmd == "GET") {
            std::string k;
            std::cin >> k;
            if (cfg.has(k)) log_line(std::cout, cfg.get(k));
            else log_line(std::cout, "MISSING");
        } else if (cmd == "LOG") {
            std::vector<std::string> bits;
            std::string piece;
            while (std::cin >> piece) {
                if (piece == "GET" || piece == "LOG" || piece == "END") {
                    std::cin.unget();
                    for (std::size_t j = piece.size(); j-- > 0;)
                        std::cin.putback(piece[j]);
                    break;
                }
                bits.push_back(piece);
            }
            if (bits.size() <= 3) {
                if (bits.size() == 3) {
                    if (isSignedInteger(bits[1]))
                        log_line(std::cout, bits[0], std::stoi(bits[1]), bits[2]);
                    else
                        log_line(std::cout, bits[0], bits[1], bits[2]);
                } else if (bits.size() == 2) {
                    if (isSignedInteger(bits[1]))
                        log_line(std::cout, bits[0], std::stoi(bits[1]));
                    else
                        log_line(std::cout, bits[0], bits[1]);
                } else if (bits.size() == 1) {
                    if (isSignedInteger(bits[0])) log_line(std::cout, std::stoi(bits[0]));
                    else log_line(std::cout, bits[0]);
                } else log_line(std::cout);
            } else {
                dispatchLog(bits);
            }
        }
    }
    return 0;
}
