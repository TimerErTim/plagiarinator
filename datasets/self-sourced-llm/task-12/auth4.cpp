#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

struct Number { int value; };
struct Add { std::size_t left, right; };
struct Sub { std::size_t left, right; };
struct Mul { std::size_t left, right; };
struct Div { std::size_t left, right; };

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

int eval_node(const std::vector<Expr>& pool, std::size_t id) {
    std::function<int(std::size_t)> rec = [&](std::size_t i) {
        return std::visit(
            [&](auto&& node) -> int {
                using N = std::decay_t<decltype(node)>;
                if constexpr (std::is_same_v<N, Number>) return node.value;
                else if constexpr (std::is_same_v<N, Add>)
                    return rec(node.left) + rec(node.right);
                else if constexpr (std::is_same_v<N, Sub>)
                    return rec(node.left) - rec(node.right);
                else if constexpr (std::is_same_v<N, Mul>)
                    return rec(node.left) * rec(node.right);
                else {
                    int d = rec(node.right);
                    if (d == 0) throw 0;
                    return rec(node.left) / d;
                }
            },
            pool[i]);
    };
    return rec(id);
}

void handle(const std::string& input) {
    std::vector<Expr> nodes;
    std::vector<std::size_t> st;
    std::istringstream parser(input);
    std::string token;
    while (parser >> token) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            std::size_t b = st.back(); st.pop_back();
            std::size_t a = st.back(); st.pop_back();
            if (token == "+") nodes.emplace_back(Add{a, b});
            else if (token == "-") nodes.emplace_back(Sub{a, b});
            else if (token == "*") nodes.emplace_back(Mul{a, b});
            else nodes.emplace_back(Div{a, b});
            st.push_back(nodes.size() - 1);
        } else {
            nodes.emplace_back(Number{std::stoi(token)});
            st.push_back(nodes.size() - 1);
        }
    }
    try {
        std::cout << eval_node(nodes, st.back()) << '\n';
    } catch (...) {
        std::cout << "ERROR\n";
    }
}

int main() {
    std::string row;
    while (std::getline(std::cin, row) && !row.empty()) handle(row);
    return 0;
}
