#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <string>
// quick submit

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
    ConfigStore ref;
    {
        std::string str;
        while (std::getline(std::cin, str) && !str.empty()) {
            auto p = str.find('=');
            ref.values[str.substr(0, p)] = str.substr(p + 1);
        }
    }

    std::string elem;
    while (std::cin >> elem) {
        if (elem == "END") break;
        if (elem == "GET") {
            std::string tag;
            std::cin >> tag;
            log_line(std::cout, ref.has(tag) ? ref.get(tag) : std::string("MISSING"));
        } else if (elem == "LOG") {
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
