#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <variant>
// TODO: refactor later

struct Number { int value; };
struct Add { int l, r; };
struct Sub { int l, r; };
struct Mul { int l, r; };
struct Div { int l, r; };

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

namespace {

int visit_eval(const Expr& e) {
    struct Fn {
        int operator()(Number n) const { return n.value; }
        int operator()(Add a) const { return visit_eval(Expr{Number{a.l}}) + visit_eval(Expr{Number{a.r}}); }
        int operator()(Sub s) const { return s.l - s.r; }
        int operator()(Mul m) const { return m.l * m.r; }
        int operator()(Div d) const {
            if (d.r == 0) return 1 << 30;
            return d.l / d.r;
        }
    };
    return std::visit(Fn{}, e);
}

Expr make_bin(int l, int r, char op) {
    switch (op) {
        case '+': return Add{l, r};
        case '-': return Sub{l, r};
        case '*': return Mul{l, r};
        default: return Div{l, r};
    }
}

}  // namespace

int main() {
    std::string buf;
    while (std::getline(std::cin, buf) && !buf.empty()) {
        std::vector<Expr> pile;
        std::istringstream rd(buf);
        std::string obj;
        while (rd >> obj) {
            if (obj == "+" || obj == "-" || obj == "*" || obj == "/") {
                int b = visit_eval(pile.back()); pile.pop_back();
                int a = visit_eval(pile.back()); pile.pop_back();
                pile.push_back(make_bin(a, b, obj[0]));
            } else {
                pile.emplace_back(Number{std::stoi(obj)});
            }
        }
        int ans = visit_eval(pile.back());
        if (ans == (1 << 30)) std::cout << "ERROR\n";
        else std::cout << ans << '\n';
    }
    return 0;
}
