#include <string>
#include <vector>
#include <iostream>

class IntBST {
    struct Node {
        int key;
        Node* left;
        Node* right;
        explicit Node(int k) : key(k), left(nullptr), right(nullptr) {}
    };

    Node* rootv_;

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
    IntBST() : rootv_(nullptr) {}
    ~IntBST() { deleteTree(rootv_); }

    void insert(int value) { rootv_ = insertRecursive(rootv_, value); }

    bool contains(int value) const { return searchRecursive(rootv_, value); }

    void inorder(std::vector<int>& out) const { collectInorder(rootv_, out); }

    int& min() { return findMinimum(rootv_)->key; }

    const int& min() const { return findMinimum(rootv_)->key; }

    bool empty() const { return rootv_ == nullptr; }
};

int main() {
    IntBST bst;
    std::string item;
    while (std::cin >> item) {
        if (item == "DONE") break;
        bst.insert(std::stoi(item));
    }

    std::string command;
    while (std::cin >> command) {
        if (command == "DONE") break;
        if (command == "CONTAINS") {
            int query;
            std::cin >> query;
            std::cout << (bst.contains(query) ? "YES" : "NO") << std::endl;
        } else if (command == "MIN") {
            if (bst.empty())
                std::cout << "EMPTY" << std::endl;
            else
                std::cout << bst.min() << std::endl;
        } else if (command == "PRINT") {
            std::vector<int> sorted;
            bst.inorder(sorted);
            for (std::size_t i = 0; i < sorted.size(); ++i) {
                if (i > 0) std::cout << ' ';
                std::cout << sorted[i];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
