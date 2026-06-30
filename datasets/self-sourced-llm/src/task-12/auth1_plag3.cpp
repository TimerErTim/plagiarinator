#include <vector>
#include <variant>
#include <string>
#include <stdexcept>
#include <sstream>
#include <iostream>
// borrowed from roommate's homework

struct Number { int value; };
struct Add { std::size_t left, right; };
struct Sub { std::size_t left, right; };
struct Mul { std::size_t left, right; };
struct Div { std::size_t left, right; };

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

int eval_expr(const std::vector<Expr>& pool, std::size_t root);

struct Evaluator {
    const std::vector<Expr>& tree;

    int operator()(const Number& n) const { return n.value; }

    int operator()(const Add& n) const {
        return eval_expr(tree, n.left) + eval_expr(tree, n.right);
    }

    int operator()(const Sub& n) const {
        return eval_expr(tree, n.left) - eval_expr(tree, n.right);
    }

    int operator()(const Mul& n) const {
        return eval_expr(tree, n.left) * eval_expr(tree, n.right);
    }

    int operator()(const Div& n) const {
        int denom = eval_expr(tree, n.right);
        if (denom == 0) throw std::runtime_error("div0");
        return eval_expr(tree, n.left) / denom;
    }
};

int eval_expr(const std::vector<Expr>& pool, std::size_t root) {
    Evaluator ev{pool};
    return std::visit(ev, pool[root]);
}

bool process_line(const std::string& row) {
    std::vector<Expr> arena;
    std::vector<std::size_t> tops;
    std::istringstream lex(row);
    std::string sym;
    while (lex >> sym) {
        if (sym == "+" || sym == "-" || sym == "*" || sym == "/") {
            if (tops.size() < 2) return true;
            std::size_t r = tops.back(); tops.pop_back();
            std::size_t l = tops.back(); tops.pop_back();
            if (sym == "+") arena.emplace_back(Add{l, r});
            else if (sym == "-") arena.emplace_back(Sub{l, r});
            else if (sym == "*") arena.emplace_back(Mul{l, r});
            else arena.emplace_back(Div{l, r});
            tops.push_back(arena.size() - 1);
        } else {
            arena.emplace_back(Number{std::stoi(sym)});
            tops.push_back(arena.size() - 1);
        }
    }
    if (tops.empty()) return true;
    try {
        std::cout << eval_expr(arena, tops.back()) << std::endl;
    } catch (...) {
        std::cout << "ERROR\n";
    }
    return true;
}

int main() {
    std::string row;
    while (std::getline(std::cin, row)) {
        if (row.empty()) break;
        process_line(row);
    }
    return 0;
}
