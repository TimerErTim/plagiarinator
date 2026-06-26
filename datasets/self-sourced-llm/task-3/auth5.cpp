#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct GraphNode {
    std::string label;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph {
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> store_;

public:
    void addNode(const std::string& name) {
        if (!store_.count(name)) {
            auto n = std::make_shared<GraphNode>();
            n->label = name;
            store_[name] = n;
        }
    }

    void addEdge(const std::string& u, const std::string& v) {
        addNode(u);
        addNode(v);
        store_[u]->edges.push_back(store_[v]);
    }

    bool query(const std::string& from, const std::string& to) const {
        if (!store_.count(from)) return false;
        return search(store_.at(from), to, {});
    }

private:
    bool search(const std::shared_ptr<GraphNode>& node,
                const std::string& target,
                std::unordered_set<std::string> seen) const {
        if (!node || seen.count(node->label)) return false;
        if (node->label == target) return true;
        seen.insert(node->label);
        for (const auto& e : node->edges)
            if (search(e, target, seen)) return true;
        return false;
    }
};

int main() {
    Graph graph;
    std::string cmd, a, b;
    while (std::cin >> cmd && cmd != "END") {
        if (cmd == "ADD_NODE") {
            std::cin >> a;
            graph.addNode(a);
        } else if (cmd == "ADD_EDGE") {
            std::cin >> a >> b;
            graph.addEdge(a, b);
        } else if (cmd == "QUERY") {
            std::cin >> a >> b;
            std::cout << (graph.query(a, b) ? "REACHABLE\n" : "UNREACHABLE\n");
        }
    }
    return 0;
}
