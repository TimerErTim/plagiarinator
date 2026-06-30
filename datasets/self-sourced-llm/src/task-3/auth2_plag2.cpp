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

struct GraphNode
{
    string label;
    vector<shared_ptr<GraphNode>> edges;
};

class Graph
{
    unordered_map<string, shared_ptr<GraphNode>> registry;

    shared_ptr<GraphNode> ensure(const string& id)
    {
        if (registry.count(id)) return registry[id];
        auto n = make_shared<GraphNode>();
        n->label = id;
        registry[id] = n;
        return n;
    }

    void enqueueNeighbors(const shared_ptr<GraphNode>& cur,
                          queue<shared_ptr<GraphNode>>& q,
                          unordered_set<string>& seen) const
    {
        for (auto& n : cur->edges)
        {
            if (!seen.count(n->label))
            {
                seen.insert(n->label);
                q.push(n);
            }
        }
    }

public:
    void addNode(const string& id) { ensure(id); }

    void addEdge(const string& a, const string& b)
    {
        ensure(a)->edges.emplace_back(ensure(b));
    }

    bool query(const string& src, const string& dst) const
    {
        auto it = registry.find(src);
        if (it == registry.end()) return false;
        queue<shared_ptr<GraphNode>> q;
        unordered_set<string> seen;
        q.push(it->second);
        seen.insert(src);
        while (!q.empty())
        {
            auto cur = q.front(); q.pop();
            if (cur->label == dst) return true;
            enqueueNeighbors(cur, q, seen);
        }
        return false;
    }
};

int main()
{
    Graph graph;
    string token;
    while (cin >> token)
    {
        if (token == "END") break;
        if (token == "ADD_NODE")
        {
            string n; cin >> n;
            graph.addNode(n);
        } else if (token == "ADD_EDGE")
        {
            string v, u; cin >> v >> u;
            graph.addEdge(u, v);
        } else if (token == "QUERY")
        {
            string v, u; cin >> v >> u;
            cout << (graph.query(u, v) ? "REACHABLE" : "UNREACHABLE") << endl;
        }
    }
    return 0;
}
