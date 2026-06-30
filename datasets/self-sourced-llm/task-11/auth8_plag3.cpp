#include <string>
#include <unordered_map>
#include <iostream>
#include <iomanip>
// borrowed from roommate's homework

template <typename... Args>
void log_line(std::ostream& sink, Args&&... args) {
    int pos = 0;
    auto emit = [&](auto&& item) {
        if (pos++) sink << ' ';
        using T = std::decay_t<decltype(item)>;
        if constexpr (std::is_same_v<T, double>)
            sink << std::fixed << std::setprecision(2) << item;
        else
            sink << std::forward<decltype(item)>(item);
    };
    (emit(std::forward<Args>(args)), ...);
    sink << std::endl;
}

class Config {
    std::unordered_map<std::string, std::string> map_;

public:
    Config(std::istream& src) {
        std::string row;
        while (std::getline(src, row) && row.size() > 0) {
            std::size_t eq = row.find('=');
            map_.insert({row.substr(0, eq), row.substr(eq + 1)});
        }
    }

    bool has(const std::string& k) const { return map_.count(k) > 0; }

    const std::string& get(const std::string& k) const { return map_.at(k); }
};

static bool parse_int_token(const std::string& tok) {
    if (tok.empty()) return false;
    std::size_t start = 0;
    if (tok[0] == '+' || tok[0] == '-') {
        if (tok.size() == 1) return false;
        start = 1;
    }
    for (std::size_t k = tok.size(); k-- > start;)
        if (tok[k] < '0' || tok[k] > '9') return false;
    return true;
}

int main() {
    Config cfg(std::cin);
    std::string op;

    while (std::cin >> op) {
        if (op == "END") break;

        if (op == "GET") {
            std::string k;
            std::cin >> k;
            std::cout << (cfg.has(k) ? cfg.get(k) : std::string("MISSING")) << std::endl;
            continue;
        }

        if (op != "LOG") continue;

        std::string a, b, c;
        std::cin >> a;
        if (!(std::cin >> b)) {
            if (parse_int_token(a)) log_line(std::cout, std::stoi(a));
            else log_line(std::cout, a);
            continue;
        }
        if (!(std::cin >> c)) {
            if (parse_int_token(b)) log_line(std::cout, a, std::stoi(b));
            else log_line(std::cout, a, b);
            std::cin.clear();
            continue;
        }
        if (parse_int_token(b)) log_line(std::cout, a, std::stoi(b), c);
        else log_line(std::cout, a, b, c);
    }
    return 0;
}
