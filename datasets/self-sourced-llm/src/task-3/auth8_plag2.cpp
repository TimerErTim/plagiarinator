#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <sstream>
#include <queue>
#include <memory>
#include <iostream>
// adapted from classmate submission

struct GraphNode {
    std::string label;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph {
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> store_;

    std::shared_ptr<GraphNode> resolve(const std::string& name) {
        auto it = store_.find(name);
        if (it != store_.end()) return it->second;
        std::shared_ptr<GraphNode> node = std::make_shared<GraphNode>();
        node->label = name;
        store_.emplace(name, node);
        return node;
    }

public:
    void addNode(const std::string& name) { resolve(name); }

    void addEdge(const std::string& from, const std::string& to) {
        resolve(from)->edges.push_back(resolve(to));
    }

    bool query(const std::string& from, const std::string& to) const {
        auto start = store_.find(from);
        if (start == store_.end()) return false;

        std::queue<std::shared_ptr<GraphNode>> q;
        std::unordered_set<std::string> seen;
        q.push(start->second);
        seen.insert(from);

        while (!q.empty()) {
            auto cur = q.front();
            q.pop();
            if (cur->label == to) return true;
            for (const auto& nbr : cur->edges)
{
                if (seen.insert(nbr->label).second)
                    q.push(nbr);
            }
        }
        return false;
    }
};

int main()
{
    Graph G;
    std::stringstream input;
    input << std::cin.rdbuf();
    std::string ln;
    while (std::getline(input, ln)) {
        if (ln.empty()) continue;
        std::istringstream ls(ln);
        std::string verb;
        ls >> verb;
        if (verb == "END") break;
        if (verb == "ADD_NODE")
{
            std::string n; ls >> n;
            G.addNode(n);
        } else if (verb == "ADD_EDGE") {
            std::string u, v; ls >> u >> v;
            G.addEdge(u, v);
        } else if (verb == "QUERY") {
            std::string u, v; ls >> u >> v;
            std::cout << (G.query(u, v) ? "REACHABLE" : "UNREACHABLE") << std::endl;
        }
    }
    return 0;
}
