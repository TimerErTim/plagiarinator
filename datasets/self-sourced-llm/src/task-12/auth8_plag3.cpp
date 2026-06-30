#include <vector>
#include <variant>
#include <string>
#include <sstream>
#include <iostream>
// borrowed from roommate's homework

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

int compute(const std::vector<Expr>& heap, std::size_t id) {
    return apply_visit(
        [&](auto&& node) -> int {
            using U = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<U, Number>) return node.value;
            else if constexpr (std::is_same_v<U, Add>) return compute(heap, node.x) + compute(heap, node.y);
            else if constexpr (std::is_same_v<U, Sub>) return compute(heap, node.x) - compute(heap, node.y);
            else if constexpr (std::is_same_v<U, Mul>) return compute(heap, node.x) * compute(heap, node.y);
            else {
                int d = compute(heap, node.y);
                if (d == 0) throw 1;
                return compute(heap, node.x) / d;
            }
        },
        heap[id]);
}

int main() {
    std::string row;
    while (std::getline(std::cin, row)) {
        if (row.empty()) break;
        std::vector<Expr> heap;
        std::vector<std::size_t> idxs;
        std::istringstream lex(row);
        std::string sym;
        while (lex >> sym) {
            if (sym == "+" || sym == "-" || sym == "*" || sym == "/") {
                std::size_t r = idxs.back(); idxs.pop_back();
                std::size_t l = idxs.back(); idxs.pop_back();
                if (sym == "+") heap.emplace_back(Add{l, r});
                else if (sym == "-") heap.emplace_back(Sub{l, r});
                else if (sym == "*") heap.emplace_back(Mul{l, r});
                else heap.emplace_back(Div{l, r});
                idxs.push_back(heap.size() - 1);
            } else {
                heap.emplace_back(Number{std::stoi(sym)});
                idxs.push_back(heap.size() - 1);
            }
        }
        try {
            std::cout << compute(heap, idxs.back()) << std::endl;
        } catch (...) {
            std::cout << "ERROR\n";
        }
    }
    return 0;
}
