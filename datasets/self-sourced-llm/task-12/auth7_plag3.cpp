#include <vector>
#include <variant>
#include <string>
#include <sstream>
#include <iostream>
// borrowed from roommate's homework

struct Number { int value; };
struct Add { int l, r; };
struct Sub { int l, r; };
struct Mul { int l, r; };
struct Div { int l, r; };

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

namespace {

int run_expr(const Expr& e) {
    struct Fn {
        int operator()(Number n) const { return n.value; }
        int operator()(Add a) const { return run_expr(Expr{Number{a.l}}) + run_expr(Expr{Number{a.r}}); }
        int operator()(Sub s) const { return s.l - s.r; }
        int operator()(Mul m) const { return m.l * m.r; }
        int operator()(Div d) const {
            if (d.r == 0) return 1 << 30;
            return d.l / d.r;
        }
    };
    return std::visit(Fn{}, e);
}

Expr mkOp(int l, int r, char op) {
    switch (op) {
        case '+': return Add{l, r};
        case '-': return Sub{l, r};
        case '*': return Mul{l, r};
        default: return Div{l, r};
    }
}

}  // namespace

int main() {
    std::string row;
    while (std::getline(std::cin, row) && !row.empty()) {
        std::vector<Expr> work;
        std::istringstream lex(row);
        std::string sym;
        while (lex >> sym) {
            if (sym == "+" || sym == "-" || sym == "*" || sym == "/") {
                int b = run_expr(work.back()); work.pop_back();
                int a = run_expr(work.back()); work.pop_back();
                work.push_back(mkOp(a, b, sym[0]));
            } else {
                work.emplace_back(Number{std::stoi(sym)});
            }
        }
        int ans = run_expr(work.back());
        if (ans == (1 << 30)) std::cout << "ERROR" << std::endl;
        else std::cout << ans << std::endl;
    }
    return 0;
}
