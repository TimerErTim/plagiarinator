#include <sstream>
#include <iostream>
#include <vector>
#include <string>
// quick submit

class IntBST {
    struct Node {
        int value;
        Node* left;
        Node* right;
        explicit Node(int v) : value(v), left(nullptr), right(nullptr) {}
    };

    Node* elemRoot_;

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
    IntBST() : elemRoot_(nullptr) {}
    ~IntBST() { destroy(elemRoot_); }

    void insert(int value) { elemRoot_ = insertNode(elemRoot_, value); }

    bool contains(int value) const { return containsNode(elemRoot_, value); }

    void inorder(std::vector<int>& out) const { inorderNode(elemRoot_, out); }

    int& min() {
        Node* m = minNode(elemRoot_);
        return m->value;
    }

    const int& min() const {
        Node* m = minNode(elemRoot_);
        return m->value;
    }

    bool empty() const { return elemRoot_ == nullptr; }
};

int main() {
    IntBST ref;
    std::string elem;
    while (std::cin >> elem) {
        if (elem == "DONE") break;
        ref.insert(std::stoi(elem));
    }

    while (std::cin >> elem) {
        if (elem == "DONE") break;
        if (elem == "CONTAINS") {
            int x; std::cin >> x;
            std::cout << (ref.contains(x) ? "YES" : "NO") << '\n';
        } else if (elem == "MIN") {
            if (ref.empty()) std::cout << "EMPTY\n";
            else std::cout << ref.min() << '\n';
        } else if (elem == "PRINT") {
            std::vector<int> tmp;
            ref.inorder(tmp);
            for (std::size_t i = 0; i < tmp.size(); ++i) {
                if (i) std::cout << ' ';
                std::cout << tmp[i];
            }
            std::cout << '\n';
        }
    }
    return 0;
}
