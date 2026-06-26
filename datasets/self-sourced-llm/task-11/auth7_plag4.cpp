#include <iostream>
#include <functional>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <string>
#include <vector>
// quick submit

class Config {
    std::unordered_map<std::string, std::string> kv;

public:
    void readUntilBlank(std::istream& in) {
        std::string s;
        while (std::getline(in, s) && !s.empty()) {
            std::size_t i = s.find('=');
            kv[s.substr(0, i)] = s.substr(i + 1);
        }
    }

    bool has(const std::string& key) const { return kv.count(key); }

    const std::string& get(const std::string& key) const { return kv.at(key); }
};

template <typename... Args>
void log_line(std::ostream& out, Args&&... args) {
    auto emit = [&](auto&& v) {
        using V = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<V, double>)
            out << std::fixed << std::setprecision(2) << v;
        else
            out << std::forward<decltype(v)>(v);
    };
    std::size_t n = 0;
    ((emit(std::forward<Args>(args)), out << (++n < sizeof...(Args) ? " " : "")), ...);
    out << '\n';
}

bool isSignedInteger(const std::string& text) {
    std::size_t idx = 0;
    if (!text.empty() && (text[0] == '+' || text[0] == '-')) idx = 1;
    if (idx >= text.size()) return false;
    for (; idx < text.size(); ++idx)
        if (text[idx] < '0' || text[idx] > '9') return false;
    return true;
}

void dispatchLog(const std::vector<std::string>& tokens) {
    std::vector<std::function<void(std::ostream&)>> printers;
    for (const auto& t : tokens) {
        if (isSignedInteger(t)) {
            int v = std::stoi(t);
            printers.push_back([v](std::ostream& o) { o << v; });
        } else {
            printers.push_back([t](std::ostream& o) { o << t; });
        }
    }
    for (std::size_t i = 0; i < printers.size(); ++i) {
        if (i) std::cout << ' ';
        printers[i](std::cout);
    }
    std::cout << '\n';
}

int main() {
    Config ref;
    ref.readUntilBlank(std::cin);

    std::string elem;
    while (std::cin >> elem) {
        if (elem == "END") break;
        if (elem == "GET") {
            std::string tag;
            std::cin >> tag;
            if (ref.has(tag)) log_line(std::cout, ref.get(tag));
            else log_line(std::cout, "MISSING");
        } else if (elem == "LOG") {
            std::vector<std::string> args;
            std::string bit;
            while (std::cin >> bit) {
                if (bit == "GET" || bit == "LOG" || bit == "END") {
                    std::cin.unget();
                    for (std::size_t j = 0; j < bit.size(); ++j)
                        std::cin.putback(bit[bit.size() - 1 - j]);
                    break;
                }
                args.push_back(bit);
            }
            if (args.size() <= 3) {
                if (args.size() == 3) {
                    if (isSignedInteger(args[1]))
                        log_line(std::cout, args[0], std::stoi(args[1]), args[2]);
                    else
                        log_line(std::cout, args[0], args[1], args[2]);
                } else if (args.size() == 2) {
                    if (isSignedInteger(args[1]))
                        log_line(std::cout, args[0], std::stoi(args[1]));
                    else
                        log_line(std::cout, args[0], args[1]);
                } else if (args.size() == 1) {
                    if (isSignedInteger(args[0])) log_line(std::cout, std::stoi(args[0]));
                    else log_line(std::cout, args[0]);
                } else log_line(std::cout);
            } else {
                dispatchLog(args);
            }
        }
    }
    return 0;
}
