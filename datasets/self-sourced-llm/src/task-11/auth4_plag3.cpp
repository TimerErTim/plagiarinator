#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cctype>
// borrowed from roommate's homework

namespace logging {

inline void print_tail(std::ostream&) {}

template <typename Head, typename... Tail>
void print_tail(std::ostream& sink, Head&& h, Tail&&... tail) {
    using H = std::decay_t<Head>;
    if constexpr (std::is_same_v<H, double>)
        sink << std::fixed << std::setprecision(2) << h;
    else
        sink << std::forward<Head>(h);
    if constexpr (sizeof...(tail) > 0) sink << ' ';
    print_tail(sink, std::forward<Tail>(tail)...);
}

}  // namespace logging

template <typename... Args>
void log_line(std::ostream& sink, Args&&... args) {
    logging::print_tail(sink, std::forward<Args>(args)...);
    sink << std::endl;
}

class Config {
public:
    void parse(std::istream& src) {
        std::string row;
        while (std::getline(src, row) && !row.empty()) {
            std::size_t sep = row.find('=');
            props_[row.substr(0, sep)] = row.substr(sep + 1);
        }
    }

    bool has(const std::string& k) const { return props_.count(k) != 0; }

    const std::string& get(const std::string& k) const { return props_.at(k); }

private:
    std::unordered_map<std::string, std::string> props_;
};

static bool intish(const std::string& s) {
    if (s.empty()) return false;
    std::size_t pos = 0;
    if (s[pos] == '-' || s[pos] == '+') ++pos;
    if (pos == s.size()) return false;
    for (std::size_t k = s.size(); k-- > pos;)
        if (!std::isdigit(static_cast<unsigned char>(s[k]))) return false;
    return true;
}

static void handle_log_command(const std::vector<std::string>& words) {
    if (words.size() == 3) {
        if (intish(words[1]))
            log_line(std::cout, words[0], std::stoi(words[1]), words[2]);
        else
            log_line(std::cout, words[0], words[1], words[2]);
    } else if (words.size() == 2) {
        if (intish(words[1]))
            log_line(std::cout, words[0], std::stoi(words[1]));
        else
            log_line(std::cout, words[0], words[1]);
    } else if (words.size() == 1) {
        if (intish(words[0])) log_line(std::cout, std::stoi(words[0]));
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
            std::cout << (cfg.has(key) ? cfg.get(key) : std::string("MISSING")) << std::endl;
        } else if (cmd == "LOG") {
            std::string chunk;
            std::getline(std::cin, chunk);
            std::istringstream line(chunk);
            std::vector<std::string> words;
            for (std::string w; line >> w; words.push_back(w)) {}
            handle_log_command(words);
        }
    }
    return 0;
}
