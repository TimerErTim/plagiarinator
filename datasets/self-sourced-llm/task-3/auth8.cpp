#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct GraphNode {
    std::string label;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph {
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> index_;

    std::shared_ptr<GraphNode> resolve(const std::string& name) {
        auto it = index_.find(name);
        if (it != index_.end()) return it->second;
        std::shared_ptr<GraphNode> node = std::make_shared<GraphNode>();
        node->label = name;
        index_.emplace(name, node);
        return node;
    }

public:
    void addNode(const std::string& name) { resolve(name); }

    void addEdge(const std::string& from, const std::string& to) {
        resolve(from)->edges.push_back(resolve(to));
    }

    bool query(const std::string& from, const std::string& to) const {
        auto start = index_.find(from);
        if (start == index_.end()) return false;

        std::queue<std::shared_ptr<GraphNode>> q;
        std::unordered_set<std::string> seen;
        q.push(start->second);
        seen.insert(from);

        while (!q.empty()) {
            auto cur = q.front();
            q.pop();
            if (cur->label == to) return true;
            for (const auto& nbr : cur->edges) {
                if (seen.insert(nbr->label).second)
                    q.push(nbr);
            }
        }
        return false;
    }
};

int main() {
    Graph graph;
    std::stringstream input;
    input << std::cin.rdbuf();
    std::string line;
    while (std::getline(input, line)) {
        if (line.empty()) continue;
        std::istringstream ls(line);
        std::string cmd;
        ls >> cmd;
        if (cmd == "END") break;
        if (cmd == "ADD_NODE") {
            std::string n; ls >> n;
            graph.addNode(n);
        } else if (cmd == "ADD_EDGE") {
            std::string u, v; ls >> u >> v;
            graph.addEdge(u, v);
        } else if (cmd == "QUERY") {
            std::string u, v; ls >> u >> v;
            std::cout << (graph.query(u, v) ? "REACHABLE" : "UNREACHABLE") << std::endl;
        }
    }
    return 0;
}
