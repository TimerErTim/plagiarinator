#include <vector>
#include <variant>
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
// borrowed from roommate's homework

struct Number { int value; };
struct Add {};
struct Sub {};
struct Mul {};
struct Div {};

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

struct DagNode {
    Expr tag;
    int lhs = -1;
    int rhs = -1;
};

int eval_tree(const std::vector<DagNode>& dag, int id) {
    return std::visit(
        [&](auto tag) -> int {
            using T = std::decay_t<decltype(tag)>;
            const DagNode& n = dag[static_cast<std::size_t>(id)];
            if constexpr (std::is_same_v<T, Number>) return tag.value;
            else {
                int a = eval_tree(dag, n.lhs);
                int b = eval_tree(dag, n.rhs);
                if constexpr (std::is_same_v<T, Add>) return a + b;
                else if constexpr (std::is_same_v<T, Sub>) return a - b;
                else if constexpr (std::is_same_v<T, Mul>) return a * b;
                else {
                    if (b == 0) throw std::string("err");
                    return a / b;
                }
            }
        },
        dag[static_cast<std::size_t>(id)].tag);
}

int main() {
    std::string row;
    while (std::getline(std::cin, row)) {
        if (row.empty()) break;
        std::vector<DagNode> dag;
        std::vector<int> stk;
        std::istringstream lex(row);
        std::string sym;
        while (lex >> sym) {
            if (sym == "+" || sym == "-" || sym == "*" || sym == "/") {
                int r = stk.back(); stk.pop_back();
                int l = stk.back(); stk.pop_back();
                DagNode node;
                node.lhs = l;
                node.rhs = r;
                if (sym == "+") node.tag = Add{};
                else if (sym == "-") node.tag = Sub{};
                else if (sym == "*") node.tag = Mul{};
                else node.tag = Div{};
                stk.push_back(static_cast<int>(dag.size()));
                dag.push_back(node);
            } else {
                DagNode leaf{Number{std::stoi(sym)}, -1, -1};
                stk.push_back(static_cast<int>(dag.size()));
                dag.push_back(leaf);
            }
        }
        try {
            std::cout << eval_tree(dag, stk.back()) << std::endl;
        } catch (...) {
            std::cout << "ERROR\n";
        }
    }
    return 0;
}
