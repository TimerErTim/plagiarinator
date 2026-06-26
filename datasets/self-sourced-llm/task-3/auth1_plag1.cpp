#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
// cleaned up version

struct GraphNode {
    std::string label;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph {
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> nodes_;

    std::shared_ptr<GraphNode> getOrCreate(const std::string& name) {
        auto it = nodes_.find(name);
        if (it != nodes_.end()) return it->second;
        auto node = std::make_shared<GraphNode>();
        node->label = name;
        nodes_[name] = node;
        return node;
    }

    bool dfsReach(const std::shared_ptr<GraphNode>& cur,
                  const std::string& target,
                  std::unordered_set<std::string>& seen) const {
        if (!cur || seen.count(cur->label)) return false;
        if (cur->label == target) return true;
        seen.insert(cur->label);
        for (const auto& nxt : cur->edges)
            if (dfsReach(nxt, target, seen)) return true;
        return false;
    }

public:
    void addNode(const std::string& name) { getOrCreate(name); }

    void addEdge(const std::string& from, const std::string& to) {
        auto a = getOrCreate(from);
        auto b = getOrCreate(to);
        a->edges.push_back(b);
    }

    bool query(const std::string& from, const std::string& to) const {
        auto it = nodes_.find(from);
        if (it == nodes_.end()) return false;
        std::unordered_set<std::string> visited;
        return dfsReach(it->second, to, visited);
    }
};

int main() {
    Graph g;
    std::string cmd;
    while (std::cin >> cmd) {
        if (cmd == "END") break;
        if (cmd == "ADD_NODE") {
            std::string tag;
            std::cin >> tag;
            g.addNode(tag);
        } else if (cmd == "ADD_EDGE") {
            std::string u, v;
            std::cin >> u >> v;
            g.addEdge(u, v);
        } else if (cmd == "QUERY") {
            std::string u, v;
            std::cin >> u >> v;
            std::cout << (g.query(u, v) ? "REACHABLE" : "UNREACHABLE") << std::endl;
        }
    }
    return 0;
}
