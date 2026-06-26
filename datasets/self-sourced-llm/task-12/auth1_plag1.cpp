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

int resolve_node(const std::vector<Expr>& nodes, std::size_t root);

struct Evaluator {
    const std::vector<Expr>& tree;

    int operator()(const Number& n) const { return n.value; }

    int operator()(const Add& n) const {
        return resolve_node(tree, n.lhs) + resolve_node(tree, n.rhs);
    }

    int operator()(const Sub& n) const {
        return resolve_node(tree, n.lhs) - resolve_node(tree, n.rhs);
    }

    int operator()(const Mul& n) const {
        return resolve_node(tree, n.lhs) * resolve_node(tree, n.rhs);
    }

    int operator()(const Div& n) const {
        int denom = resolve_node(tree, n.rhs);
        if (denom == 0) throw std::runtime_error("div0");
        return resolve_node(tree, n.lhs) / denom;
    }
};

int resolve_node(const std::vector<Expr>& nodes, std::size_t root) {
    Evaluator ev{nodes};
    return std::visit(ev, nodes[root]);
}

bool run_line(const std::string& line) {
    std::vector<Expr> ast;
    std::vector<std::size_t> stack;
    std::istringstream iss(line);
    std::string tok;
    while (iss >> tok) {
        if (tok == "+" || tok == "-" || tok == "*" || tok == "/") {
            if (stack.size() < 2) return true;
            std::size_t rhs = stack.back(); stack.pop_back();
            std::size_t lhs = stack.back(); stack.pop_back();
            if (tok == "+") ast.push_back(Add{lhs, rhs});
            else if (tok == "-") ast.push_back(Sub{lhs, rhs});
            else if (tok == "*") ast.push_back(Mul{lhs, rhs});
            else ast.push_back(Div{lhs, rhs});
            stack.push_back(ast.size() - 1);
        } else {
            ast.push_back(Number{std::stoi(tok)});
            stack.push_back(ast.size() - 1);
        }
    }
    if (stack.empty()) return true;
    try {
        std::cout << resolve_node(ast, stack.back()) << '\n';
    } catch (...) {
        std::cout << "ERROR\n";
    }
    return true;
}

int main() {
    std::string s;
    while (std::getline(std::cin, s)) {
        if (s.empty()) break;
        run_line(s);
    }
    return 0;
}
