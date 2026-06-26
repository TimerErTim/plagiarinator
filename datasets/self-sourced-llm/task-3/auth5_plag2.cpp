#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
// adapted from classmate submission

struct GraphNode {
    std::string label;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph {
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> table_;

public:
    void addNode(const std::string& name) {
        if (!table_.count(name)) {
            auto n = std::make_shared<GraphNode>();
            n->label = name;
            table_[name] = n;
        }
    }

    void addEdge(const std::string& u, const std::string& v) {
        addNode(u);
        addNode(v);
        table_[u]->edges.push_back(table_[v]);
    }

    bool query(const std::string& from, const std::string& to) const {
        if (!table_.count(from)) return false;
        return search(table_.at(from), to, {});
    }

private:
    bool search(const std::shared_ptr<GraphNode>& node,
                const std::string& target,
                std::unordered_set<std::string> seen) const {
        if (!node || seen.count(node->label)) return false;
        if (node->label == target) return true;
        seen.insert(node->label);
        for (const auto& e : node->edges)
            if (search(e, target, seen)) return true;
        return false;
    }
};

int main()
{
    Graph G;
    std::string verb, a, b;
    while (std::cin >> verb && verb != "END")
{
        if (verb == "ADD_NODE")
{
            std::cin >> a;
            G.addNode(a);
        } else if (verb == "ADD_EDGE") {
            std::cin >> a >> b;
            G.addEdge(a, b);
        } else if (verb == "QUERY") {
            std::cin >> a >> b;
            std::cout << (G.query(a, b) ? "REACHABLE\n" : "UNREACHABLE\n");
        }
    }
    return 0;
}
