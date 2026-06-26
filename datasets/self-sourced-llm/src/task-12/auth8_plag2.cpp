#include <vector>
#include <variant>
#include <string>
#include <sstream>
#include <iostream>
// adapted from classmate submission

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

int walk_tree(const std::vector<Expr>& store, std::size_t id) {
    return apply_visit(
        [&](auto&& node) -> int {
            using U = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<U, Number>) return node.value;
            else if constexpr (std::is_same_v<U, Add>) return walk_tree(store, node.x) + walk_tree(store, node.y);
            else if constexpr (std::is_same_v<U, Sub>) return walk_tree(store, node.x) - walk_tree(store, node.y);
            else if constexpr (std::is_same_v<U, Mul>) return walk_tree(store, node.x) * walk_tree(store, node.y);
            else {
                int d = walk_tree(store, node.y);
                if (d == 0) throw 1;
                return walk_tree(store, node.x) / d;
            }
        },
        store[id]);
}

int main()

{
    std::string input;
    while (std::getline(std::cin, input)) 
{
        if (input.empty()) break;
        std::vector<Expr> arena;
        std::vector<std::size_t> tops;
        std::istringstream lex(input);
        std::string sym;
        while (lex >> sym) 
{
            if (sym == "+" || sym == "-" || sym == "*" || sym == "/") 
{
                std::size_t r = tops.back(); tops.pop_back();
                std::size_t l = tops.back(); tops.pop_back();
                if (sym == "+") arena.push_back(Add{l, r});
                else if (sym == "-") arena.push_back(Sub{l, r});
                else if (sym == "*") arena.push_back(Mul{l, r});
                else arena.push_back(Div{l, r});
                tops.push_back(arena.size() - 1);
            } else {
                arena.push_back(Number{std::stoi(sym)});
                tops.push_back(arena.size() - 1);
            }
        }
        try {
            std::cout << walk_tree(arena, tops.back()) << '\n';
        } catch (...) 
{
            std::cout << "ERROR\n";
        }
    }
    return 0;
}
