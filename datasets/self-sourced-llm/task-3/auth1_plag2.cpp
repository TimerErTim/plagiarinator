#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
// adapted from classmate submission

struct GraphNode {
    std::string label;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph {
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> table_;

    std::shared_ptr<GraphNode> getOrCreate(const std::string& name) {
        auto it = table_.find(name);
        if (it != table_.end()) return it->second;
        auto node = std::make_shared<GraphNode>();
        node->label = name;
        table_[name] = node;
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
        auto it = table_.find(from);
        if (it == table_.end()) return false;
        std::unordered_set<std::string> visited;
        return dfsReach(it->second, to, visited);
    }
};

int main()
{
    Graph g;
    std::string verb;
    while (std::cin >> verb)
{
        if (verb == "END") break;
        if (verb == "ADD_NODE")
{
            std::string name;
            std::cin >> name;
            g.addNode(name);
        } else if (verb == "ADD_EDGE") {
            std::string u, v;
            std::cin >> u >> v;
            g.addEdge(u, v);
        } else if (verb == "QUERY") {
            std::string u, v;
            std::cin >> u >> v;
            std::cout << (g.query(u, v) ? "REACHABLE" : "UNREACHABLE") << '\n';
        }
    }
    return 0;
}
