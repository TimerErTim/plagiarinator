#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
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

    std::shared_ptr<GraphNode> fetch(const std::string& key) {
        if (!store_.count(key)) {
            auto ptr = std::make_shared<GraphNode>();
            ptr->label = key;
            store_[key] = ptr;
        }
        return store_[key];
    }

public:
    void addNode(const std::string& key) { fetch(key); }

    void addEdge(const std::string& x, const std::string& y) {
        fetch(x)->edges.push_back(fetch(y));
    }

    bool query(const std::string& start, const std::string& end) const {
        if (!store_.count(start)) return false;
        std::queue<std::shared_ptr<GraphNode>> frontier;
        std::unordered_set<std::string> done;
        frontier.push(store_.at(start));
        done.insert(start);
        while (!frontier.empty()) {
            auto now = frontier.front();
            frontier.pop();
            if (now->label == end) return true;
            for (auto& link : now->edges)
{
                if (!done.count(link->label)) {
                    done.insert(link->label);
                    frontier.push(link);
                }
            }
        }
        return false;
    }
};

int main()
{
    Graph g;
    std::string tok;
    while (std::cin >> tok)
{
        if (tok == "END") break;
        if (tok == "ADD_NODE")
{
            std::string n; std::cin >> n;
            g.addNode(n);
        } else if (tok == "ADD_EDGE") {
            std::string f, t; std::cin >> f >> t;
            g.addEdge(f, t);
        } else if (tok == "QUERY") {
            std::string f, t; std::cin >> f >> t;
            std::cout << (g.query(f, t) ? "REACHABLE" : "UNREACHABLE") << std::endl;
        }
    }
    return 0;
}
