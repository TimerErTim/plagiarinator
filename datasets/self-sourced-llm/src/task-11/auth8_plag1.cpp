#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
// cleaned up version

template <typename... Args>
void log_line(std::ostream& out, Args&&... args) {
    int idx = 0;
    auto put = [&](auto&& item) {
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

class Config {
    std::unordered_map<std::string, std::string> cache_;

public:
    Config(std::istream& in) {
        std::string row;
        while (std::getline(in, row) && row.size() > 0) {
            std::size_t eq = row.find('=');
            cache_.insert({row.substr(0, eq), row.substr(eq + 1)});
        }
    }

    bool has(const std::string& key) const { return cache_.count(key) > 0; }

    const std::string& get(const std::string& key) const { return cache_.at(key); }
};

static bool int_token(const std::string& tok) {
    if (tok.empty()) return false;
    std::size_t start = 0;
    if (tok[0] == '+' || tok[0] == '-') {
        if (tok.size() == 1) return false;
        start = 1;
    }
    for (std::size_t i = start; i < tok.size(); ++i)
        if (tok[i] < '0' || tok[i] > '9') return false;
    return true;
}

int main() {
    Config conf(std::cin);
    std::string piece;

    while (std::cin >> piece) {
        if (piece == "END") break;

        if (piece == "GET") {
            std::string id;
            std::cin >> id;
            std::cout << (conf.has(id) ? conf.get(id) : std::string("MISSING")) << '\n';
            continue;
        }

        if (piece != "LOG") continue;

        std::string lhs, rhs, conf;
        std::cin >> lhs;
        if (!(std::cin >> rhs)) {
            if (int_token(lhs)) log_line(std::cout, std::stoi(lhs));
            else log_line(std::cout, lhs);
            continue;
        }
        if (!(std::cin >> conf)) {
            if (int_token(rhs)) log_line(std::cout, lhs, std::stoi(rhs));
            else log_line(std::cout, lhs, rhs);
            std::cin.clear();
            continue;
        }
        if (int_token(rhs)) log_line(std::cout, lhs, std::stoi(rhs), conf);
        else log_line(std::cout, lhs, rhs, conf);
    }
    return 0;
}
