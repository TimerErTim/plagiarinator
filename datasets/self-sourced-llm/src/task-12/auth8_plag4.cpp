#include <iostream>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

struct Number { int value; };
struct Add { std::size_t x, y; };
struct Sub { std::size_t x, y; };
struct Mul { std::size_t x, y; };
struct Div { std::size_t x, y; };

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

template <typename Visitor, typename Variant>
auto apply_visit(Visitor&& vis, Variant&& v) {
    return std::visit(std::forward<Visitor>(vis), std::forward<Variant>(v));
}

int resolve(const std::vector<Expr>& store, std::size_t id) {
    return apply_visit(
        [&](auto&& node) -> int {
            using U = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<U, Number>) return node.value;
            else if constexpr (std::is_same_v<U, Add>) return resolve(store, node.x) + resolve(store, node.y);
            else if constexpr (std::is_same_v<U, Sub>) return resolve(store, node.x) - resolve(store, node.y);
            else if constexpr (std::is_same_v<U, Mul>) return resolve(store, node.x) * resolve(store, node.y);
            else {
                int d = resolve(store, node.y);
                if (d == 0) throw 1;
                return resolve(store, node.x) / d;
            }
        },
        store[id]);
}

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) break;
        std::vector<Expr> arena;
        std::vector<std::size_t> tops;
        std::istringstream lex(line);
        std::string tok;
        while (lex >> tok) {
            if (tok == "+" || tok == "-" || tok == "*" || tok == "/") {
                std::size_t r = tops.back(); tops.pop_back();
                std::size_t l = tops.back(); tops.pop_back();
                if (tok == "+") arena.emplace_back(Add{l, r});
                else if (tok == "-") arena.emplace_back(Sub{l, r});
                else if (tok == "*") arena.emplace_back(Mul{l, r});
                else arena.emplace_back(Div{l, r});
                tops.push_back(arena.size() - 1);
            } else {
                arena.emplace_back(Number{std::stoi(tok)});
                tops.push_back(arena.size() - 1);
            }
        }
        try {
            std::cout << resolve(arena, tops.back()) << '\n';
        } catch (...) {
            std::cout << "ERROR\n";
        }
    }
    return 0;
}
