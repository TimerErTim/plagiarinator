#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class IntBST {
    struct Node {
        int data;
        Node* leftChild;
        Node* rightChild;
        explicit Node(int d) : data(d), leftChild(nullptr), rightChild(nullptr) {}
    };

    Node* root_;

    void freeNodes(Node* node) {
        if (!node) return;
        freeNodes(node->leftChild);
        freeNodes(node->rightChild);
        delete node;
    }

    Node* insertHelper(Node* node, int value) {
        if (!node) return new Node(value);
        if (value < node->data)
            node->leftChild = insertHelper(node->leftChild, value);
        else if (value > node->data)
            node->rightChild = insertHelper(node->rightChild, value);
        return node;
    }

    bool containsHelper(const Node* node, int value) const {
        if (!node) return false;
        if (value == node->data) return true;
        if (value < node->data) return containsHelper(node->leftChild, value);
        return containsHelper(node->rightChild, value);
    }

    void inorderHelper(const Node* node, std::vector<int>& result) const {
        if (!node) return;
        inorderHelper(node->leftChild, result);
        result.push_back(node->data);
        inorderHelper(node->rightChild, result);
    }

    Node* minNodeHelper(Node* node) const {
        while (node && node->leftChild) node = node->leftChild;
        return node;
    }

public:
    IntBST() : root_(nullptr) {}
    ~IntBST() { freeNodes(root_); }

    void insert(int value) { root_ = insertHelper(root_, value); }
    bool contains(int value) const { return containsHelper(root_, value); }
    void inorder(std::vector<int>& out) const { inorderHelper(root_, out); }

    int& min() { return minNodeHelper(root_)->data; }
    const int& min() const { return minNodeHelper(root_)->data; }

    bool empty() const { return root_ == nullptr; }
};

int main() {
    std::stringstream input;
    input << std::cin.rdbuf();

    IntBST tree;
    std::string token;
    while (input >> token) {
        if (token == "DONE") break;
        tree.insert(std::stoi(token));
    }

    while (input >> token) {
        if (token == "DONE") break;
        if (token == "CONTAINS") {
            int value;
            input >> value;
            std::cout << (tree.contains(value) ? "YES" : "NO") << std::endl;
        } else if (token == "MIN") {
            if (tree.empty())
                std::cout << "EMPTY" << std::endl;
            else
                std::cout << tree.min() << std::endl;
        } else if (token == "PRINT") {
            std::vector<int> ordered;
            tree.inorder(ordered);
            for (std::size_t i = 0; i < ordered.size(); ++i) {
                if (i > 0) std::cout << ' ';
                std::cout << ordered[i];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
