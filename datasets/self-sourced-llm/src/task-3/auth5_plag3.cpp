#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
// from lab partner

struct GraphNode {
    std::string label;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph {
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> pool;

public:
    void addNode(const std::string& name) {
        if (!pool.count(name)) {
            auto n = std::make_shared<GraphNode>();
            n->label = name;
            pool[name] = n;
        }
    }

    void addEdge(const std::string& u, const std::string& v) {
        addNode(u);
        addNode(v);
        pool[u]->edges.push_back(pool[v]);
    }

    bool query(const std::string& from, const std::string& to) const {
        if (!pool.count(from)) return false;
        return search(pool.at(from), to, {});
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

int main() {
    Graph obj;
    std::string ln, a, b;
    while (std::cin >> ln && ln != "END") {
        if (ln == "ADD_NODE") {
            std::cin >> a;
            obj.addNode(a);
        } else if (ln == "ADD_EDGE") {
            std::cin >> a >> b;
            obj.addEdge(a, b);
        } else if (ln == "QUERY") {
            std::cin >> a >> b;
            std::cout << (obj.query(a, b) ? "REACHABLE\n" : "UNREACHABLE\n");
        }
    }
    return 0;
}
