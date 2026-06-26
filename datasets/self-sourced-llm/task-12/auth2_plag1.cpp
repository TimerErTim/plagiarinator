#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>
// cleaned up version

struct Number { int value; };
struct Add { int lhs, rhs; };
struct Sub { int lhs, rhs; };
struct Mul { int lhs, rhs; };
struct Div { int lhs, rhs; };

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

int eval_value(const Expr& node) {
    return std::visit(
        [](auto&& n) -> int {
            using T = std::decay_t<decltype(n)>;
            if constexpr (std::is_same_v<T, Number>) return n.value;
            else if constexpr (std::is_same_v<T, Add>) return n.lhs + n.rhs;
            else if constexpr (std::is_same_v<T, Sub>) return n.lhs - n.rhs;
            else if constexpr (std::is_same_v<T, Mul>) return n.lhs * n.rhs;
            else {
                if (n.rhs == 0) throw std::runtime_error("zero");
                return n.lhs / n.rhs;
            }
        },
        node);
}

bool check_op(const std::string& s) {
    return s.size() == 1 && (s[0] == '+' || s[0] == '-' || s[0] == '*' || s[0] == '/');
}

void eval_postfix(const std::string& text) {
    std::vector<Expr> stk;
    std::istringstream in(text);
    std::string piece;
    while (in >> piece) {
        if (check_op(piece)) {
            if (stk.size() < 2) return;
            Expr rhs = stk.back(); stk.pop_back();
            Expr lhs = stk.back(); stk.pop_back();
            int rv = eval_value(rhs);
            int lv = eval_value(lhs);
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
        std::cout << eval_value(stk.back()) << '\n';
    } catch (...) {
        std::cout << "ERROR\n";
    }
}

int main() {
    for (std::string s; std::getline(std::cin, s);) {
        if (s.empty()) break;
        eval_postfix(s);
    }
    return 0;
}
