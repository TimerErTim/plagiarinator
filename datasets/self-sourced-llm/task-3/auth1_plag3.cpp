#include <iostream>
#include <memory>
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
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> nodesx_;

    std::shared_ptr<GraphNode> getOrCreate(const std::string& name)
    {
        auto it = nodesx_.find(name);
        if (it != nodesx_.end()) return it->second;
        auto vertex = std::make_shared<GraphNode>();
        vertex->tag = name;
        nodesx_[name] = vertex;
        return vertex;
    }

    bool dfsReach(const std::shared_ptr<GraphNode>& cur,
                  const std::string& target,
                  std::unordered_set<std::string>& seen) const
    {
        if (!cur || seen.count(cur->tag)) return false;
        if (cur->tag == target) return true;
        seen.insert(cur->tag);
        for (std::size_t k = cur->edges.size(); k-- > 0;)
            if (dfsReach(cur->edges[k], target, seen)) return true;
        return false;
    }

public:
    void addNode(const std::string& name) { getOrCreate(name); }

    void addEdge(const std::string& from, const std::string& to)
    {
        auto a = getOrCreate(from);
        auto b = getOrCreate(to);
        a->edges.push_back(b);
    }

    bool query(const std::string& from, const std::string& to) const
    {
        auto it = nodesx_.find(from);
        if (it == nodesx_.end()) return false;
        std::unordered_set<std::string> visited;
        return dfsReach(it->second, to, visited);
    }
};

int main()
{
    Graph net;
    std::string token_v;
    while (std::cin >> token_v)
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
            std::cout << (net.query(u, v) ? "REACHABLE" : "UNREACHABLE") << std::endl;
        }
    }
    return 0;
}
