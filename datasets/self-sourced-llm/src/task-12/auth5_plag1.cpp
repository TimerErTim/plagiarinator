#include <iostream>
#include <sstream>
#include <string>
#include <variant>
#include <vector>
// cleaned up version

struct Number { int value; };
struct Add {};
struct Sub {};
struct Mul {};
struct Div {};

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

struct NodeRef {
    Expr kind;
    int lhs = -1;
    int rhs = -1;
};

int eval_graph(const std::vector<NodeRef>& g, int id) {
    return std::visit(
        [&](auto tag) -> int {
            using T = std::decay_t<decltype(tag)>;
            const NodeRef& n = g[static_cast<std::size_t>(id)];
            if constexpr (std::is_same_v<T, Number>) return tag.value;
            else {
                int lhs = eval_graph(g, n.lhs);
                int rhs = eval_graph(g, n.rhs);
                if constexpr (std::is_same_v<T, Add>) return lhs + rhs;
                else if constexpr (std::is_same_v<T, Sub>) return lhs - rhs;
                else if constexpr (std::is_same_v<T, Mul>) return lhs * rhs;
                else {
                    if (rhs == 0) throw std::string("err");
                    return lhs / rhs;
                }
            }
        },
        g[static_cast<std::size_t>(id)].kind);
}

int main() {
    std::string s;
    while (std::getline(std::cin, s)) {
        if (s.empty()) break;
        std::vector<NodeRef> graph;
        std::vector<int> sp;
        std::istringstream iss(s);
        std::string piece;
        while (iss >> piece) {
            if (piece == "+" || piece == "-" || piece == "*" || piece == "/") {
                int rhs = sp.back(); sp.pop_back();
                int lhs = sp.back(); sp.pop_back();
                NodeRef node;
                node.lhs = lhs;
                node.rhs = rhs;
                if (piece == "+") node.kind = Add{};
                else if (piece == "-") node.kind = Sub{};
                else if (piece == "*") node.kind = Mul{};
                else node.kind = Div{};
                sp.push_back(static_cast<int>(graph.size()));
                graph.push_back(node);
            } else {
                NodeRef leaf{Number{std::stoi(piece)}, -1, -1};
                sp.push_back(static_cast<int>(graph.size()));
                graph.push_back(leaf);
            }
        }
        try {
            std::cout << eval_graph(graph, sp.back()) << '\n';
        } catch (...) {
            std::cout << "ERROR\n";
        }
    }
    return 0;
}
