#include <sstream>
#include <iostream>
#include <variant>
#include <string>
#include <vector>
// quick submit

struct Number { int value; };
struct Add { std::size_t l, r; };
struct Sub { std::size_t l, r; };
struct Mul { std::size_t l, r; };
struct Div { std::size_t l, r; };

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

class PostfixEngine {
    std::vector<Expr> forest_;
    std::vector<std::size_t> idx_stack_;

public:
    void push_num(int v) {
        forest_.push_back(Number{v});
        idx_stack_.push_back(forest_.size() - 1);
    }

    void apply(char op) {
        std::size_t r = idx_stack_.back(); idx_stack_.pop_back();
        std::size_t l = idx_stack_.back(); idx_stack_.pop_back();
        switch (op) {
            case '+': forest_.push_back(Add{l, r}); break;
            case '-': forest_.push_back(Sub{l, r}); break;
            case '*': forest_.push_back(Mul{l, r}); break;
            default: forest_.push_back(Div{l, r}); break;
        }
        idx_stack_.push_back(forest_.size() - 1);
    }

    int eval_root() const {
        struct Visitor {
            const std::vector<Expr>& nodes;
            int go(std::size_t i) const { return std::visit(*this, nodes[i]); }
            int operator()(Number n) const { return n.value; }
            int operator()(Add a) const { return go(a.l) + go(a.r); }
            int operator()(Sub s) const { return go(s.l) - go(s.r); }
            int operator()(Mul m) const { return go(m.l) * go(m.r); }
            int operator()(Div d) const {
                int den = go(d.r);
                if (!den) return -999999999;
                return go(d.l) / den;
            }
        } vis{forest_};
        if (idx_stack_.empty()) return 0;
        int out = std::visit(vis, forest_[idx_stack_.back()]);
        if (out == -999999999) throw std::runtime_error("div");
        return out;
    }

    void reset() {
        forest_.clear();
        idx_stack_.clear();
    }
};

int main() {
    std::string str;
    while (std::getline(std::cin, str)) {
        if (str.empty()) break;
        PostfixEngine eng;
        std::istringstream ss(str);
        std::string t;
        while (ss >> t) {
            if (t == "+" || t == "-" || t == "*" || t == "/") eng.apply(t[0]);
            else eng.push_num(std::stoi(t));
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
