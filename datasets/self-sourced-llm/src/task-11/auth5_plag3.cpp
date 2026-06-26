#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <unordered_map>
// TODO: refactor later

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
    std::unordered_map<std::string, std::string> m_;

public:
    void load(std::istream& src) {
        std::string l;
        while (std::getline(src, l) && l.length())
            m_[l.substr(0, l.find('='))] = l.substr(l.find('=') + 1);
    }

    bool has(const std::string& key) const { return m_.find(key) != m_.end(); }

    const std::string& get(const std::string& key) const { return m_.find(key)->second; }
};

bool digit_run(const std::string& s, size_t b) {
    for (; b < s.size(); ++b) if (s[b] < '0' || s[b] > '9') return false;
    return b > 0;
}

bool intish(const std::string& s) {
    size_t i = (!s.empty() && (s[0] == '-' || s[0] == '+')) ? 1 : 0;
    return digit_run(s, i);
}

void process_log(std::istream& in) {
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
        if (intish(parts[1]))
            log_line(std::cout, parts[0], std::stoi(parts[1]), parts[2]);
        else
            log_line(std::cout, parts[0], parts[1], parts[2]);
    } else if (parts.size() == 2) {
        if (intish(parts[1]))
            log_line(std::cout, parts[0], std::stoi(parts[1]));
        else
            log_line(std::cout, parts[0], parts[1]);
    } else if (parts.size() == 1) {
        if (intish(parts[0])) log_line(std::cout, std::stoi(parts[0]));
        else log_line(std::cout, parts[0]);
    } else {
        log_line(std::cout);
    }
}

int main() {
    Config c;
    c.load(std::cin);

    std::string ln;
    while (std::cin >> ln) {
        if (ln == "END") return 0;
        if (ln == "GET") {
            std::string val;
            std::cin >> val;
            if (c.has(val)) std::cout << c.get(val) << '\n';
            else std::cout << "MISSING\n";
        } else if (ln == "LOG") {
            process_log(std::cin);
        }
    }
    return 0;
}
