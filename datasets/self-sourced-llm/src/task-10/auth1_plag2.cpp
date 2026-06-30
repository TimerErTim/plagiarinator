#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class IntBST
{
    struct Node
    {
        int value;
        Node* left;
        Node* right;
        explicit Node(int v) : value(v), left(nullptr), right(nullptr) {}
    };

    Node* rootx_;

    static void destroy(Node* n)
    {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

    Node* insertNode(Node* n, int v)
    {
        if (!n) return new Node(v);
        if (v < n->value) n->left = insertNode(n->left, v);
        else if (v > n->value) n->right = insertNode(n->right, v);
        return n;
    }

    bool lookupNode(const Node* n, int v) const
    {
        if (!n) return false;
        if (v == n->value) return true;
        if (v < n->value) return lookupNode(n->left, v);
        return lookupNode(n->right, v);
    }

    void inorderNode(const Node* n, std::vector<int>& out) const
    {
        if (!n) return;
        inorderNode(n->left, out);
        out.push_back(n->value);
        inorderNode(n->right, out);
    }

    Node* minNode(Node* n) const
    {
        while (n && n->left) n = n->left;
        return n;
    }

public:
    IntBST() : rootx_(nullptr) {}
    ~IntBST() { destroy(rootx_); }

    void insert(int value) { rootx_ = insertNode(rootx_, value); }

    bool contains(int value) const { return lookupNode(rootx_, value); }

    void inorder(std::vector<int>& out) const { inorderNode(rootx_, out); }

    int& min()
    {
        Node* m = minNode(rootx_);
        return m->value;
    }

    const int& min() const
    {
        Node* m = minNode(rootx_);
        return m->value;
    }

    bool empty() const { return rootx_ == nullptr; }
};

int main()
{
    IntBST tree;
    std::string token;
    while (std::cin >> token)
    {
        if (token == "DONE") break;
        tree.insert(std::stoi(token));
    }

    while (std::cin >> token)
    {
        if (token == "DONE") break;
        if (token == "CONTAINS")
        {
            int x; std::cin >> x;
            if (tree.contains(x)) std::cout << "YES\n";
            else std::cout << "NO\n";
        }
        else if (token == "MIN")
        {
            if (tree.empty()) std::cout << "EMPTY\n";
            else std::cout << tree.min() << '\n';
        }
        else if (token == "PRINT")
        {
            std::vector<int> vals;
            tree.inorder(vals);
            for (std::size_t j = 0; j < vals.size(); ++j)
            {
                if (j) std::cout << ' ';
                std::cout << vals[j];
            }
            std::cout << '\n';
        }
    }
    return 0;
}
