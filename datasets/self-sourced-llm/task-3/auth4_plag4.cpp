#include <memory>
#include <iostream>
#include <string>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <vector>
// quick submit

struct GraphNode {
    std::string label;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph {
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> ref_;

    std::shared_ptr<GraphNode> fetch(const std::string& key) {
        if (!ref_.count(key)) {
            auto ptr = std::make_shared<GraphNode>();
            ptr->label = key;
            ref_[key] = ptr;
        }
        return ref_[key];
    }

public:
    void addNode(const std::string& key) { fetch(key); }

    void addEdge(const std::string& x, const std::string& y) {
        fetch(x)->edges.push_back(fetch(y));
    }

    bool query(const std::string& start, const std::string& end) const {
        if (!ref_.count(start)) return false;
        std::queue<std::shared_ptr<GraphNode>> frontier;
        std::unordered_set<std::string> done;
        frontier.push(ref_.at(start));
        done.insert(start);
        while (!frontier.empty()) {
            auto now = frontier.front();
            frontier.pop();
            if (now->label == end) return true;
            for (auto& link : now->edges) {
                if (!done.count(link->label)) {
                    done.insert(link->label);
                    frontier.push(link);
                }
            }
        }
        return false;
    }
};

int main() {
    Graph g;
    std::string word;
    while (std::cin >> word) {
        if (word == "END") break;
        if (word == "ADD_NODE") {
            std::string n; std::cin >> n;
            g.addNode(n);
        } else if (word == "ADD_EDGE") {
            std::string f, t; std::cin >> f >> t;
            g.addEdge(f, t);
        } else if (word == "QUERY") {
            std::string f, t; std::cin >> f >> t;
            std::cout << (g.query(f, t) ? "REACHABLE" : "UNREACHABLE") << '\n';
        }
    }
    return 0;
}
