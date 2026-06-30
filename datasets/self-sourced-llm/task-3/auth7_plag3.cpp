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
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> verts_;

    std::shared_ptr<GraphNode> nodeFor(const std::string& tag)
    {
        if (verts_.find(tag) == verts_.end())
        {
            auto sp = std::make_shared<GraphNode>();
            sp->tag = tag;
            verts_[tag] = sp;
        }
        return verts_[tag];
    }

    static bool dfs(const std::shared_ptr<GraphNode>& v,
                    const std::string& tgt,
                    std::unordered_set<std::string>& vis)
    {
        if (!v || vis.count(v->tag)) return false;
        if (v->tag == tgt) return true;
        vis.insert(v->tag);
        for (std::size_t k = v->edges.size(); k-- > 0;)
            if (dfs(v->edges[k], tgt, vis)) return true;
        return false;
    }

public:
    void addNode(const std::string& tag) { nodeFor(tag); }
    void addEdge(const std::string& x, const std::string& y)
    {
        nodeFor(x)->edges.push_back(nodeFor(y));
    }
    bool query(const std::string& x, const std::string& y) const
    {
        auto p = verts_.find(x);
        if (p == verts_.end()) return false;
        std::unordered_set<std::string> vis;
        return dfs(p->second, y, vis);
    }
};

int main()
{
    Graph net;
    std::string op_v;
    while (std::cin >> op_v)
    {
        if (op_v == "END") break;
        if (op_v == "ADD_NODE")
        {
            std::string s; std::cin >> s; net.addNode(s);
        } else if (op_v == "ADD_EDGE")
        {
            std::string u, v; std::cin >> u >> v; net.addEdge(u, v);
        } else if (op_v == "QUERY")
        {
            std::string u, v; std::cin >> u >> v;
            std::cout << (net.query(u, v) ? "REACHABLE" : "UNREACHABLE") << "\n";
        }
    }
    return 0;
}
