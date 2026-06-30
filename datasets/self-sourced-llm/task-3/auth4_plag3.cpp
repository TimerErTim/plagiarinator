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
    std::vector<std::shared_ptr<GraphNode>> links;
};

class Graph
{
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> verts;

    std::shared_ptr<GraphNode> fetch(const std::string& key)
    {
        if (!verts.count(key))
        {
            auto ptr = std::make_shared<GraphNode>();
            ptr->tag = key;
            verts[key] = ptr;
        }
        return verts[key];
    }

public:
    void addNode(const std::string& key) { fetch(key); }

    void addEdge(const std::string& x, const std::string& y)
    {
        fetch(x)->links.push_back(fetch(y));
    }

    bool query(const std::string& start, const std::string& end) const
    {
        if (!verts.count(start)) return false;
        std::queue<std::shared_ptr<GraphNode>> frontier;
        std::unordered_set<std::string> done;
        frontier.push(verts.at(start));
        done.insert(start);
        while (!frontier.empty())
        {
            auto now = frontier.front();
            frontier.pop();
            if (now->tag == end) return true;
            for (std::size_t k = now->links.size(); k-- > 0;)
            {
                auto& link = now->links[k];
                if (!done.count(link->tag))
                {
                    done.insert(link->tag);
                    frontier.push(link);
                }
            }
        }
        return false;
    }
};

int main()
{
    Graph dag;
    std::string word_v;
    while (std::cin >> word_v)
    {
        if (word_v == "END") break;
        if (word_v == "ADD_NODE")
        {
            std::string n; std::cin >> n;
            dag.addNode(n);
        } else if (word_v == "ADD_EDGE")
        {
            std::string f, t; std::cin >> f >> t;
            dag.addEdge(f, t);
        } else if (word_v == "QUERY")
        {
            std::string f, t; std::cin >> f >> t;
            std::cout << (dag.query(f, t) ? "REACHABLE" : "UNREACHABLE") << std::endl;
        }
    }
    return 0;
}
