#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>
// cleaned up version

struct Number { int value; };
struct Add { std::size_t lhs, rhs; };
struct Sub { std::size_t lhs, rhs; };
struct Mul { std::size_t lhs, rhs; };
struct Div { std::size_t lhs, rhs; };

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

struct EvalCtx {
    const std::vector<Expr>* exprs;

    int eval(std::size_t i) const {
        struct V {
            const EvalCtx* ctx;
            int operator()(Number n) const { return n.value; }
            int operator()(Add lhs) const { return ctx->eval(lhs.lhs) + ctx->eval(lhs.rhs); }
            int operator()(Sub lhs) const { return ctx->eval(lhs.lhs) - ctx->eval(lhs.rhs); }
            int operator()(Mul lhs) const { return ctx->eval(lhs.lhs) * ctx->eval(lhs.rhs); }
            int operator()(Div lhs) const {
                int rhs = ctx->eval(lhs.rhs);
                if (rhs == 0) throw std::runtime_error("div0");
                return ctx->eval(lhs.lhs) / rhs;
            }
        };
        return std::visit(V{this}, (*exprs)[i]);
    }
};

void parse_eval(const std::string& s) {
    std::vector<Expr> bag;
    std::vector<std::size_t> stack;
    std::istringstream in(s);
    std::string t;
    while (in >> t) {
        if (t.size() == 1 && std::string("+-*/").find(t[0]) != std::string::npos) {
            std::size_t rhs = stack.back(); stack.pop_back();
            std::size_t lhs = stack.back(); stack.pop_back();
            char c = t[0];
            if (c == '+') bag.push_back(Add{lhs, rhs});
            else if (c == '-') bag.push_back(Sub{lhs, rhs});
            else if (c == '*') bag.push_back(Mul{lhs, rhs});
            else bag.push_back(Div{lhs, rhs});
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
        parse_eval(ln);
    }
    return 0;
}
