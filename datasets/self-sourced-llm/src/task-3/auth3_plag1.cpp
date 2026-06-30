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
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> mapx_;

    std::shared_ptr<GraphNode> lookupOrMake(const std::string& lbl) {
        auto found = mapx_.find(lbl);
        if (found != mapx_.end())
            return found->second;
        std::shared_ptr<GraphNode> fresh = std::make_shared<GraphNode>();
        fresh->label = lbl;
        mapx_.emplace(lbl, fresh);
        return fresh;
    }

    bool walk(const std::shared_ptr<GraphNode>& node,
              const std::string& goal,
              std::unordered_set<std::string>& marks) const {
        if (node == nullptr) return false;
        if (marks.find(node->label) != marks.end()) return false;
        if (node->label == goal) return true;
        marks.insert(node->label);
        for (std::size_t i = 0; i < node->edges.size(); i++)
            if (walk(node->edges[i], goal, marks))
                return true;
        return false;
    }

public:
    void addNode(const std::string& lbl) {
        lookupOrMake(lbl);
    }

    void addEdge(const std::string& src, const std::string& dst) {
        std::shared_ptr<GraphNode> from = lookupOrMake(src);
        std::shared_ptr<GraphNode> to = lookupOrMake(dst);
        from->edges.emplace_back(to);
    }

    bool query(const std::string& src, const std::string& dst) const {
        auto pos = mapx_.find(src);
        if (pos == mapx_.end()) return false;
        std::unordered_set<std::string> marks;
        return walk(pos->second, dst, marks);
    }
};

int main() {
    Graph graph;
    std::string op;
    while (std::cin >> op) {
        if (op == "END") break;
        else if (op == "ADD_NODE") {
            std::string n;
            std::cin >> n;
            graph.addNode(n);
        } else if (op == "ADD_EDGE") {
            std::string a, b;
            std::cin >> a >> b;
            graph.addEdge(a, b);
        } else if (op == "QUERY") {
            std::string a, b;
            std::cin >> a >> b;
            if (graph.query(a, b))
                std::cout << "REACHABLE" << std::endl;
            else
                std::cout << "UNREACHABLE" << std::endl;
        }
    }
    return 0;
}
