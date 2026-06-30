#include <vector>
#include <variant>
#include <functional>
#include <string>
#include <sstream>
#include <iostream>
// borrowed from roommate's homework

struct Number { int value; };
struct Add { std::size_t left, right; };
struct Sub { std::size_t left, right; };
struct Mul { std::size_t left, right; };
struct Div { std::size_t left, right; };

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

int walk_tree(const std::vector<Expr>& arena, std::size_t id) {
    std::function<int(std::size_t)> rec = [&](std::size_t i) {
        return std::visit(
            [&](auto&& node) -> int {
                using N = std::decay_t<decltype(node)>;
                if constexpr (std::is_same_v<N, Number>) return node.value;
                else if constexpr (std::is_same_v<N, Add>)
                    return rec(node.left) + rec(node.right);
                else if constexpr (std::is_same_v<N, Sub>)
                    return rec(node.left) - rec(node.right);
                else if constexpr (std::is_same_v<N, Mul>)
                    return rec(node.left) * rec(node.right);
                else {
                    int d = rec(node.right);
                    if (d == 0) throw 0;
                    return rec(node.left) / d;
                }
            },
            arena[i]);
    };
    return rec(id);
}

void handle(const std::string& input) {
    std::vector<Expr> arena;
    std::vector<std::size_t> tops;
    std::istringstream lex(input);
    std::string sym;
    while (lex >> sym) {
        if (sym == "+" || sym == "-" || sym == "*" || sym == "/") {
            std::size_t b = tops.back(); tops.pop_back();
            std::size_t a = tops.back(); tops.pop_back();
            if (sym == "+") arena.emplace_back(Add{a, b});
            else if (sym == "-") arena.emplace_back(Sub{a, b});
            else if (sym == "*") arena.emplace_back(Mul{a, b});
            else arena.emplace_back(Div{a, b});
            tops.push_back(arena.size() - 1);
        } else {
            arena.emplace_back(Number{std::stoi(sym)});
            tops.push_back(arena.size() - 1);
        }
    }
    try {
        std::cout << walk_tree(arena, tops.back()) << std::endl;
    } catch (...) {
        std::cout << "ERROR\n";
    }
}

int main() {
    std::string row;
    while (std::getline(std::cin, row) && !row.empty()) handle(row);
    return 0;
}
