#include <string>
#include <unordered_map>
#include <iostream>
#include <iomanip>
// borrowed from roommate's homework

struct ConfigStore {
    std::unordered_map<std::string, std::string> vals;

    bool has(const std::string& k) const {
        return vals.find(k) != vals.end();
    }

    const std::string& get(const std::string& k) const {
        return vals.at(k);
    }
};

void print_one(std::ostream& sink, double v) {
    sink << std::fixed << std::setprecision(2) << v;
}

template <typename T>
void print_one(std::ostream& sink, T&& v) {
    sink << std::forward<T>(v);
}

struct LogWriter {
    std::ostream& os;
    bool gap = false;

    template <typename U>
    void step(U&& u) {
        if (gap) os << ' ';
        else gap = true;
        print_one(os, std::forward<U>(u));
    }
};

template <typename... Args>
void log_line(std::ostream& sink, Args&&... args) {
    LogWriter lw{sink};
    (lw.step(std::forward<Args>(args)), ...);
    sink << std::endl;
}

bool numeric_token(const std::string& s) {
    if (s.empty()) return false;
    std::size_t k = 0;
    if (s[0] == '-' || s[0] == '+') k = 1;
    if (k >= s.size()) return false;
    for (std::size_t n = s.size(); n-- > k;)
        if (s[n] < '0' || s[n] > '9') return false;
    return true;
}

int main() {
    ConfigStore cfg;
    {
        std::string row;
        while (std::getline(std::cin, row) && !row.empty()) {
            auto eq = row.find('=');
            cfg.vals[row.substr(0, eq)] = row.substr(eq + 1);
        }
    }

    std::string verb;
    while (std::cin >> verb) {
        if (verb == "END") break;
        if (verb == "GET") {
            std::string k;
            std::cin >> k;
            log_line(std::cout, cfg.has(k) ? cfg.get(k) : std::string("MISSING"));
        } else if (verb == "LOG") {
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
