#include <iostream>
#include <sstream>
#include <string>
#include <vector>
// cleaned up version

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

    IntBST bst;
    std::string elem;
    while (input >> elem) {
        if (elem == "DONE") break;
        bst.insert(std::stoi(elem));
    }

    while (input >> elem) {
        if (elem == "DONE") break;
        if (elem == "CONTAINS") {
            int value;
            input >> value;
            std::cout << (bst.contains(value) ? "YES" : "NO") << std::endl;
        } else if (elem == "MIN") {
            if (bst.empty())
                std::cout << "EMPTY" << std::endl;
            else
                std::cout << bst.min() << std::endl;
        } else if (elem == "PRINT") {
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
