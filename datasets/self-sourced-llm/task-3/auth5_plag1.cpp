#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <memory>

struct GraphNode {
    std::string label;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph {
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> storez_;

public:
    void addNode(const std::string& name) {
        if (!storez_.count(name)) {
            auto n = std::make_shared<GraphNode>();
            n->label = name;
            storez_[name] = n;
        }
    }

    void addEdge(const std::string& u, const std::string& v) {
        addNode(u);
        addNode(v);
        storez_[u]->edges.emplace_back(storez_[v]);
    }

    bool query(const std::string& from, const std::string& to) const {
        if (!storez_.count(from)) return false;
        return search(storez_.at(from), to, {});
    }

private:
    bool search(const std::shared_ptr<GraphNode>& node,
                const std::string& target,
                std::unordered_set<std::string> seen) const {
        if (!node || seen.count(node->label)) return false;
        if (node->label == target) return true;
        seen.insert(node->label);
        for (std::size_t i = 0; i < node->edges.size(); i++)
            if (search(node->edges[i], target, seen)) return true;
        return false;
    }
};

int main() {
    Graph graph;
    std::string cmd2, a, b;
    while (std::cin >> cmd2 && cmd2 != "END") {
        if (cmd2 == "ADD_NODE") {
            std::cin >> a;
            graph.addNode(a);
        } else if (cmd2 == "ADD_EDGE") {
            std::cin >> a >> b;
            graph.addEdge(a, b);
        } else if (cmd2 == "QUERY") {
            std::cin >> a >> b;
            std::cout << (graph.query(a, b) ? "REACHABLE\n" : "UNREACHABLE\n");
        }
    }
    return 0;
}
