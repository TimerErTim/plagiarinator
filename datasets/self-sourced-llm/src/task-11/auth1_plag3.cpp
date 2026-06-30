#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
// borrowed from roommate's homework

template <typename... Args>
void log_line(std::ostream& sink, Args&&... args) {
    ((sink << std::forward<Args>(args) << " "), ...);
    sink << std::endl;
}

class Config {
    std::unordered_map<std::string, std::string> props_;

public:
    void read(std::istream& src) {
        std::string row;
        while (std::getline(src, row) && !row.empty()) {
            auto eq = row.find('=');
            if (eq != std::string::npos)
                props_[row.substr(0, eq)] = row.substr(eq + 1);
        }
    }

    bool has(const std::string& k) const {
        return props_.find(k) != props_.end();
    }

    const std::string& get(const std::string& k) const {
        return props_.at(k);
    }
};

static bool intish(const std::string& s) {
    if (s.empty()) return false;
    std::size_t p = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    if (p >= s.size()) return false;
    for (std::size_t k = s.size(); k-- > p;)
        if (!std::isdigit(static_cast<unsigned char>(s[k]))) return false;
    return true;
}

struct TokVal {
    bool numeric;
    int ival;
    std::string str;
};

static TokVal classify(const std::string& t) {
    if (intish(t)) return TokVal{true, std::stoi(t), {}};
    return TokVal{false, 0, t};
}

static void flush_log(std::ostream& sink, const std::vector<TokVal>& vals) {
    if (vals.size() == 1) {
        if (vals[0].numeric) log_line(sink, vals[0].ival);
        else log_line(sink, vals[0].str);
    } else if (vals.size() == 2) {
        if (vals[0].numeric && vals[1].numeric) log_line(sink, vals[0].ival, vals[1].ival);
        else if (vals[0].numeric) log_line(sink, vals[0].ival, vals[1].str);
        else if (vals[1].numeric) log_line(sink, vals[0].str, vals[1].ival);
        else log_line(sink, vals[0].str, vals[1].str);
    } else if (vals.size() == 3) {
        const TokVal& x = vals[0], &y = vals[1], &z = vals[2];
        if (y.numeric) {
            if (x.numeric && z.numeric) log_line(sink, x.ival, y.ival, z.ival);
            else if (x.numeric) log_line(sink, x.ival, y.ival, z.str);
            else if (z.numeric) log_line(sink, x.str, y.ival, z.ival);
            else log_line(sink, x.str, y.ival, z.str);
        } else {
            log_line(sink, x.numeric ? std::to_string(x.ival) : x.str, y.str,
                     z.numeric ? std::to_string(z.ival) : z.str);
        }
    } else if (vals.empty()) {
        sink << std::endl;
    } else {
        for (std::size_t n = vals.size(); n-- > 0;) {
            if (n + 1 < vals.size()) sink << ' ';
            if (vals[n].numeric) sink << vals[n].ival;
            else sink << vals[n].str;
        }
        sink << std::endl;
    }
}

int main() {
    Config cfg;
    cfg.read(std::cin);

    std::string verb;
    while (std::cin >> verb && verb != "END") {
        if (verb == "GET") {
            std::string k;
            std::cin >> k;
            std::cout << (cfg.has(k) ? cfg.get(k) : std::string("MISSING")) << std::endl;
        } else if (verb == "LOG") {
            std::vector<std::string> bits;
            std::string piece;
            while (std::cin >> piece) {
                if (piece == "GET" || piece == "LOG" || piece == "END") {
                    for (std::size_t k = piece.size(); k-- > 0;)
                        std::cin.putback(piece[k]);
                    break;
                }
                bits.push_back(piece);
            }
            std::vector<TokVal> decoded;
            for (const auto& b : bits) decoded.push_back(classify(b));
            flush_log(std::cout, decoded);
        }
    }
    return 0;
}
