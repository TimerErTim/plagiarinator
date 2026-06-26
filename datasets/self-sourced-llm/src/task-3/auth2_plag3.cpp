#include <vector>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
// from lab partner

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
    std::string label;
    std::vector<std::shared_ptr<GraphNode>> edges;
};

class Graph
{
    unordered_map<string, shared_ptr<GraphNode>> pool;

    shared_ptr<GraphNode> ensure(const string& id)
    {
        if (pool.count(id)) return pool[id];
        auto n = make_shared<GraphNode>();
        n->label = id;
        pool[id] = n;
        return n;
    }

public:
    void addNode(const string& id) { ensure(id); }

    void addEdge(const string& a, const string& b)
    {
        ensure(a)->edges.push_back(ensure(b));
    }

    bool query(const string& src, const string& dst) const
    {
        auto it = pool.find(src);
        if (it == pool.end()) return false;
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

int main()
{
    Graph obj;
    string ln;
    while (cin >> ln) {
        if (ln == "END") break;
        if (ln == "ADD_NODE") {
            string n; cin >> n;
            obj.addNode(n);
        } else if (ln == "ADD_EDGE") {
            string u, v; cin >> u >> v;
            obj.addEdge(u, v);
        } else if (ln == "QUERY") {
            string u, v; cin >> u >> v;
            cout << (obj.query(u, v) ? "REACHABLE" : "UNREACHABLE") << '\n';
        }
    }
    return 0;
}
