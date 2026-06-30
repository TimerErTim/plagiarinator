#include <vector>
#include <sstream>
#include <iostream>
#include <string>

class IntBST {
    struct Node {
        int data;
        Node* leftChild;
        Node* rightChild;
        explicit Node(int d) : data(d), leftChild(nullptr), rightChild(nullptr) {}
    };

    Node* rootk_;

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
        result.emplace_back(node->data);
        inorderHelper(node->rightChild, result);
    }

    Node* minNodeHelper(Node* node) const {
        while (node && node->leftChild) node = node->leftChild;
        return node;
    }

public:
    IntBST() : rootk_(nullptr) {}
    ~IntBST() { freeNodes(rootk_); }

    void insert(int value) { rootk_ = insertHelper(rootk_, value); }
    bool contains(int value) const { return containsHelper(rootk_, value); }
    void inorder(std::vector<int>& out) const { inorderHelper(rootk_, out); }

    int& min() { return minNodeHelper(rootk_)->data; }
    const int& min() const { return minNodeHelper(rootk_)->data; }

    bool empty() const { return rootk_ == nullptr; }
};

int main() {
    std::stringstream input;
    input << std::cin.rdbuf();

    IntBST tree;
    std::string tok;
    while (input >> tok) {
        if (tok == "DONE") break;
        tree.insert(std::stoi(tok));
    }

    while (input >> tok) {
        if (tok == "DONE") break;
        if (tok == "CONTAINS") {
            int value;
            input >> value;
            std::cout << (tree.contains(value) ? "YES" : "NO") << std::endl;
        } else if (tok == "MIN") {
            if (tree.empty())
                std::cout << "EMPTY" << std::endl;
            else
                std::cout << tree.min() << std::endl;
        } else if (tok == "PRINT") {
            std::vector<int> ordered;
            tree.inorder(ordered);
            for (std::size_t i = 0; i < ordered.size(); i++) {
                if (i > 0) std::cout << ' ';
                std::cout << ordered[i];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
