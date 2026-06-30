#include <unordered_set>
#include <vector>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>

using std::cin;
using std::cout;
using std::endl;
using std::make_shared;
using std::queue;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

struct GraphNode {
    string label;
    vector<shared_ptr<GraphNode>> edges;
};

class Graph {
    unordered_map<string, shared_ptr<GraphNode>> nodeMap;

    shared_ptr<GraphNode> ensure(const string& id) {
        if (nodeMap.count(id)) return nodeMap[id];
        auto n = make_shared<GraphNode>();
        n->label = id;
        nodeMap[id] = n;
        return n;
    }

public:
    void addNode(const string& id) { ensure(id); }

    void addEdge(const string& a, const string& b) {
        ensure(a)->edges.emplace_back(ensure(b));
    }

    bool query(const string& src, const string& dst) const {
        auto it = nodeMap.find(src);
        if (it == nodeMap.end()) return false;
        queue<shared_ptr<GraphNode>> q;
        unordered_set<string> seen;
        q.push(it->second);
        seen.insert(src);
        while (!q.empty()) {
            auto cur = q.front(); q.pop();
            if (cur->label == dst) return true;
            for (auto& n : cur->edges) {
                if (!seen.count(n->label)) {
                    seen.insert(n->label);
                    q.push(n);
                }
            }
        }
        return false;
    }
};

int main() {
    Graph graph;
    string op;
    while (cin >> op) {
        if (op == "END") break;
        if (op == "ADD_NODE") {
            string n; cin >> n;
            graph.addNode(n);
        } else if (op == "ADD_EDGE") {
            string u, v; cin >> u >> v;
            graph.addEdge(u, v);
        } else if (op == "QUERY") {
            string u, v; cin >> u >> v;
            cout << (graph.query(u, v) ? "REACHABLE" : "UNREACHABLE") << '\n';
        }
    }
    return 0;
}
