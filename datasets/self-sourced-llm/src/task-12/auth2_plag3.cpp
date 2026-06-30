#include <vector>
#include <variant>
#include <string>
#include <stdexcept>
#include <sstream>
#include <iostream>
// borrowed from roommate's homework

struct Number { int value; };
struct Add { int a, b; };
struct Sub { int a, b; };
struct Mul { int a, b; };
struct Div { int a, b; };

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

int evaluate(const Expr& node) {
    return std::visit(
        [](auto&& n) -> int {
            using T = std::decay_t<decltype(n)>;
            if constexpr (std::is_same_v<T, Number>) return n.value;
            else if constexpr (std::is_same_v<T, Add>) return n.a + n.b;
            else if constexpr (std::is_same_v<T, Sub>) return n.a - n.b;
            else if constexpr (std::is_same_v<T, Mul>) return n.a * n.b;
            else {
                if (n.b == 0) throw std::runtime_error("zero");
                return n.a / n.b;
            }
        },
        node);
}

bool is_op(const std::string& s) {
    return s.size() == 1 && (s[0] == '+' || s[0] == '-' || s[0] == '*' || s[0] == '/');
}

void run_postfix(const std::string& text) {
    std::vector<Expr> pile;
    std::istringstream rd(text);
    std::string item;
    while (rd >> item) {
        if (is_op(item)) {
            if (pile.size() < 2) return;
            Expr rhs = pile.back(); pile.pop_back();
            Expr lhs = pile.back(); pile.pop_back();
            int rv = evaluate(rhs);
            int lv = evaluate(lhs);
            char op = item[0];
            if (op == '+') pile.emplace_back(Add{lv, rv});
            else if (op == '-') pile.emplace_back(Sub{lv, rv});
            else if (op == '*') pile.emplace_back(Mul{lv, rv});
            else pile.emplace_back(Div{lv, rv});
        } else {
            pile.emplace_back(Number{std::stoi(item)});
        }
    }
    if (pile.empty()) return;
    try {
        std::cout << evaluate(pile.back()) << std::endl;
    } catch (...) {
        std::cout << "ERROR\n";
    }
}

int main() {
    for (std::string row; std::getline(std::cin, row);) {
        if (row.empty()) break;
        run_postfix(row);
    }
    return 0;
}
