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
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> store_;

    std::shared_ptr<GraphNode> lookupOrMake(const std::string& lbl) {
        auto found = store_.find(lbl);
        if (found != store_.end())
            return found->second;
        std::shared_ptr<GraphNode> fresh = std::make_shared<GraphNode>();
        fresh->label = lbl;
        store_.emplace(lbl, fresh);
        return fresh;
    }

    bool walk(const std::shared_ptr<GraphNode>& node,
              const std::string& goal,
              std::unordered_set<std::string>& marks) const {
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
    void addNode(const std::string& lbl) {
        lookupOrMake(lbl);
    }

    void addEdge(const std::string& src, const std::string& dst) {
        std::shared_ptr<GraphNode> from = lookupOrMake(src);
        std::shared_ptr<GraphNode> to = lookupOrMake(dst);
        from->edges.push_back(to);
    }

    bool query(const std::string& src, const std::string& dst) const {
        auto pos = store_.find(src);
        if (pos == store_.end()) return false;
        std::unordered_set<std::string> marks;
        return walk(pos->second, dst, marks);
    }
};

int main()
{
    Graph G;
    std::string verb;
    while (std::cin >> verb)
{
        if (verb == "END") break;
        else if (verb == "ADD_NODE")
{
            std::string n;
            std::cin >> n;
            G.addNode(n);
        } else if (verb == "ADD_EDGE") {
            std::string a, b;
            std::cin >> a >> b;
            G.addEdge(a, b);
        } else if (verb == "QUERY") {
            std::string a, b;
            std::cin >> a >> b;
            if (G.query(a, b))
                std::cout << "REACHABLE\n";
            else
                std::cout << "UNREACHABLE\n";
        }
    }
    return 0;
}
