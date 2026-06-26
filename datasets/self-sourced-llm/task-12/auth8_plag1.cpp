#include <iostream>
#include <sstream>
#include <string>
#include <variant>
#include <vector>
// cleaned up version

struct Number { int value; };
struct Add { std::size_t lhs, rhs; };
struct Sub { std::size_t lhs, rhs; };
struct Mul { std::size_t lhs, rhs; };
struct Div { std::size_t lhs, rhs; };

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

template <typename Visitor, typename Variant>
auto apply_visit(Visitor&& vis, Variant&& v) {
    return std::visit(std::forward<Visitor>(vis), std::forward<Variant>(v));
}

int eval_tree(const std::vector<Expr>& store, std::size_t id) {
    return apply_visit(
        [&](auto&& node) -> int {
            using U = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<U, Number>) return node.value;
            else if constexpr (std::is_same_v<U, Add>) return eval_tree(store, node.lhs) + eval_tree(store, node.rhs);
            else if constexpr (std::is_same_v<U, Sub>) return eval_tree(store, node.lhs) - eval_tree(store, node.rhs);
            else if constexpr (std::is_same_v<U, Mul>) return eval_tree(store, node.lhs) * eval_tree(store, node.rhs);
            else {
                int d = eval_tree(store, node.rhs);
                if (d == 0) throw 1;
                return eval_tree(store, node.lhs) / d;
            }
        },
        store[id]);
}

int main() {
    std::string s;
    while (std::getline(std::cin, s)) {
        if (s.empty()) break;
        std::vector<Expr> arena;
        std::vector<std::size_t> tops;
        std::istringstream lex(s);
        std::string sym;
        while (lex >> sym) {
            if (sym == "+" || sym == "-" || sym == "*" || sym == "/") {
                std::size_t rhs = tops.back(); tops.pop_back();
                std::size_t lhs = tops.back(); tops.pop_back();
                if (sym == "+") arena.emplace_back(Add{lhs, rhs});
                else if (sym == "-") arena.emplace_back(Sub{lhs, rhs});
                else if (sym == "*") arena.emplace_back(Mul{lhs, rhs});
                else arena.emplace_back(Div{lhs, rhs});
                tops.push_back(arena.size() - 1);
            } else {
                arena.emplace_back(Number{std::stoi(sym)});
                tops.push_back(arena.size() - 1);
            }
        }
        try {
            std::cout << eval_tree(arena, tops.back()) << '\n';
        } catch (...) {
            std::cout << "ERROR\n";
        }
    }
    return 0;
}
