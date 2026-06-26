#include <cctype>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace logging {

inline void print_tail(std::ostream&) {}

template <typename Head, typename... Tail>
void print_tail(std::ostream& os, Head&& h, Tail&&... tail) {
    using H = std::decay_t<Head>;
    if constexpr (std::is_same_v<H, double>)
        os << std::fixed << std::setprecision(2) << h;
    else
        os << std::forward<Head>(h);
    if constexpr (sizeof...(tail) > 0) os << ' ';
    print_tail(os, std::forward<Tail>(tail)...);
}

}  // namespace logging

template <typename... Args>
void log_line(std::ostream& out, Args&&... args) {
    logging::print_tail(out, std::forward<Args>(args)...);
    out << '\n';
}

class Config {
public:
    void parse(std::istream& in) {
        std::string row;
        while (std::getline(in, row) && !row.empty()) {
            std::size_t sep = row.find('=');
            data_[row.substr(0, sep)] = row.substr(sep + 1);
        }
    }

    bool has(const std::string& key) const { return data_.count(key) != 0; }

    const std::string& get(const std::string& key) const { return data_.at(key); }

private:
    std::unordered_map<std::string, std::string> data_;
};

static bool is_int_literal(const std::string& s) {
    if (s.empty()) return false;
    std::size_t pos = 0;
    if (s[pos] == '-' || s[pos] == '+') ++pos;
    if (pos == s.size()) return false;
    for (; pos < s.size(); ++pos)
        if (!std::isdigit(static_cast<unsigned char>(s[pos]))) return false;
    return true;
}

static void handle_log_command(const std::vector<std::string>& words) {
    if (words.size() == 3) {
        if (is_int_literal(words[1]))
            log_line(std::cout, words[0], std::stoi(words[1]), words[2]);
        else
            log_line(std::cout, words[0], words[1], words[2]);
    } else if (words.size() == 2) {
        if (is_int_literal(words[1]))
            log_line(std::cout, words[0], std::stoi(words[1]));
        else
            log_line(std::cout, words[0], words[1]);
    } else if (words.size() == 1) {
        if (is_int_literal(words[0])) log_line(std::cout, std::stoi(words[0]));
        else log_line(std::cout, words[0]);
    } else if (words.empty()) {
        log_line(std::cout);
    }
}

int main() {
    Config cfg;
    cfg.parse(std::cin);

    std::string cmd;
    while (std::getline(std::cin, cmd)) {
        if (cmd == "END") break;
        if (cmd == "GET") {
            std::string key;
            std::getline(std::cin, key);
            key.erase(0, key.find_first_not_of(" \t"));
            std::cout << (cfg.has(key) ? cfg.get(key) : std::string("MISSING")) << '\n';
        } else if (cmd == "LOG") {
            std::string chunk;
            std::getline(std::cin, chunk);
            std::istringstream line(chunk);
            std::vector<std::string> words;
            for (std::string w; line >> w; words.push_back(w)) {
            }
            handle_log_command(words);
        }
    }
    return 0;
}
