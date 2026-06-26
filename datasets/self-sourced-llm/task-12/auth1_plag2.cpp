#include <vector>
#include <variant>
#include <string>
#include <stdexcept>
#include <sstream>
#include <iostream>
// adapted from classmate submission

struct Number { int value; };
struct Add { std::size_t left_idx, right_idx; };
struct Sub { std::size_t left_idx, right_idx; };
struct Mul { std::size_t left_idx, right_idx; };
struct Div { std::size_t left_idx, right_idx; };

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

int compute_expr(const std::vector<Expr>& nodes, std::size_t root);

struct Evaluator {
    const std::vector<Expr>& tree;

    int operator()(const Number& n) const { return n.value; }

    int operator()(const Add& n) const {
        return compute_expr(tree, n.left_idx) + compute_expr(tree, n.right_idx);
    }

    int operator()(const Sub& n) const {
        return compute_expr(tree, n.left_idx) - compute_expr(tree, n.right_idx);
    }

    int operator()(const Mul& n) const {
        return compute_expr(tree, n.left_idx) * compute_expr(tree, n.right_idx);
    }

    int operator()(const Div& n) const {
        int denom = compute_expr(tree, n.right_idx);
        if (denom == 0) throw std::runtime_error("div0");
        return compute_expr(tree, n.left_idx) / denom;
    }
};

int compute_expr(const std::vector<Expr>& nodes, std::size_t root) {
    Evaluator ev{nodes};
    return std::visit(ev, nodes[root]);
}

bool handle_line(const std::string& line) {
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
        std::cout << compute_expr(ast, stack.back()) << '\n';
    } catch (...) {
        std::cout << "ERROR\n";
    }
    return true;
}

int main()

{
    std::string input;
    while (std::getline(std::cin, input)) 
{
        if (input.empty()) break;
        handle_line(input);
    }
    return 0;
}
