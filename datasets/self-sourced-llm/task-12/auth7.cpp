#include <iostream>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

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
    std::string text;
    while (std::getline(std::cin, text) && !text.empty()) {
        std::vector<Expr> st;
        std::istringstream rd(text);
        std::string item;
        while (rd >> item) {
            if (item == "+" || item == "-" || item == "*" || item == "/") {
                int b = visit_eval(st.back()); st.pop_back();
                int a = visit_eval(st.back()); st.pop_back();
                st.push_back(make_bin(a, b, item[0]));
            } else {
                st.emplace_back(Number{std::stoi(item)});
            }
        }
        int ans = visit_eval(st.back());
        if (ans == (1 << 30)) std::cout << "ERROR\n";
        else std::cout << ans << '\n';
    }
    return 0;
}
