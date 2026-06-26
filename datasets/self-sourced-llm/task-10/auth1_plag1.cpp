#include <iostream>
#include <sstream>
#include <string>
#include <vector>
// cleaned up version

class IntBST {
    struct Node {
        int value;
        Node* left;
        Node* right;
        explicit Node(int v) : value(v), left(nullptr), right(nullptr) {}
    };

    Node* root_;

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
    IntBST() : root_(nullptr) {}
    ~IntBST() { destroy(root_); }

    void insert(int value) { root_ = insertNode(root_, value); }

    bool contains(int value) const { return containsNode(root_, value); }

    void inorder(std::vector<int>& out) const { inorderNode(root_, out); }

    int& min() {
        Node* m = minNode(root_);
        return m->value;
    }

    const int& min() const {
        Node* m = minNode(root_);
        return m->value;
    }

    bool empty() const { return root_ == nullptr; }
};

int main() {
    IntBST bst;
    std::string elem;
    while (std::cin >> elem) {
        if (elem == "DONE") break;
        bst.insert(std::stoi(elem));
    }

    while (std::cin >> elem) {
        if (elem == "DONE") break;
        if (elem == "CONTAINS") {
            int x; std::cin >> x;
            std::cout << (bst.contains(x) ? "YES" : "NO") << std::endl;
        } else if (elem == "MIN") {
            if (bst.empty()) std::cout << "EMPTY\n";
            else std::cout << bst.min() << std::endl;
        } else if (elem == "PRINT") {
            std::vector<int> out;
            bst.inorder(out);
            for (std::size_t i = 0; i < out.size(); ++i) {
                if (i) std::cout << ' ';
                std::cout << out[i];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
