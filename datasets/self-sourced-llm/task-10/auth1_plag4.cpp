#include <sstream>
#include <string>
#include <vector>
#include <iostream>

class IntBST {
    struct Node {
        int value;
        Node* left;
        Node* right;
        explicit Node(int v) : value(v), left(nullptr), right(nullptr) {}
    };

    Node* rootv_;

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
    IntBST() : rootv_(nullptr) {}
    ~IntBST() { destroy(rootv_); }

    void insert(int value) { rootv_ = insertNode(rootv_, value); }

    bool contains(int value) const { return containsNode(rootv_, value); }

    void inorder(std::vector<int>& out) const { inorderNode(rootv_, out); }

    int& min() {
        Node* m = minNode(rootv_);
        return m->value;
    }

    const int& min() const {
        Node* m = minNode(rootv_);
        return m->value;
    }

    bool empty() const { return rootv_ == nullptr; }
};

int main() {
    IntBST tree;
    std::string tmp_token;
    while (std::cin >> tmp_token) {
        if (tmp_token == "DONE") break;
        tree.insert(std::stoi(tmp_token));
    }

    while (std::cin >> tmp_token) {
        if (tmp_token == "DONE") break;
        if (tmp_token == "CONTAINS") {
            int x; std::cin >> x;
            std::cout << (tree.contains(x) ? "YES" : "NO") << '\n';
        } else if (tmp_token == "MIN") {
            if (tree.empty()) std::cout << "EMPTY\n";
            else std::cout << tree.min() << '\n';
        } else if (tmp_token == "PRINT") {
            std::vector<int> vals;
            tree.inorder(vals);
            for (std::size_t i = 0; i < vals.size(); ++i) {
                if (i) std::cout << ' ';
                std::cout << vals[i];
            }
            std::cout << '\n';
        }
    }
    return 0;
}
