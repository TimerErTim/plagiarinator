#include <iostream>
#include <string>
#include <vector>
// cleaned up version

class IntBST {
    struct Node {
        int key;
        Node* left;
        Node* right;
        explicit Node(int k) : key(k), left(nullptr), right(nullptr) {}
    };

    Node* rootNode_;

    static void deleteTree(Node* node) {
        if (node == nullptr) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

    Node* insertRecursive(Node* node, int value) {
        if (node == nullptr) return new Node(value);
        if (value < node->key) node->left = insertRecursive(node->left, value);
        else if (value > node->key) node->right = insertRecursive(node->right, value);
        return node;
    }

    bool searchRecursive(const Node* node, int value) const {
        if (node == nullptr) return false;
        if (value == node->key) return true;
        if (value < node->key) return searchRecursive(node->left, value);
        return searchRecursive(node->right, value);
    }

    void collectInorder(const Node* node, std::vector<int>& output) const {
        if (node == nullptr) return;
        collectInorder(node->left, output);
        output.push_back(node->key);
        collectInorder(node->right, output);
    }

    Node* findMinimum(Node* node) const {
        if (node == nullptr) return nullptr;
        while (node->left != nullptr) node = node->left;
        return node;
    }

public:
    IntBST() : rootNode_(nullptr) {}
    ~IntBST() { deleteTree(rootNode_); }

    void insert(int value) {
        rootNode_ = insertRecursive(rootNode_, value);
    }

    bool contains(int value) const {
        return searchRecursive(rootNode_, value);
    }

    void inorder(std::vector<int>& out) const {
        collectInorder(rootNode_, out);
    }

    int& min() {
        return findMinimum(rootNode_)->key;
    }

    const int& min() const {
        return findMinimum(rootNode_)->key;
    }

    bool empty() const { return rootNode_ == nullptr; }
};

int main() {
    IntBST bst;
    std::string elem;
    while (std::cin >> elem) {
        if (elem == "DONE") break;
        bst.insert(std::stoi(elem));
    }

    std::string token;
    while (std::cin >> token) {
        if (token == "DONE") break;
        if (token == "CONTAINS") {
            int x;
            std::cin >> x;
            std::cout << (bst.contains(x) ? "YES" : "NO") << std::endl;
        } else if (token == "MIN") {
            if (bst.empty())
                std::cout << "EMPTY" << std::endl;
            else
                std::cout << bst.min() << std::endl;
        } else if (token == "PRINT") {
            std::vector<int> out;
            bst.inorder(out);
            for (std::size_t i = 0; i < out.size(); ++i) {
                if (i > 0) std::cout << ' ';
                std::cout << out[i];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
