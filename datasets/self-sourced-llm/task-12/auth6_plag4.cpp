#include <sstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <variant>
// quick submit

struct Number { int value; };
struct Add { std::size_t a, b; };
struct Sub { std::size_t a, b; };
struct Mul { std::size_t a, b; };
struct Div { std::size_t a, b; };

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

struct EvalCtx {
    const std::vector<Expr>* exprs;

    int eval(std::size_t i) const {
        struct V {
            const EvalCtx* ctx;
            int operator()(Number n) const { return n.value; }
            int operator()(Add x) const { return ctx->eval(x.a) + ctx->eval(x.b); }
            int operator()(Sub x) const { return ctx->eval(x.a) - ctx->eval(x.b); }
            int operator()(Mul x) const { return ctx->eval(x.a) * ctx->eval(x.b); }
            int operator()(Div x) const {
                int y = ctx->eval(x.b);
                if (y == 0) throw std::runtime_error("div0");
                return ctx->eval(x.a) / y;
            }
        };
        return std::visit(V{this}, (*exprs)[i]);
    }
};

void eval_line(const std::string& s) {
    std::vector<Expr> bag;
    std::vector<std::size_t> stack;
    std::istringstream in(s);
    std::string t;
    while (in >> t) {
        if (t.size() == 1 && std::string("+-*/").find(t[0]) != std::string::npos) {
            std::size_t y = stack.back(); stack.pop_back();
            std::size_t x = stack.back(); stack.pop_back();
            char c = t[0];
            if (c == '+') bag.push_back(Add{x, y});
            else if (c == '-') bag.push_back(Sub{x, y});
            else if (c == '*') bag.push_back(Mul{x, y});
            else bag.push_back(Div{x, y});
            stack.push_back(bag.size() - 1);
        } else {
            bag.push_back(Number{std::stoi(t)});
            stack.push_back(bag.size() - 1);
        }
    }
    EvalCtx ctx{&bag};
    try {
        std::cout << ctx.eval(stack.back()) << '\n';
    } catch (...) {
        std::cout << "ERROR\n";
    }
}

int main() {
    std::string ln;
    while (std::getline(std::cin, ln)) {
        if (ln.empty()) break;
        eval_line(ln);
    }
    return 0;
}
