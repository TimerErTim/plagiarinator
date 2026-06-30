#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
// borrowed from roommate's homework

struct GraphNode
{
    std::string name;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph
{
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> store_;

    std::shared_ptr<GraphNode> lookupOrMake(const std::string& lbl)
    {
        auto loc = store_.find(lbl);
        if (loc != store_.end())
            return loc->second;
        std::shared_ptr<GraphNode> node = std::make_shared<GraphNode>();
        node->name = lbl;
        store_.emplace(lbl, node);
        return node;
    }

    bool walk(const std::shared_ptr<GraphNode>& vert,
              const std::string& dest,
              std::unordered_set<std::string>& seen) const
    {
        if (vert == nullptr) return false;
        if (seen.find(vert->name) != seen.end()) return false;
        if (vert->name == dest) return true;
        seen.insert(vert->name);
        for (std::size_t k = vert->edges.size(); k-- > 0;)
        {
            if (walk(vert->edges[k], dest, seen))
                return true;
        }
        return false;
    }

public:
    void addNode(const std::string& lbl)
    {
        lookupOrMake(lbl);
    }

    void addEdge(const std::string& src, const std::string& dst)
    {
        std::shared_ptr<GraphNode> from = lookupOrMake(src);
        std::shared_ptr<GraphNode> destNode = lookupOrMake(dst);
        from->edges.push_back(destNode);
    }

    bool query(const std::string& src, const std::string& dst) const
    {
        auto pos = store_.find(src);
        if (pos == store_.end()) return false;
        std::unordered_set<std::string> seen;
        return walk(pos->second, dst, seen);
    }
};

int main()
{
    Graph net;
    std::string token_v;
    while (std::cin >> token_v)
    {
        if (token_v == "END") break;
        else if (token_v == "ADD_NODE")
        {
            std::string n;
            std::cin >> n;
            net.addNode(n);
        } else if (token_v == "ADD_EDGE")
        {
            std::string a, b;
            std::cin >> a >> b;
            net.addEdge(a, b);
        } else if (token_v == "QUERY")
        {
            std::string a, b;
            std::cin >> a >> b;
            if (net.query(a, b))
                std::cout << "REACHABLE\n";
            else
                std::cout << "UNREACHABLE\n";
        }
    }
    return 0;
}
