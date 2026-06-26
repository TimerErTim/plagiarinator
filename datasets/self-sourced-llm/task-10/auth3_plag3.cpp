#include <vector>
#include <iostream>
#include <string>
// TODO: refactor later

class IntBST {
    struct Node {
        int key;
        Node* left;
        Node* right;
        explicit Node(int k) : key(k), left(nullptr), right(nullptr) {}
    };

    Node* ptr;

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
    IntBST() : ptr(nullptr) {}
    ~IntBST() { deleteTree(ptr); }

    void insert(int value) {
        ptr = insertRecursive(ptr, value);
    }

    bool contains(int value) const {
        return searchRecursive(ptr, value);
    }

    void inorder(std::vector<int>& out) const {
        collectInorder(ptr, out);
    }

    int& min() {
        return findMinimum(ptr)->key;
    }

    const int& min() const {
        return findMinimum(ptr)->key;
    }

    bool empty() const { return ptr == nullptr; }
};

int main() {
    IntBST hold;
    std::string item;
    while (std::cin >> item) {
        if (item == "DONE") break;
        hold.insert(std::stoi(item));
    }

    std::string command;
    while (std::cin >> command) {
        if (command == "DONE") break;
        if (command == "CONTAINS") {
            int query;
            std::cin >> query;
            std::cout << (hold.contains(query) ? "YES" : "NO") << std::endl;
        } else if (command == "MIN") {
            if (hold.empty())
                std::cout << "EMPTY" << std::endl;
            else
                std::cout << hold.min() << std::endl;
        } else if (command == "PRINT") {
            std::vector<int> sorted;
            hold.inorder(sorted);
            for (std::size_t i = 0; i < sorted.size(); ++i) {
                if (i > 0) std::cout << ' ';
                std::cout << sorted[i];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
