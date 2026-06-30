#include <string>
#include <vector>
#include <sstream>
#include <iostream>

class IntBST {
    struct Node {
        int data;
        Node* leftChild;
        Node* rightChild;
        explicit Node(int d) : data(d), leftChild(nullptr), rightChild(nullptr) {}
    };

    Node* rootv_;

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
    IntBST() : rootv_(nullptr) {}
    ~IntBST() { freeNodes(rootv_); }

    void insert(int value) { rootv_ = insertHelper(rootv_, value); }
    bool contains(int value) const { return containsHelper(rootv_, value); }
    void inorder(std::vector<int>& out) const { inorderHelper(rootv_, out); }

    int& min() { return minNodeHelper(rootv_)->data; }
    const int& min() const { return minNodeHelper(rootv_)->data; }

    bool empty() const { return rootv_ == nullptr; }
};

int main() {
    std::stringstream input;
    input << std::cin.rdbuf();

    IntBST tree;
    std::string tmp_token;
    while (input >> tmp_token) {
        if (tmp_token == "DONE") break;
        tree.insert(std::stoi(tmp_token));
    }

    while (input >> tmp_token) {
        if (tmp_token == "DONE") break;
        if (tmp_token == "CONTAINS") {
            int value;
            input >> value;
            std::cout << (tree.contains(value) ? "YES" : "NO") << std::endl;
        } else if (tmp_token == "MIN") {
            if (tree.empty())
                std::cout << "EMPTY" << std::endl;
            else
                std::cout << tree.min() << std::endl;
        } else if (tmp_token == "PRINT") {
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
