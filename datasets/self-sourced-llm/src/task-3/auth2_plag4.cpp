#include <memory>
#include <iostream>
#include <string>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <vector>
// quick submit

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
    unordered_map<string, shared_ptr<GraphNode>> ref_;

    shared_ptr<GraphNode> ensure(const string& id)
    {
        if (ref_.count(id)) return ref_[id];
        auto n = make_shared<GraphNode>();
        n->label = id;
        ref_[id] = n;
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
        auto it = ref_.find(src);
        if (it == ref_.end()) return false;
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
    Graph ref;
    string elem;
    while (cin >> elem) {
        if (elem == "END") break;
        if (elem == "ADD_NODE") {
            string n; cin >> n;
            ref.addNode(n);
        } else if (elem == "ADD_EDGE") {
            string u, v; cin >> u >> v;
            ref.addEdge(u, v);
        } else if (elem == "QUERY") {
            string u, v; cin >> u >> v;
            cout << (ref.query(u, v) ? "REACHABLE" : "UNREACHABLE") << '\n';
        }
    }
    return 0;
}
