#include <functional>
#include <iostream>
#include <sstream>
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

int resolve_node(const std::vector<Expr>& pool, std::size_t id) {
    std::function<int(std::size_t)> rec = [&](std::size_t i) {
        return std::visit(
            [&](auto&& node) -> int {
                using N = std::decay_t<decltype(node)>;
                if constexpr (std::is_same_v<N, Number>) return node.value;
                else if constexpr (std::is_same_v<N, Add>)
                    return rec(node.lhs) + rec(node.rhs);
                else if constexpr (std::is_same_v<N, Sub>)
                    return rec(node.lhs) - rec(node.rhs);
                else if constexpr (std::is_same_v<N, Mul>)
                    return rec(node.lhs) * rec(node.rhs);
                else {
                    int d = rec(node.rhs);
                    if (d == 0) throw 0;
                    return rec(node.lhs) / d;
                }
            },
            pool[i]);
    };
    return rec(id);
}

void process_input(const std::string& input) {
    std::vector<Expr> nodes;
    std::vector<std::size_t> st;
    std::istringstream parser(input);
    std::string token;
    while (parser >> token) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            std::size_t rhs = st.back(); st.pop_back();
            std::size_t lhs = st.back(); st.pop_back();
            if (token == "+") nodes.emplace_back(Add{lhs, rhs});
            else if (token == "-") nodes.emplace_back(Sub{lhs, rhs});
            else if (token == "*") nodes.emplace_back(Mul{lhs, rhs});
            else nodes.emplace_back(Div{lhs, rhs});
            st.push_back(nodes.size() - 1);
        } else {
            nodes.emplace_back(Number{std::stoi(token)});
            st.push_back(nodes.size() - 1);
        }
    }
    try {
        std::cout << resolve_node(nodes, st.back()) << '\n';
    } catch (...) {
        std::cout << "ERROR\n";
    }
}

int main() {
    std::string s;
    while (std::getline(std::cin, s) && !s.empty()) process_input(s);
    return 0;
}
