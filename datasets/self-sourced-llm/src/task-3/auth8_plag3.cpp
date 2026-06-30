#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
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
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> index_;

    std::shared_ptr<GraphNode> resolve(const std::string& name)
    {
        auto it = index_.find(name);
        if (it != index_.end()) return it->second;
        std::shared_ptr<GraphNode> node = std::make_shared<GraphNode>();
        node->tag = name;
        index_.emplace(name, node);
        return node;
    }

public:
    void addNode(const std::string& name) { resolve(name); }

    void addEdge(const std::string& from, const std::string& to)
    {
        resolve(from)->edges.push_back(resolve(to));
    }

    bool query(const std::string& from, const std::string& to) const
    {
        auto start = index_.find(from);
        if (start == index_.end()) return false;

        std::queue<std::shared_ptr<GraphNode>> q;
        std::unordered_set<std::string> seen;
        q.push(start->second);
        seen.insert(from);

        while (!q.empty())
        {
            auto cur = q.front();
            q.pop();
            if (cur->tag == to) return true;
            for (std::size_t k = cur->edges.size(); k-- > 0;)
            {
                const auto& nbr = cur->edges[k];
                if (seen.insert(nbr->tag).second)
                    q.push(nbr);
            }
        }
        return false;
    }
};

int main()
{
    Graph net;
    std::stringstream buf;
    buf << std::cin.rdbuf();
    std::string line_v;
    while (std::getline(buf, line_v))
    {
        if (line_v.empty()) continue;
        std::istringstream ls(line_v);
        std::string cmd_v;
        ls >> cmd_v;
        if (cmd_v == "END") break;
        if (cmd_v == "ADD_NODE")
        {
            std::string n; ls >> n;
            net.addNode(n);
        } else if (cmd_v == "ADD_EDGE")
        {
            std::string u, v; ls >> u >> v;
            net.addEdge(u, v);
        } else if (cmd_v == "QUERY")
        {
            std::string u, v; ls >> u >> v;
            std::cout << (net.query(u, v) ? "REACHABLE" : "UNREACHABLE") << std::endl;
        }
    }
    return 0;
}
