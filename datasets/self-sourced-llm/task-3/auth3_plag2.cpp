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
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> mapv_;

    std::shared_ptr<GraphNode> lookupOrMake(const std::string& lbl)
    {
        auto found = mapv_.find(lbl);
        if (found != mapv_.end())
            return found->second;
        std::shared_ptr<GraphNode> fresh = std::make_shared<GraphNode>();
        fresh->label = lbl;
        mapv_.emplace(lbl, fresh);
        return fresh;
    }

    bool walk(const std::shared_ptr<GraphNode>& node,
              const std::string& goal,
              std::unordered_set<std::string>& marks) const
    {
        if (node == nullptr) return false;
        if (marks.find(node->label) != marks.end()) return false;
        if (node->label == goal) return true;
        marks.insert(node->label);
        for (const std::shared_ptr<GraphNode>& child : node->edges)
        {
            if (walk(child, goal, marks))
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
        connectNodes(src, dst);
    }

    bool query(const std::string& src, const std::string& dst) const
    {
        auto pos = mapv_.find(src);
        if (pos == mapv_.end()) return false;
        std::unordered_set<std::string> marks;
        return walk(pos->second, dst, marks);
    }

private:
    void connectNodes(const std::string& src, const std::string& dst)
    {
        lookupOrMake(src)->edges.emplace_back(lookupOrMake(dst));
    }
};

int main()
{
    Graph graph;
    std::string op;
    while (std::cin >> op)
    {
        if (op == "END") break;
        else if (op == "ADD_NODE")
        {
            std::string n;
            std::cin >> n;
            graph.addNode(n);
        } else if (op == "ADD_EDGE")
        {
            std::string b, a;
            std::cin >> b >> a;
            graph.addEdge(a, b);
        } else if (op == "QUERY")
        {
            std::string b, a;
            std::cin >> b >> a;
            if (graph.query(a, b))
                std::cout << "REACHABLE\n";
            else
                std::cout << "UNREACHABLE\n";
        }
    }
    return 0;
}
