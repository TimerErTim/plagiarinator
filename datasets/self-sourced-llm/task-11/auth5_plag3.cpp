#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <iomanip>
// borrowed from roommate's homework

template<typename T>
struct LogFmt {
    static void write(std::ostream& sink, T v) { sink << v; }
};

template<>
struct LogFmt<double> {
    static void write(std::ostream& sink, double v) {
        sink << std::fixed << std::setprecision(2) << v;
    }
};

template<typename... Ts>
void log_line(std::ostream& sink, Ts&&... xs) {
    const char* gap = "";
    ([&] {
        sink << gap;
        LogFmt<std::decay_t<Ts>>::write(sink, std::forward<Ts>(xs));
        gap = " ";
    }(), ...);
    sink << std::endl;
}

class Config {
    std::unordered_map<std::string, std::string> props_;

public:
    void load(std::istream& src) {
        std::string row;
        while (std::getline(src, row) && row.length())
            props_[row.substr(0, row.find('='))] = row.substr(row.find('=') + 1);
    }

    bool has(const std::string& k) const { return props_.find(k) != props_.end(); }

    const std::string& get(const std::string& k) const { return props_.find(k)->second; }
};

bool digit_run(const std::string& s, size_t b) {
    for (std::size_t k = s.size(); k-- > b;)
        if (s[k] < '0' || s[k] > '9') return false;
    return b > 0;
}

bool intish(const std::string& s) {
    size_t i = (!s.empty() && (s[0] == '-' || s[0] == '+')) ? 1 : 0;
    return digit_run(s, i);
}

void process_log(std::istream& src) {
    std::vector<std::string> chunks;
    std::string piece;
    while (src >> piece) {
        if (piece == "GET" || piece == "LOG" || piece == "END") {
            for (std::size_t k = piece.size(); k-- > 0;)
                src.putback(piece[k]);
            break;
        }
        chunks.push_back(piece);
    }
    if (chunks.size() == 3) {
        if (intish(chunks[1]))
            log_line(std::cout, chunks[0], std::stoi(chunks[1]), chunks[2]);
        else
            log_line(std::cout, chunks[0], chunks[1], chunks[2]);
    } else if (chunks.size() == 2) {
        if (intish(chunks[1]))
            log_line(std::cout, chunks[0], std::stoi(chunks[1]));
        else
            log_line(std::cout, chunks[0], chunks[1]);
    } else if (chunks.size() == 1) {
        if (intish(chunks[0])) log_line(std::cout, std::stoi(chunks[0]));
        else log_line(std::cout, chunks[0]);
    } else {
        log_line(std::cout);
    }
}

int main() {
    Config cfg;
    cfg.load(std::cin);

    std::string verb;
    while (std::cin >> verb) {
        if (verb == "END") return 0;
        if (verb == "GET") {
            std::string k;
            std::cin >> k;
            if (cfg.has(k)) std::cout << cfg.get(k) << std::endl;
            else std::cout << "MISSING\n";
        } else if (verb == "LOG") {
            process_log(std::cin);
        }
    }
    return 0;
}
