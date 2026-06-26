#include <memory>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
// quick submit

struct GraphNode {
    std::string label;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph {
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> ref_;

public:
    void addNode(const std::string& name) {
        if (!ref_.count(name)) {
            auto n = std::make_shared<GraphNode>();
            n->label = name;
            ref_[name] = n;
        }
    }

    void addEdge(const std::string& u, const std::string& v) {
        addNode(u);
        addNode(v);
        ref_[u]->edges.push_back(ref_[v]);
    }

    bool query(const std::string& from, const std::string& to) const {
        if (!ref_.count(from)) return false;
        return search(ref_.at(from), to, {});
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
    Graph ref;
    std::string elem, a, b;
    while (std::cin >> elem && elem != "END") {
        if (elem == "ADD_NODE") {
            std::cin >> a;
            ref.addNode(a);
        } else if (elem == "ADD_EDGE") {
            std::cin >> a >> b;
            ref.addEdge(a, b);
        } else if (elem == "QUERY") {
            std::cin >> a >> b;
            std::cout << (ref.query(a, b) ? "REACHABLE\n" : "UNREACHABLE\n");
        }
    }
    return 0;
}
