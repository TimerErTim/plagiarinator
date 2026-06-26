#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <queue>
#include <memory>
#include <iostream>
// adapted from classmate submission

struct GraphNode {
    std::string label;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph {
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> table_;

    std::shared_ptr<GraphNode> touch(const std::string& id) {
        auto iter = table_.find(id);
        if (iter != table_.end()) return iter->second;
        std::shared_ptr<GraphNode> created = std::make_shared<GraphNode>();
        created->label = id;
        table_[id] = created;
        return created;
    }

public:
    void addNode(const std::string& id) { touch(id); }

    void addEdge(const std::string& src, const std::string& dst) {
        touch(src)->edges.emplace_back(touch(dst));
    }

    bool query(const std::string& src, const std::string& dst) const {
        auto startIt = table_.find(src);
        if (startIt == table_.end()) return false;

        std::queue<std::shared_ptr<GraphNode>> pending;
        std::unordered_set<std::string> visited;
        pending.push(startIt->second);
        visited.insert(src);

        while (!pending.empty()) {
            std::shared_ptr<GraphNode> active = pending.front();
            pending.pop();
            if (active->label == dst) return true;
            for (const std::shared_ptr<GraphNode>& neighbor : active->edges)
{
                if (visited.insert(neighbor->label).second)
                    pending.push(neighbor);
            }
        }
        return false;
    }
};

int main()
{
    Graph g;
    for (std::string verb; std::cin >> verb;)
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
