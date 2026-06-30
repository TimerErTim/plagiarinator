#include <vector>
#include <variant>
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
// borrowed from roommate's homework

struct Number { int value; };
struct Add { std::size_t l, r; };
struct Sub { std::size_t l, r; };
struct Mul { std::size_t l, r; };
struct Div { std::size_t l, r; };

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

class RpnMachine {
    std::vector<Expr> arena_;
    std::vector<std::size_t> tops_;

public:
    void push_num(int v) {
        arena_.push_back(Number{v});
        tops_.push_back(arena_.size() - 1);
    }

    void apply(char op) {
        std::size_t r = tops_.back(); tops_.pop_back();
        std::size_t l = tops_.back(); tops_.pop_back();
        switch (op) {
            case '+': arena_.push_back(Add{l, r}); break;
            case '-': arena_.push_back(Sub{l, r}); break;
            case '*': arena_.push_back(Mul{l, r}); break;
            default: arena_.push_back(Div{l, r}); break;
        }
        tops_.push_back(arena_.size() - 1);
    }

    int eval_root() const {
        struct Visitor {
            const std::vector<Expr>& pool;
            int walk(std::size_t i) const { return std::visit(*this, pool[i]); }
            int operator()(Number n) const { return n.value; }
            int operator()(Add a) const { return walk(a.l) + walk(a.r); }
            int operator()(Sub s) const { return walk(s.l) - walk(s.r); }
            int operator()(Mul m) const { return walk(m.l) * walk(m.r); }
            int operator()(Div d) const {
                int den = walk(d.r);
                if (!den) return -999999999;
                return walk(d.l) / den;
            }
        } vis{arena_};
        if (tops_.empty()) return 0;
        int out = std::visit(vis, arena_[tops_.back()]);
        if (out == -999999999) throw std::runtime_error("div");
        return out;
    }

    void reset() {
        arena_.clear();
        tops_.clear();
    }
};

int main() {
    std::string row;
    while (std::getline(std::cin, row)) {
        if (row.empty()) break;
        RpnMachine mach;
        std::istringstream lex(row);
        std::string sym;
        while (lex >> sym) {
            if (sym == "+" || sym == "-" || sym == "*" || sym == "/") mach.apply(sym[0]);
            else mach.push_num(std::stoi(sym));
        }
        try {
            std::cout << mach.eval_root() << std::endl;
        } catch (...) {
            std::cout << "ERROR\n";
        }
        mach.reset();
    }
    return 0;
}
