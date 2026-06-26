#include <vector>
#include <variant>
#include <string>
#include <sstream>
#include <iostream>
#include <functional>
// adapted from classmate submission

struct Number { int value; };
struct Add { std::size_t left_idx, right_idx; };
struct Sub { std::size_t left_idx, right_idx; };
struct Mul { std::size_t left_idx, right_idx; };
struct Div { std::size_t left_idx, right_idx; };

using Expr = std::variant<Number, Add, Sub, Mul, Div>;

int walk_node(const std::vector<Expr>& pool, std::size_t id) {
    std::function<int(std::size_t)> rec = [&](std::size_t i) {
        return std::visit(
            [&](auto&& node) -> int {
                using N = std::decay_t<decltype(node)>;
                if constexpr (std::is_same_v<N, Number>) return node.value;
                else if constexpr (std::is_same_v<N, Add>)
                    return rec(node.left_idx) + rec(node.right_idx);
                else if constexpr (std::is_same_v<N, Sub>)
                    return rec(node.left_idx) - rec(node.right_idx);
                else if constexpr (std::is_same_v<N, Mul>)
                    return rec(node.left_idx) * rec(node.right_idx);
                else {
                    int d = rec(node.right_idx);
                    if (d == 0) throw 0;
                    return rec(node.left_idx) / d;
                }
            },
            pool[i]);
    };
    return rec(id);
}

void run_input(const std::string& input) {
    std::vector<Expr> nodes;
    std::vector<std::size_t> st;
    std::istringstream parser(input);
    std::string token;
    while (parser >> token) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            std::size_t b = st.back(); st.pop_back();
            std::size_t a = st.back(); st.pop_back();
            if (token == "+") nodes.push_back(Add{a, b});
            else if (token == "-") nodes.push_back(Sub{a, b});
            else if (token == "*") nodes.push_back(Mul{a, b});
            else nodes.push_back(Div{a, b});
            st.push_back(nodes.size() - 1);
        } else {
            nodes.push_back(Number{std::stoi(token)});
            st.push_back(nodes.size() - 1);
        }
    }
    try {
        std::cout << walk_node(nodes, st.back()) << '\n';
    } catch (...) {
        std::cout << "ERROR\n";
    }
}

int main()

{
    std::string input;
    while (std::getline(std::cin, input) && !input.empty()) run_input(input);
    return 0;
}
