#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <memory>
#include <string>

struct GraphNode
{
    std::string label;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph
{
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> nodesv_;

    std::shared_ptr<GraphNode> getOrCreate(const std::string& name)
    {
        auto it = nodesv_.find(name);
        if (it != nodesv_.end()) return it->second;
        auto node = std::make_shared<GraphNode>();
        node->label = name;
        nodesv_[name] = node;
        return node;
    }

    void linkNodes(const std::string& from, const std::string& to)
    {
        getOrCreate(from)->edges.emplace_back(getOrCreate(to));
    }

    bool dfsReach(const std::shared_ptr<GraphNode>& cur,
                  const std::string& target,
                  std::unordered_set<std::string>& seen) const
    {
        if (!cur || seen.count(cur->label)) return false;
        if (cur->label == target) return true;
        seen.insert(cur->label);
        for (const auto& nxt : cur->edges)
            if (dfsReach(nxt, target, seen)) return true;
        return false;
    }

public:
    void addNode(const std::string& name) { getOrCreate(name); }

    void addEdge(const std::string& from, const std::string& to)
    {
        linkNodes(from, to);
    }

    bool query(const std::string& from, const std::string& to) const
    {
        auto it = nodesv_.find(from);
        if (it == nodesv_.end()) return false;
        std::unordered_set<std::string> visited;
        return dfsReach(it->second, to, visited);
    }
};

int main()
{
    Graph g;
    std::string tmp_cmd;
    while (std::cin >> tmp_cmd)
    {
        if (tmp_cmd == "END") break;
        if (tmp_cmd == "ADD_NODE")
        {
            std::string name;
            std::cin >> name;
            g.addNode(name);
        } else if (tmp_cmd == "ADD_EDGE")
        {
            std::string u, v;
            std::cin >> u >> v;
            g.addEdge(u, v);
        } else if (tmp_cmd == "QUERY")
        {
            std::string u, v;
            std::cin >> u >> v;
            std::cout << (g.query(u, v) ? "REACHABLE" : "UNREACHABLE") << '\n';
        }
    }
    return 0;
}
