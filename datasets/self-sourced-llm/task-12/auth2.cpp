#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

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
    std::vector<Expr> stk;
    std::istringstream in(text);
    std::string piece;
    while (in >> piece) {
        if (is_op(piece)) {
            if (stk.size() < 2) return;
            Expr rhs = stk.back(); stk.pop_back();
            Expr lhs = stk.back(); stk.pop_back();
            int rv = evaluate(rhs);
            int lv = evaluate(lhs);
            char op = piece[0];
            if (op == '+') stk.emplace_back(Add{lv, rv});
            else if (op == '-') stk.emplace_back(Sub{lv, rv});
            else if (op == '*') stk.emplace_back(Mul{lv, rv});
            else stk.emplace_back(Div{lv, rv});
        } else {
            stk.emplace_back(Number{std::stoi(piece)});
        }
    }
    if (stk.empty()) return;
    try {
        std::cout << evaluate(stk.back()) << '\n';
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
