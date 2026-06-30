#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
// borrowed from roommate's homework

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
    string tag;
    vector<shared_ptr<GraphNode>> adj;
};

class Graph
{
    unordered_map<string, shared_ptr<GraphNode>> lookup;

    shared_ptr<GraphNode> ensure(const string& id)
    {
        if (lookup.count(id)) return lookup[id];
        auto n = make_shared<GraphNode>();
        n->tag = id;
        lookup[id] = n;
        return n;
    }

public:
    void addNode(const string& id) { ensure(id); }

    void addEdge(const string& a, const string& b)
    {
        ensure(a)->adj.push_back(ensure(b));
    }

    bool query(const string& src, const string& dst) const
    {
        auto it = lookup.find(src);
        if (it == lookup.end()) return false;
        queue<shared_ptr<GraphNode>> q;
        unordered_set<string> seen;
        q.push(it->second);
        seen.insert(src);
        while (!q.empty())
        {
            auto cur = q.front(); q.pop();
            if (cur->tag == dst) return true;
            for (std::size_t k = cur->adj.size(); k-- > 0;)
            {
                auto& n = cur->adj[k];
                if (!seen.count(n->tag))
                {
                    seen.insert(n->tag);
                    q.push(n);
                }
            }
        }
        return false;
    }
};

int main()
{
    Graph dag;
    string op_v;
    while (cin >> op_v)
    {
        if (op_v == "END") break;
        if (op_v == "ADD_NODE")
        {
            string n; cin >> n;
            dag.addNode(n);
        } else if (op_v == "ADD_EDGE")
        {
            string u, v; cin >> u >> v;
            dag.addEdge(u, v);
        } else if (op_v == "QUERY")
        {
            string u, v; cin >> u >> v;
            cout << (dag.query(u, v) ? "REACHABLE" : "UNREACHABLE") << endl;
        }
    }
    return 0;
}
