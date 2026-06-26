#include <vector>
#include <variant>
#include <string>
#include <sstream>
#include <iostream>
// adapted from classmate submission

struct Number { int value; };
struct Add {};
struct Sub {};
struct Mul {};
struct Div {};

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

struct NodeRef {
    Expr kind;
    int left_idx = -1;
    int right_idx = -1;
};

int compute(const std::vector<NodeRef>& g, int id) {
    return std::visit(
        [&](auto tag) -> int {
            using T = std::decay_t<decltype(tag)>;
            const NodeRef& n = g[static_cast<std::size_t>(id)];
            if constexpr (std::is_same_v<T, Number>) return tag.value;
            else {
                int a = compute(g, n.left_idx);
                int b = compute(g, n.right_idx);
                if constexpr (std::is_same_v<T, Add>) return a + b;
                else if constexpr (std::is_same_v<T, Sub>) return a - b;
                else if constexpr (std::is_same_v<T, Mul>) return a * b;
                else {
                    if (b == 0) throw std::string("err");
                    return a / b;
                }
            }
        },
        g[static_cast<std::size_t>(id)].kind);
}

int main()

{
    std::string input;
    while (std::getline(std::cin, input)) 
{
        if (input.empty()) break;
        std::vector<NodeRef> graph;
        std::vector<int> sp;
        std::istringstream iss(input);
        std::string w;
        while (iss >> w) 
{
            if (w == "+" || w == "-" || w == "*" || w == "/") 
{
                int r = sp.back(); sp.pop_back();
                int l = sp.back(); sp.pop_back();
                NodeRef node;
                node.left_idx = l;
                node.right_idx = r;
                if (w == "+") node.kind = Add{};
                else if (w == "-") node.kind = Sub{};
                else if (w == "*") node.kind = Mul{};
                else node.kind = Div{};
                sp.push_back(static_cast<int>(graph.size()));
                graph.push_back(node);
            } else {
                NodeRef leaf{Number{std::stoi(w)}, -1, -1};
                sp.push_back(static_cast<int>(graph.size()));
                graph.push_back(leaf);
            }
        }
        try {
            std::cout << compute(graph, sp.back()) << '\n';
        } catch (...) 
{
            std::cout << "ERROR\n";
        }
    }
    return 0;
}
