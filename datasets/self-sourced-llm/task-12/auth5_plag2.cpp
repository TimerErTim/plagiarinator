#include <iostream>
#include <sstream>
#include <string>
#include <variant>
#include <vector>
#include <stdexcept>

struct Number { int value; };
struct Add {};
struct Sub {};
struct Mul {};
struct Div {};

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

struct NodeRef {
    Expr kind;
    int left = -1;
    int right = -1;
};

int compute(const std::vector<NodeRef>& g, int id)
{
    return std::visit(
        [&](auto tag) -> int {
            using T = std::decay_t<decltype(tag)>;
            const NodeRef& n = g[static_cast<std::size_t>(id)];
            if constexpr (std::is_same_v<T, Number>) return tag.value;
            else {
                int a = compute(g, n.left);
                int b = compute(g, n.right);
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

static void push_leaf(std::vector<NodeRef>& graph, std::vector<int>& sp, int val)
{
    NodeRef leaf{Number{val}, -1, -1};
    sp.push_back(static_cast<int>(graph.size()));
    graph.push_back(leaf);
}

static void push_bin(std::vector<NodeRef>& graph, std::vector<int>& sp, const std::string& w)
{
    int r = sp.back(); sp.pop_back();
    int l = sp.back(); sp.pop_back();
    NodeRef node;
    node.left = l;
    node.right = r;
    if (w == "+") node.kind = Add{};
    else if (w == "-") node.kind = Sub{};
    else if (w == "*") node.kind = Mul{};
    else node.kind = Div{};
    sp.push_back(static_cast<int>(graph.size()));
    graph.push_back(node);
}

int main()
{
    std::string line;
    while (std::getline(std::cin, line))
    {
        if (line.empty()) break;
        std::vector<NodeRef> graph;
        std::vector<int> sp;
        std::istringstream iss(line);
        std::string w;
        while (iss >> w)
        {
            if (w == "+" || w == "-" || w == "*" || w == "/")
                push_bin(graph, sp, w);
            else
                push_leaf(graph, sp, std::stoi(w));
        }
        try
        {
            std::cout << compute(graph, sp.back()) << '\n';
        }
        catch (...)
        {
            std::cout << "ERROR\n";
        }
    }
    return 0;
}
