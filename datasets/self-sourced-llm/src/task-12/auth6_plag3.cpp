#include <vector>
#include <variant>
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
// borrowed from roommate's homework

struct Number { int value; };
struct Add { std::size_t a, b; };
struct Sub { std::size_t a, b; };
struct Mul { std::size_t a, b; };
struct Div { std::size_t a, b; };

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

struct Runner {
    const std::vector<Expr>* pool;

    int run(std::size_t i) const {
        struct V {
            const Runner* ctx;
            int operator()(Number n) const { return n.value; }
            int operator()(Add x) const { return ctx->run(x.a) + ctx->run(x.b); }
            int operator()(Sub x) const { return ctx->run(x.a) - ctx->run(x.b); }
            int operator()(Mul x) const { return ctx->run(x.a) * ctx->run(x.b); }
            int operator()(Div x) const {
                int y = ctx->run(x.b);
                if (y == 0) throw std::runtime_error("div0");
                return ctx->run(x.a) / y;
            }
        };
        return std::visit(V{this}, (*pool)[i]);
    }
};

void eval_line(const std::string& row) {
    std::vector<Expr> pool;
    std::vector<std::size_t> frames;
    std::istringstream lex(row);
    std::string sym;
    while (lex >> sym) {
        if (sym.size() == 1 && std::string("+-*/").find(sym[0]) != std::string::npos) {
            std::size_t y = frames.back(); frames.pop_back();
            std::size_t x = frames.back(); frames.pop_back();
            char c = sym[0];
            if (c == '+') pool.push_back(Add{x, y});
            else if (c == '-') pool.push_back(Sub{x, y});
            else if (c == '*') pool.push_back(Mul{x, y});
            else pool.push_back(Div{x, y});
            frames.push_back(pool.size() - 1);
        } else {
            pool.push_back(Number{std::stoi(sym)});
            frames.push_back(pool.size() - 1);
        }
    }
    Runner ctx{&pool};
    try {
        std::cout << ctx.run(frames.back()) << std::endl;
    } catch (...) {
        std::cout << "ERROR\n";
    }
}

int main() {
    std::string row;
    while (std::getline(std::cin, row)) {
        if (row.empty()) break;
        eval_line(row);
    }
    return 0;
}
