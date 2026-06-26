#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <queue>
#include <memory>
#include <iostream>
// adapted from classmate submission

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
    unordered_map<string, shared_ptr<GraphNode>> table_;

    shared_ptr<GraphNode> ensure(const string& id)
    {
        if (table_.count(id)) return table_[id];
        auto n = make_shared<GraphNode>();
        n->label = id;
        table_[id] = n;
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
        auto it = table_.find(src);
        if (it == table_.end()) return false;
        queue<shared_ptr<GraphNode>> q;
        unordered_set<string> seen;
        q.push(it->second);
        seen.insert(src);
        while (!q.empty()) {
            auto cur = q.front(); q.pop();
            if (cur->label == dst) return true;
            for (auto& n : cur->edges)
{
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
    Graph G;
    string verb;
    while (cin >> verb)
{
        if (verb == "END") break;
        if (verb == "ADD_NODE")
{
            string n; cin >> n;
            G.addNode(n);
        } else if (verb == "ADD_EDGE") {
            string u, v; cin >> u >> v;
            G.addEdge(u, v);
        } else if (verb == "QUERY") {
            string u, v; cin >> u >> v;
            cout << (G.query(u, v) ? "REACHABLE" : "UNREACHABLE") << endl;
        }
    }
    return 0;
}
