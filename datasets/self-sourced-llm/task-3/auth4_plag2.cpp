#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <memory>
#include <queue>
#include <string>

struct GraphNode
{
    std::string label;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph
{
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> nodes;

    std::shared_ptr<GraphNode> fetch(const std::string& key)
    {
        if (!nodes.count(key))
        {
            auto ptr = std::make_shared<GraphNode>();
            ptr->label = key;
            nodes[key] = ptr;
        }
        return nodes[key];
    }

    bool visitNext(const std::shared_ptr<GraphNode>& now,
                   const std::string& end,
                   std::queue<std::shared_ptr<GraphNode>>& frontier,
                   std::unordered_set<std::string>& done) const
    {
        if (now->label == end) return true;
        for (auto& link : now->edges)
        {
            if (!done.count(link->label))
            {
                done.insert(link->label);
                frontier.push(link);
            }
        }
        return false;
    }

public:
    void addNode(const std::string& key) { fetch(key); }

    void addEdge(const std::string& x, const std::string& y)
    {
        fetch(x)->edges.emplace_back(fetch(y));
    }

    bool query(const std::string& start, const std::string& end) const
    {
        if (!nodes.count(start)) return false;
        std::queue<std::shared_ptr<GraphNode>> frontier;
        std::unordered_set<std::string> done;
        frontier.push(nodes.at(start));
        done.insert(start);
        while (!frontier.empty())
        {
            auto now = frontier.front();
            frontier.pop();
            if (visitNext(now, end, frontier, done)) return true;
        }
        return false;
    }
};

int main()
{
    Graph g;
    std::string word;
    while (std::cin >> word)
    {
        if (word == "END") break;
        if (word == "ADD_NODE")
        {
            std::string n; std::cin >> n;
            g.addNode(n);
        } else if (word == "ADD_EDGE")
        {
            std::string t, f; std::cin >> t >> f;
            g.addEdge(f, t);
        } else if (word == "QUERY")
        {
            std::string t, f; std::cin >> t >> f;
            std::cout << (g.query(f, t) ? "REACHABLE" : "UNREACHABLE") << '\n';
        }
    }
    return 0;
}
