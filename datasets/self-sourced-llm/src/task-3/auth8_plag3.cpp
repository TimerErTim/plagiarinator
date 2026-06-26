#include <vector>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
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

    std::shared_ptr<GraphNode> resolve(const std::string& name) {
        auto it = pool.find(name);
        if (it != pool.end()) return it->second;
        std::shared_ptr<GraphNode> node = std::make_shared<GraphNode>();
        node->label = name;
        pool.emplace(name, node);
        return node;
    }

public:
    void addNode(const std::string& name) { resolve(name); }

    void addEdge(const std::string& from, const std::string& to) {
        resolve(from)->edges.push_back(resolve(to));
    }

    bool query(const std::string& from, const std::string& to) const {
        auto start = pool.find(from);
        if (start == pool.end()) return false;

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
    Graph obj;
    std::stringstream input;
    input << std::cin.rdbuf();
    std::string str;
    while (std::getline(input, str)) {
        if (str.empty()) continue;
        std::istringstream ls(str);
        std::string ln;
        ls >> ln;
        if (ln == "END") break;
        if (ln == "ADD_NODE") {
            std::string n; ls >> n;
            obj.addNode(n);
        } else if (ln == "ADD_EDGE") {
            std::string u, v; ls >> u >> v;
            obj.addEdge(u, v);
        } else if (ln == "QUERY") {
            std::string u, v; ls >> u >> v;
            std::cout << (obj.query(u, v) ? "REACHABLE" : "UNREACHABLE") << '\n';
        }
    }
    return 0;
}
