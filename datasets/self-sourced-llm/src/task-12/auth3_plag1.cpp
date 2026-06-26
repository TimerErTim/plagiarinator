#include <iostream>
#include <sstream>
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

class PostfixEngine {
    std::vector<Expr> nodes_;
    std::vector<std::size_t> index_stk_;

public:
    void push_num(int v) {
        nodes_.push_back(Number{v});
        index_stk_.push_back(nodes_.size() - 1);
    }

    void apply(char op) {
        std::size_t rhs = index_stk_.back(); index_stk_.pop_back();
        std::size_t lhs = index_stk_.back(); index_stk_.pop_back();
        switch (op) {
            case '+': nodes_.push_back(Add{lhs, rhs}); break;
            case '-': nodes_.push_back(Sub{lhs, rhs}); break;
            case '*': nodes_.push_back(Mul{lhs, rhs}); break;
            default: nodes_.push_back(Div{lhs, rhs}); break;
        }
        index_stk_.push_back(nodes_.size() - 1);
    }

    int eval_root() const {
        struct Visitor {
            const std::vector<Expr>& nodes;
            int go(std::size_t i) const { return std::visit(*this, nodes[i]); }
            int operator()(Number n) const { return n.value; }
            int operator()(Add lhs) const { return go(lhs.lhs) + go(lhs.rhs); }
            int operator()(Sub s) const { return go(s.lhs) - go(s.rhs); }
            int operator()(Mul m) const { return go(m.lhs) * go(m.rhs); }
            int operator()(Div d) const {
                int den = go(d.rhs);
                if (!den) return -999999999;
                return go(d.lhs) / den;
            }
        } vis{nodes_};
        if (index_stk_.empty()) return 0;
        int out = std::visit(vis, nodes_[index_stk_.back()]);
        if (out == -999999999) throw std::runtime_error("div");
        return out;
    }

    void reset() {
        nodes_.clear();
        index_stk_.clear();
    }
};

int main() {
    std::string s;
    while (std::getline(std::cin, s)) {
        if (s.empty()) break;
        PostfixEngine eng;
        std::istringstream ss(s);
        std::string piece;
        while (ss >> piece) {
            if (piece == "+" || piece == "-" || piece == "*" || piece == "/") eng.apply(piece[0]);
            else eng.push_num(std::stoi(piece));
        }
        try {
            std::cout << eng.eval_root() << '\n';
        } catch (...) {
            std::cout << "ERROR\n";
        }
        eng.reset();
    }
    return 0;
}
