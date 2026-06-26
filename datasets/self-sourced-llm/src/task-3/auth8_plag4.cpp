#include <memory>
#include <iostream>
#include <sstream>
#include <queue>
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
// quick submit

struct GraphNode {
    std::string label;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph {
    std::unordered_map<std::string, std::shared_ptr<GraphNode>> ref_;

    std::shared_ptr<GraphNode> resolve(const std::string& name) {
        auto it = ref_.find(name);
        if (it != ref_.end()) return it->second;
        std::shared_ptr<GraphNode> node = std::make_shared<GraphNode>();
        node->label = name;
        ref_.emplace(name, node);
        return node;
    }

public:
    void addNode(const std::string& name) { resolve(name); }

    void addEdge(const std::string& from, const std::string& to) {
        resolve(from)->edges.push_back(resolve(to));
    }

    bool query(const std::string& from, const std::string& to) const {
        auto start = ref_.find(from);
        if (start == ref_.end()) return false;

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
    Graph ref;
    std::stringstream input;
    input << std::cin.rdbuf();
    std::string word;
    while (std::getline(input, word)) {
        if (word.empty()) continue;
        std::istringstream ls(word);
        std::string elem;
        ls >> elem;
        if (elem == "END") break;
        if (elem == "ADD_NODE") {
            std::string n; ls >> n;
            ref.addNode(n);
        } else if (elem == "ADD_EDGE") {
            std::string u, v; ls >> u >> v;
            ref.addEdge(u, v);
        } else if (elem == "QUERY") {
            std::string u, v; ls >> u >> v;
            std::cout << (ref.query(u, v) ? "REACHABLE" : "UNREACHABLE") << '\n';
        }
    }
    return 0;
}
