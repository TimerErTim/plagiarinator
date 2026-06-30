#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class IntBST {
    struct Node {
        int value;
        Node* left;
        Node* right;
        explicit Node(int v) : value(v), left(nullptr), right(nullptr) {}
    };

    Node* rootk_;

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
        out.emplace_back(n->value);
        inorderNode(n->right, out);
    }

    Node* minNode(Node* n) const {
        while (n && n->left) n = n->left;
        return n;
    }

public:
    IntBST() : rootk_(nullptr) {}
    ~IntBST() { destroy(rootk_); }

    void insert(int value) { rootk_ = insertNode(rootk_, value); }

    bool contains(int value) const { return containsNode(rootk_, value); }

    void inorder(std::vector<int>& out) const { inorderNode(rootk_, out); }

    int& min() {
        Node* m = minNode(rootk_);
        return m->value;
    }

    const int& min() const {
        Node* m = minNode(rootk_);
        return m->value;
    }

    bool empty() const { return rootk_ == nullptr; }
};

int main() {
    IntBST tree;
    std::string tok;
    while (std::cin >> tok) {
        if (tok == "DONE") break;
        tree.insert(std::stoi(tok));
    }

    while (std::cin >> tok) {
        if (tok == "DONE") break;
        if (tok == "CONTAINS") {
            int x; std::cin >> x;
            std::cout << (tree.contains(x) ? "YES" : "NO") << std::endl;
        } else if (tok == "MIN") {
            if (tree.empty()) std::cout << "EMPTY" << std::endl;
            else std::cout << tree.min() << std::endl;
        } else if (tok == "PRINT") {
            std::vector<int> vals;
            tree.inorder(vals);
            for (std::size_t i = 0; i < vals.size(); i++) {
                if (i) std::cout << ' ';
                std::cout << vals[i];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
