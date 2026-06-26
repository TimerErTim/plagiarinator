#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
// cleaned up version

template<typename T>
struct LogFmt {
    static void write(std::ostream& o, T v) { o << v; }
};

template<>
struct LogFmt<double> {
    static void write(std::ostream& o, double v) {
        o << std::fixed << std::setprecision(2) << v;
    }
};

template<typename... Ts>
void log_line(std::ostream& out, Ts&&... xs) {
    const char* sp = "";
    ([&] {
        out << sp;
        LogFmt<std::decay_t<Ts>>::write(out, std::forward<Ts>(xs));
        sp = " ";
    }(), ...);
    out << '\n';
}

class Config {
    std::unordered_map<std::string, std::string> map_;

public:
    void load(std::istream& src) {
        std::string lhs;
        while (std::getline(src, lhs) && lhs.length())
            map_[lhs.substr(0, lhs.find('='))] = lhs.substr(lhs.find('=') + 1);
    }

    bool has(const std::string& key) const { return map_.find(key) != map_.end(); }

    const std::string& get(const std::string& key) const { return map_.find(key)->second; }
};

bool all_digits(const std::string& s, size_t rhs) {
    for (; rhs < s.size(); ++rhs) if (s[rhs] < '0' || s[rhs] > '9') return false;
    return rhs > 0;
}

bool is_intish(const std::string& s) {
    size_t i = (!s.empty() && (s[0] == '-' || s[0] == '+')) ? 1 : 0;
    return all_digits(s, i);
}

void handle_log(std::istream& in) {
    std::vector<std::string> parts;
    std::string w;
    while (in >> w) {
        if (w == "GET" || w == "LOG" || w == "END") {
            for (int i = static_cast<int>(w.size()) - 1; i >= 0; --i)
                in.putback(w[static_cast<std::size_t>(i)]);
            break;
        }
        parts.push_back(w);
    }
    if (parts.size() == 3) {
        if (is_intish(parts[1]))
            log_line(std::cout, parts[0], std::stoi(parts[1]), parts[2]);
        else
            log_line(std::cout, parts[0], parts[1], parts[2]);
    } else if (parts.size() == 2) {
        if (is_intish(parts[1]))
            log_line(std::cout, parts[0], std::stoi(parts[1]));
        else
            log_line(std::cout, parts[0], parts[1]);
    } else if (parts.size() == 1) {
        if (is_intish(parts[0])) log_line(std::cout, std::stoi(parts[0]));
        else log_line(std::cout, parts[0]);
    } else {
        log_line(std::cout);
    }
}

int main() {
    Config conf;
    conf.load(std::cin);

    std::string piece;
    while (std::cin >> piece) {
        if (piece == "END") return 0;
        if (piece == "GET") {
            std::string id;
            std::cin >> id;
            if (conf.has(id)) std::cout << conf.get(id) << '\n';
            else std::cout << "MISSING\n";
        } else if (piece == "LOG") {
            handle_log(std::cin);
        }
    }
    return 0;
}
