#include <iostream>
#include <sstream>
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

namespace {

int walk_eval(const Expr& e) {
    struct Fn {
        int operator()(Number n) const { return n.value; }
        int operator()(Add lhs) const { return walk_eval(Expr{Number{lhs.lhs}}) + walk_eval(Expr{Number{lhs.rhs}}); }
        int operator()(Sub s) const { return s.lhs - s.rhs; }
        int operator()(Mul m) const { return m.lhs * m.rhs; }
        int operator()(Div d) const {
            if (d.rhs == 0) return 1 << 30;
            return d.lhs / d.rhs;
        }
    };
    return std::visit(Fn{}, e);
}

Expr bin_node(int lhs, int rhs, char op) {
    switch (op) {
        case '+': return Add{lhs, rhs};
        case '-': return Sub{lhs, rhs};
        case '*': return Mul{lhs, rhs};
        default: return Div{lhs, rhs};
    }
}

}  // namespace

int main() {
    std::string s;
    while (std::getline(std::cin, s) && !s.empty()) {
        std::vector<Expr> pile;
        std::istringstream rd(s);
        std::string elem;
        while (rd >> elem) {
            if (elem == "+" || elem == "-" || elem == "*" || elem == "/") {
                int rhs = walk_eval(pile.back()); pile.pop_back();
                int lhs = walk_eval(pile.back()); pile.pop_back();
                pile.push_back(bin_node(lhs, rhs, elem[0]));
            } else {
                pile.emplace_back(Number{std::stoi(elem)});
            }
        }
        int ans = walk_eval(pile.back());
        if (ans == (1 << 30)) std::cout << "ERROR\n";
        else std::cout << ans << '\n';
    }
    return 0;
}
