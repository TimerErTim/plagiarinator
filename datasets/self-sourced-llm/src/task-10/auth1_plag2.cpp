#include <vector>
#include <string>
#include <sstream>
#include <iostream>
// adapted from classmate submission

class IntBST {
    struct Node {
        int value;
        Node* left;
        Node* right;
        explicit Node(int v) : value(v), left(nullptr), right(nullptr) {}
    };

    Node* rootNode_;

    static void destroy(Node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

    Node* insertNode(Node* n, int v) {
        if (!n) return new Node(v);
        if (v < n->value) n->left = insertNode(n->left, v);
        else if (v > n->value) n->right = insertNode(n->right, v);
        return n;
    }

    bool containsNode(const Node* n, int v) const {
        if (!n) return false;
        if (v == n->value) return true;
        return v < n->value ? containsNode(n->left, v) : containsNode(n->right, v);
    }

    void inorderNode(const Node* n, std::vector<int>& out) const {
        if (!n) return;
        inorderNode(n->left, out);
        out.push_back(n->value);
        inorderNode(n->right, out);
    }

    Node* minNode(Node* n) const {
        while (n && n->left) n = n->left;
        return n;
    }

public:
    IntBST() : rootNode_(nullptr) {}
    ~IntBST() { destroy(rootNode_); }

    void insert(int value) { rootNode_ = insertNode(rootNode_, value); }

    bool contains(int value) const { return containsNode(rootNode_, value); }

    void inorder(std::vector<int>& out) const { inorderNode(rootNode_, out); }

    int& min() {
        Node* m = minNode(rootNode_);
        return m->value;
    }

    const int& min() const {
        Node* m = minNode(rootNode_);
        return m->value;
    }

    bool empty() const { return rootNode_ == nullptr; }
};

int main()
{
    IntBST t;
    std::string op;
    while (std::cin >> op)
{
        if (op == "DONE") break;
        t.insert(std::stoi(op));
    }

    while (std::cin >> op)
{
        if (op == "DONE") break;
        if (op == "CONTAINS")
{
            int x; std::cin >> x;
            std::cout << (t.contains(x) ? "YES" : "NO") << '\n';
        } else if (op == "MIN") {
            if (t.empty()) std::cout << "EMPTY\n";
            else std::cout << t.min() << '\n';
        } else if (op == "PRINT") {
            std::vector<int> vals;
            t.inorder(vals);
            for (std::size_t i = 0; i < vals.size(); ++i) {
                if (i) std::cout << ' ';
                std::cout << vals[i];
            }
            std::cout << '\n';
        }
    }
    return 0;
}
