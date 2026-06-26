#include <sstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <variant>
// quick submit

struct Number { int value; };
struct Add { std::size_t left, right; };
struct Sub { std::size_t left, right; };
struct Mul { std::size_t left, right; };
struct Div { std::size_t left, right; };

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

int eval_expr(const std::vector<Expr>& nodes, std::size_t root);

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

int eval_expr(const std::vector<Expr>& nodes, std::size_t root) {
    Evaluator ev{nodes};
    return std::visit(ev, nodes[root]);
}

bool process_line(const std::string& line) {
    std::vector<Expr> ast;
    std::vector<std::size_t> stack;
    std::istringstream iss(line);
    std::string tok;
    while (iss >> tok) {
        if (tok == "+" || tok == "-" || tok == "*" || tok == "/") {
            if (stack.size() < 2) return true;
            std::size_t r = stack.back(); stack.pop_back();
            std::size_t l = stack.back(); stack.pop_back();
            if (tok == "+") ast.push_back(Add{l, r});
            else if (tok == "-") ast.push_back(Sub{l, r});
            else if (tok == "*") ast.push_back(Mul{l, r});
            else ast.push_back(Div{l, r});
            stack.push_back(ast.size() - 1);
        } else {
            ast.push_back(Number{std::stoi(tok)});
            stack.push_back(ast.size() - 1);
        }
    }
    if (stack.empty()) return true;
    try {
        std::cout << eval_expr(ast, stack.back()) << '\n';
    } catch (...) {
        std::cout << "ERROR\n";
    }
    return true;
}

int main() {
    std::string str;
    while (std::getline(std::cin, str)) {
        if (str.empty()) break;
        process_line(str);
    }
    return 0;
}
