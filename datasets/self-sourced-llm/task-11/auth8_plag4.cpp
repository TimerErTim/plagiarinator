#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <string>
// quick submit

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
    std::unordered_map<std::string, std::string> data_;

public:
    Config(std::istream& in) {
        std::string row;
        while (std::getline(in, row) && row.size() > 0) {
            std::size_t eq = row.find('=');
            data_.insert({row.substr(0, eq), row.substr(eq + 1)});
        }
    }

    bool has(const std::string& key) const { return data_.count(key) > 0; }

    const std::string& get(const std::string& key) const { return data_.at(key); }
};

static bool parse_int_token(const std::string& tok) {
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
    Config ref(std::cin);
    std::string word;

    while (std::cin >> word) {
        if (word == "END") break;

        if (word == "GET") {
            std::string tag;
            std::cin >> tag;
            std::cout << (ref.has(tag) ? ref.get(tag) : std::string("MISSING")) << '\n';
            continue;
        }

        if (word != "LOG") continue;

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
