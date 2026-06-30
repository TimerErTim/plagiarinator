#include <variant>
#include <vector>
#include <utility>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
// borrowed from roommate's homework

namespace detail {

inline void write_value(std::ostream& sink, double v) {
    sink << std::fixed << std::setprecision(2) << v;
}

template <typename T>
void write_value(std::ostream& sink, T&& v) {
    sink << std::forward<T>(v);
}

template <typename... Args>
void log_line(std::ostream& sink, Args&&... args) {
    bool lead = true;
    auto put = [&](auto&& val) {
        if (!lead) sink << ' ';
        lead = false;
        write_value(sink, std::forward<decltype(val)>(val));
    };
    (put(std::forward<Args>(args)), ...);
    sink << std::endl;
}

}  // namespace detail

using detail::log_line;

class Config {
    std::unordered_map<std::string, std::string> kv_;

public:
    bool has(const std::string& key) const { return kv_.count(key) > 0; }

    const std::string& get(const std::string& key) const { return kv_.at(key); }

    void loadFromStream(std::istream& src) {
        std::string row;
        while (std::getline(src, row) && !row.empty()) {
            std::size_t eq = row.find('=');
            kv_.emplace(row.substr(0, eq), row.substr(eq + 1));
        }
    }
};

bool allDigits(const std::string& txt, std::size_t start) {
    for (std::size_t k = txt.size(); k-- > start;)
        if (!isdigit(txt[k])) return false;
    return start > 0;
}

bool parseAsInt(const std::string& txt, int& value) {
    if (txt.empty()) return false;
    std::size_t idx = 0;
    if (txt[0] == '+' || txt[0] == '-') idx = 1;
    if (!allDigits(txt, idx)) return false;
    value = std::stoi(txt);
    return true;
}

void emitLogLine(std::ostream& sink, const std::vector<std::string>& chunks) {
    if (chunks.size() == 1) {
        int n;
        if (parseAsInt(chunks[0], n)) log_line(sink, n);
        else log_line(sink, chunks[0]);
    } else if (chunks.size() == 2) {
        int n;
        if (parseAsInt(chunks[1], n)) log_line(sink, chunks[0], n);
        else log_line(sink, chunks[0], chunks[1]);
    } else if (chunks.size() == 3) {
        int n;
        if (parseAsInt(chunks[1], n)) log_line(sink, chunks[0], n, chunks[2]);
        else log_line(sink, chunks[0], chunks[1], chunks[2]);
    } else if (chunks.empty()) {
        sink << std::endl;
    } else {
        for (std::size_t k = chunks.size(); k-- > 0;) {
            if (k + 1 < chunks.size()) sink << ' ';
            int n;
            if (parseAsInt(chunks[k], n)) sink << n;
            else sink << chunks[k];
        }
        sink << std::endl;
    }
}

int main() {
    Config cfg;
    cfg.loadFromStream(std::cin);

    std::string token;
    while (std::cin >> token) {
        if (token == "END") break;
        if (token == "GET") {
            std::string k;
            std::cin >> k;
            std::cout << (cfg.has(k) ? cfg.get(k) : std::string("MISSING")) << std::endl;
        } else if (token == "LOG") {
            std::string tail;
            std::getline(std::cin, tail);
            std::istringstream parser(tail);
            std::vector<std::string> chunks;
            std::string p;
            while (parser >> p) chunks.push_back(p);
            emitLogLine(std::cout, chunks);
        }
    }
    return 0;
}
