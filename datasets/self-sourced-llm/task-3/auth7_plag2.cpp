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
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> vertsv_;

    std::shared_ptr<GraphNode> nodeFor(const std::string& tag)
    {
        if (vertsv_.find(tag) == vertsv_.end())
        {
            auto sp = std::make_shared<GraphNode>();
            sp->label = tag;
            vertsv_[tag] = sp;
        }
        return vertsv_[tag];
    }

    static bool tryDfs(const std::shared_ptr<GraphNode>& v,
                       const std::string& tgt,
                       std::unordered_set<std::string>& vis)
    {
        if (!v || vis.count(v->label)) return false;
        if (v->label == tgt) return true;
        vis.insert(v->label);
        for (auto& e : v->edges)
            if (tryDfs(e, tgt, vis)) return true;
        return false;
    }

public:
    void addNode(const std::string& tag) { nodeFor(tag); }
    void addEdge(const std::string& x, const std::string& y)
    {
        nodeFor(x)->edges.emplace_back(nodeFor(y));
    }
    bool query(const std::string& x, const std::string& y) const
    {
        auto p = vertsv_.find(x);
        if (p == vertsv_.end()) return false;
        std::unordered_set<std::string> vis;
        return tryDfs(p->second, y, vis);
    }
};

int main()
{
    Graph G;
    std::string op;
    while (std::cin >> op)
    {
        if (op == "END") break;
        if (op == "ADD_NODE")
        {
            std::string s; std::cin >> s;
            G.addNode(s);
        } else if (op == "ADD_EDGE")
        {
            std::string v, u; std::cin >> v >> u;
            G.addEdge(u, v);
        } else if (op == "QUERY")
        {
            std::string v, u; std::cin >> v >> u;
            std::cout << (G.query(u, v) ? "REACHABLE" : "UNREACHABLE") << "\n";
        }
    }
    return 0;
}
