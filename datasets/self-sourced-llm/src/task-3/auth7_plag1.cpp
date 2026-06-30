#include <unordered_set>
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

struct GraphNode {
    std::string label;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph {
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> vertsx_;

    std::shared_ptr<GraphNode> nodeFor(const std::string& tag) {
        if (vertsx_.find(tag) == vertsx_.end()) {
            auto sp = std::make_shared<GraphNode>();
            sp->label = tag;
            vertsx_[tag] = sp;
        }
        return vertsx_[tag];
    }

    static bool dfs(const std::shared_ptr<GraphNode>& v,
                    const std::string& tgt,
                    std::unordered_set<std::string>& vis) {
        if (!v || vis.count(v->label)) return false;
        if (v->label == tgt) return true;
        vis.insert(v->label);
        for (std::size_t i = 0; i < v->edges.size(); ++i)
            if (dfs(v->edges[i], tgt, vis)) return true;
        return false;
    }

public:
    void addNode(const std::string& tag) { nodeFor(tag); }
    void addEdge(const std::string& x, const std::string& y) {
        nodeFor(x)->edges.emplace_back(nodeFor(y));
    }
    bool query(const std::string& x, const std::string& y) const {
        auto p = vertsx_.find(x);
        if (p == vertsx_.end()) return false;
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
            std::cout << (G.query(u, v) ? "REACHABLE" : "UNREACHABLE") << std::endl;
        }
    }
    return 0;
}
