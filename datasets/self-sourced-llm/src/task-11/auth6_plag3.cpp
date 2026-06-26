#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <string>
// TODO: refactor later

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

bool numeric_token(const std::string& s) {
    if (s.empty()) return false;
    std::size_t k = 0;
    if (s[0] == '-' || s[0] == '+') k = 1;
    if (k >= s.size()) return false;
    for (; k < s.size(); ++k)
        if (s[k] < '0' || s[k] > '9') return false;
    return true;
}

int main() {
    ConfigStore box;
    {
        std::string buf;
        while (std::getline(std::cin, buf) && !buf.empty()) {
            auto p = buf.find('=');
            box.values[buf.substr(0, p)] = buf.substr(p + 1);
        }
    }

    std::string ln;
    while (std::cin >> ln) {
        if (ln == "END") break;
        if (ln == "GET") {
            std::string val;
            std::cin >> val;
            log_line(std::cout, box.has(val) ? box.get(val) : std::string("MISSING"));
        } else if (ln == "LOG") {
            std::string t1, t2, t3;
            if (!(std::cin >> t1)) continue;
            if (std::cin >> t2) {
                if (std::cin >> t3) {
                    if (numeric_token(t2))
                        log_line(std::cout, t1, std::stoi(t2), t3);
                    else
                        log_line(std::cout, t1, t2, t3);
                } else {
                    if (numeric_token(t2))
                        log_line(std::cout, t1, std::stoi(t2));
                    else
                        log_line(std::cout, t1, t2);
                    std::cin.clear();
                }
            } else {
                if (numeric_token(t1)) log_line(std::cout, std::stoi(t1));
                else log_line(std::cout, t1);
            }
        }
    }
    return 0;
}
