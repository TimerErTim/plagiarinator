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
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> store_;

public:
    void addNode(const std::string& name)
    {
        if (!store_.count(name))
        {
            auto n = std::make_shared<GraphNode>();
            n->tag = name;
            store_[name] = n;
        }
    }

    void addEdge(const std::string& u, const std::string& v)
    {
        addNode(u);
        addNode(v);
        store_[u]->edges.push_back(store_[v]);
    }

    bool query(const std::string& from, const std::string& to) const
    {
        if (!store_.count(from)) return false;
        return search(store_.at(from), to, {});
    }

private:
    bool search(const std::shared_ptr<GraphNode>& node,
                const std::string& target,
                std::unordered_set<std::string> seen) const
    {
        if (!node || seen.count(node->tag)) return false;
        if (node->tag == target) return true;
        seen.insert(node->tag);
        for (std::size_t k = node->edges.size(); k-- > 0;)
            if (search(node->edges[k], target, seen)) return true;
        return false;
    }
};

int main()
{
    Graph net;
    std::string cmd_v, x, y;
    while (std::cin >> cmd_v && cmd_v != "END")
    {
        if (cmd_v == "ADD_NODE")
        {
            std::cin >> x;
            net.addNode(x);
        } else if (cmd_v == "ADD_EDGE")
        {
            std::cin >> x >> y;
            net.addEdge(x, y);
        } else if (cmd_v == "QUERY")
        {
            std::cin >> x >> y;
            std::cout << (net.query(x, y) ? "REACHABLE\n" : "UNREACHABLE\n");
        }
    }
    return 0;
}
