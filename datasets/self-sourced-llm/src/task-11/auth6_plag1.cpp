#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
// cleaned up version

struct ConfigStore {
    std::unordered_map<std::string, std::string> values;

    bool has(const std::string& key) const {
        return values.find(key) != values.end();
    }

    const std::string& get(const std::string& key) const {
        return values.at(key);
    }
};

void print_one(std::ostream& out, double v) {
    out << std::fixed << std::setprecision(2) << v;
}

template <typename T>
void print_one(std::ostream& out, T&& v) {
    out << std::forward<T>(v);
}

struct LogWriter {
    std::ostream& os;
    bool sep = false;

    template <typename U>
    void step(U&& u) {
        if (sep) os << ' ';
        else sep = true;
        print_one(os, std::forward<U>(u));
    }
};

template <typename... Args>
void log_line(std::ostream& out, Args&&... args) {
    LogWriter w{out};
    (w.step(std::forward<Args>(args)), ...);
    out << '\n';
}

bool is_numeric(const std::string& s) {
    if (s.empty()) return false;
    std::size_t k = 0;
    if (s[0] == '-' || s[0] == '+') k = 1;
    if (k >= s.size()) return false;
    for (; k < s.size(); ++k)
        if (s[k] < '0' || s[k] > '9') return false;
    return true;
}

int main() {
    ConfigStore conf;
    {
        std::string s;
        while (std::getline(std::cin, s) && !s.empty()) {
            auto p = s.find('=');
            conf.values[s.substr(0, p)] = s.substr(p + 1);
        }
    }

    std::string piece;
    while (std::cin >> piece) {
        if (piece == "END") break;
        if (piece == "GET") {
            std::string id;
            std::cin >> id;
            log_line(std::cout, conf.has(id) ? conf.get(id) : std::string("MISSING"));
        } else if (piece == "LOG") {
            std::string t1, t2, t3;
            if (!(std::cin >> t1)) continue;
            if (std::cin >> t2) {
                if (std::cin >> t3) {
                    if (is_numeric(t2))
                        log_line(std::cout, t1, std::stoi(t2), t3);
                    else
                        log_line(std::cout, t1, t2, t3);
                } else {
                    if (is_numeric(t2))
                        log_line(std::cout, t1, std::stoi(t2));
                    else
                        log_line(std::cout, t1, t2);
                    std::cin.clear();
                }
            } else {
                if (is_numeric(t1)) log_line(std::cout, std::stoi(t1));
                else log_line(std::cout, t1);
            }
        }
    }
    return 0;
}
