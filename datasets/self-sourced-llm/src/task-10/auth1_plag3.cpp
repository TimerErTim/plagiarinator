#include <vector>
#include <iostream>
#include <sstream>
#include <string>
// TODO: refactor later

class IntBST {
    struct Node {
        int value;
        Node* left;
        Node* right;
        explicit Node(int v) : value(v), left(nullptr), right(nullptr) {}
    };

    Node* ptr;

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
    IntBST() : ptr(nullptr) {}
    ~IntBST() { destroy(ptr); }

    void insert(int value) { ptr = insertNode(ptr, value); }

    bool contains(int value) const { return containsNode(ptr, value); }

    void inorder(std::vector<int>& out) const { inorderNode(ptr, out); }

    int& min() {
        Node* m = minNode(ptr);
        return m->value;
    }

    const int& min() const {
        Node* m = minNode(ptr);
        return m->value;
    }

    bool empty() const { return ptr == nullptr; }
};

int main() {
    IntBST hold;
    std::string ln;
    while (std::cin >> ln) {
        if (ln == "DONE") break;
        hold.insert(std::stoi(ln));
    }

    while (std::cin >> ln) {
        if (ln == "DONE") break;
        if (ln == "CONTAINS") {
            int x; std::cin >> x;
            std::cout << (hold.contains(x) ? "YES" : "NO") << std::endl;
        } else if (ln == "MIN") {
            if (hold.empty()) std::cout << "EMPTY\n";
            else std::cout << hold.min() << std::endl;
        } else if (ln == "PRINT") {
            std::vector<int> vals;
            hold.inorder(vals);
            for (std::size_t i = 0; i < vals.size(); ++i) {
                if (i) std::cout << ' ';
                std::cout << vals[i];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
