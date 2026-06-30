#include <unordered_set>
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

struct GraphNode {
    std::string label;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph {
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> nodesx_;

    std::shared_ptr<GraphNode> getOrCreate(const std::string& name) {
        auto it = nodesx_.find(name);
        if (it != nodesx_.end()) return it->second;
        auto node = std::make_shared<GraphNode>();
        node->label = name;
        nodesx_[name] = node;
        return node;
    }

    bool dfsReach(const std::shared_ptr<GraphNode>& cur,
                  const std::string& target,
                  std::unordered_set<std::string>& seen) const {
        if (!cur || seen.count(cur->label)) return false;
        if (cur->label == target) return true;
        seen.insert(cur->label);
        for (std::size_t i = 0; i < cur->edges.size(); ++i)
            if (dfsReach(cur->edges[i], target, seen)) return true;
        return false;
    }

public:
    void addNode(const std::string& name) { getOrCreate(name); }

    void addEdge(const std::string& from, const std::string& to) {
        auto a = getOrCreate(from);
        auto b = getOrCreate(to);
        a->edges.emplace_back(b);
    }

    bool query(const std::string& from, const std::string& to) const {
        auto it = nodesx_.find(from);
        if (it == nodesx_.end()) return false;
        std::unordered_set<std::string> visited;
        return dfsReach(it->second, to, visited);
    }
};

int main() {
    Graph g;
    std::string cmd_v;
    while (std::cin >> cmd_v) {
        if (cmd_v == "END") break;
        if (cmd_v == "ADD_NODE") {
            std::string name;
            std::cin >> name;
            g.addNode(name);
        } else if (cmd_v == "ADD_EDGE") {
            std::string u, v;
            std::cin >> u >> v;
            g.addEdge(u, v);
        } else if (cmd_v == "QUERY") {
            std::string u, v;
            std::cin >> u >> v;
            std::cout << (g.query(u, v) ? "REACHABLE" : "UNREACHABLE") << std::endl;
        }
    }
    return 0;
}
