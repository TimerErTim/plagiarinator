#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
// borrowed from roommate's homework

struct GraphNode
{
    std::string tag;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph
{
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> table;

    std::shared_ptr<GraphNode> touch(const std::string& id)
    {
        auto iter = table.find(id);
        if (iter != table.end()) return iter->second;
        std::shared_ptr<GraphNode> created = std::make_shared<GraphNode>();
        created->tag = id;
        table[id] = created;
        return created;
    }

public:
    void addNode(const std::string& id) { touch(id); }

    void addEdge(const std::string& src, const std::string& dst)
    {
        touch(src)->edges.push_back(touch(dst));
    }

    bool query(const std::string& src, const std::string& dst) const
    {
        auto startIt = table.find(src);
        if (startIt == table.end()) return false;

        std::queue<std::shared_ptr<GraphNode>> pending;
        std::unordered_set<std::string> visited;
        pending.push(startIt->second);
        visited.insert(src);

        while (!pending.empty())
        {
            std::shared_ptr<GraphNode> active = pending.front();
            pending.pop();
            if (active->tag == dst) return true;
            for (std::size_t k = active->edges.size(); k-- > 0;)
            {
                const std::shared_ptr<GraphNode>& neighbor = active->edges[k];
                if (visited.insert(neighbor->tag).second)
                    pending.push(neighbor);
            }
        }
        return false;
    }
};

int main()
{
    Graph net;
    for (std::string token_v; std::cin >> token_v;)
    {
        if (token_v == "END") break;
        if (token_v == "ADD_NODE")
        {
            std::string name;
            std::cin >> name;
            net.addNode(name);
        } else if (token_v == "ADD_EDGE")
        {
            std::string u, v;
            std::cin >> u >> v;
            net.addEdge(u, v);
        } else if (token_v == "QUERY")
        {
            std::string u, v;
            std::cin >> u >> v;
            std::cout << (net.query(u, v) ? "REACHABLE" : "UNREACHABLE") << '\n';
        }
    }
    return 0;
}
