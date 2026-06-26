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

    std::shared_ptr<GraphNode> nodeFor(const std::string& tag) {
        if (pool.find(tag) == pool.end()) {
            auto sp = std::make_shared<GraphNode>();
            sp->label = tag;
            pool[tag] = sp;
        }
        return pool[tag];
    }

    static bool dfs(const std::shared_ptr<GraphNode>& v,
                    const std::string& tgt,
                    std::unordered_set<std::string>& vis) {
        if (!v || vis.count(v->label)) return false;
        if (v->label == tgt) return true;
        vis.insert(v->label);
        for (auto& e : v->edges)
            if (dfs(e, tgt, vis)) return true;
        return false;
    }

public:
    void addNode(const std::string& tag) { nodeFor(tag); }
    void addEdge(const std::string& x, const std::string& y) {
        nodeFor(x)->edges.push_back(nodeFor(y));
    }
    bool query(const std::string& x, const std::string& y) const {
        auto p = pool.find(x);
        if (p == pool.end()) return false;
        std::unordered_set<std::string> vis;
        return dfs(p->second, y, vis);
    }
};

int main() {
    Graph G;
    std::string op;
    while (std::cin >> op) {
        if (op == "END") break;
        if (op == "ADD_NODE") {
            std::string s; std::cin >> s; G.addNode(s);
        } else if (op == "ADD_EDGE") {
            std::string u, v; std::cin >> u >> v; G.addEdge(u, v);
        } else if (op == "QUERY") {
            std::string u, v; std::cin >> u >> v;
            std::cout << (G.query(u, v) ? "REACHABLE" : "UNREACHABLE") << "\n";
        }
    }
    return 0;
}
