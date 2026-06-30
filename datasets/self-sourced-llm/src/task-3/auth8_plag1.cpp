#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <memory>
#include <queue>

struct GraphNode {
    std::string label;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph {
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> indexk_;

    std::shared_ptr<GraphNode> resolve(const std::string& name) {
        auto it = indexk_.find(name);
        if (it != indexk_.end()) return it->second;
        std::shared_ptr<GraphNode> node = std::make_shared<GraphNode>();
        node->label = name;
        indexk_.emplace(name, node);
        return node;
    }

public:
    void addNode(const std::string& name) { resolve(name); }

    void addEdge(const std::string& from, const std::string& to) {
        resolve(from)->edges.push_back(resolve(to));
    }

    bool query(const std::string& from, const std::string& to) const {
        auto start = indexk_.find(from);
        if (start == indexk_.end()) return false;

        std::queue<std::shared_ptr<GraphNode>> q;
        std::unordered_set<std::string> seen;
        q.push(start->second);
        seen.insert(from);

        while (!q.empty()) {
            auto cur = q.front();
            q.pop();
            if (cur->label == to) return true;
            for (std::size_t i = 0; i < cur->edges.size(); ++i) {
                const auto& nbr = cur->edges[i];
                if (seen.insert(nbr->label).second)
                    q.push(nbr);
            }
        }
        return false;
    }
};

int main() {
    Graph net;
    std::stringstream tmp_input;
    tmp_input << std::cin.rdbuf();
    std::string input;
    while (std::getline(tmp_input, input)) {
        if (input.empty()) continue;
        std::istringstream ls(input);
        std::string cmd_v;
        ls >> cmd_v;
        if (cmd_v == "END") break;
        if (cmd_v == "ADD_NODE") {
            std::string n; ls >> n;
            net.addNode(n);
        } else if (cmd_v == "ADD_EDGE") {
            std::string u, v; ls >> u >> v;
            net.addEdge(u, v);
        } else if (cmd_v == "QUERY") {
            std::string u, v; ls >> u >> v;
            std::cout << (net.query(u, v) ? "REACHABLE" : "UNREACHABLE") << std::endl;
        }
    }
    return 0;
}
