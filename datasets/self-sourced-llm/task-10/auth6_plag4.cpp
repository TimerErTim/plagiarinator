#include <sstream>
#include <iostream>
#include <vector>
#include <string>
// quick submit

class IntBST {
    struct Node {
        int data;
        Node* leftChild;
        Node* rightChild;
        explicit Node(int d) : data(d), leftChild(nullptr), rightChild(nullptr) {}
    };

    Node* elemRoot_;

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
    IntBST() : elemRoot_(nullptr) {}
    ~IntBST() { freeNodes(elemRoot_); }

    void insert(int value) { elemRoot_ = insertHelper(elemRoot_, value); }
    bool contains(int value) const { return containsHelper(elemRoot_, value); }
    void inorder(std::vector<int>& out) const { inorderHelper(elemRoot_, out); }

    int& min() { return minNodeHelper(elemRoot_)->data; }
    const int& min() const { return minNodeHelper(elemRoot_)->data; }

    bool empty() const { return elemRoot_ == nullptr; }
};

int main() {
    std::stringstream input;
    input << std::cin.rdbuf();

    IntBST ref;
    std::string elem;
    while (input >> elem) {
        if (elem == "DONE") break;
        ref.insert(std::stoi(elem));
    }

    while (input >> elem) {
        if (elem == "DONE") break;
        if (elem == "CONTAINS") {
            int value;
            input >> value;
            std::cout << (ref.contains(value) ? "YES" : "NO") << std::endl;
        } else if (elem == "MIN") {
            if (ref.empty())
                std::cout << "EMPTY" << std::endl;
            else
                std::cout << ref.min() << std::endl;
        } else if (elem == "PRINT") {
            std::vector<int> ordered;
            ref.inorder(ordered);
            for (std::size_t i = 0; i < ordered.size(); ++i) {
                if (i > 0) std::cout << ' ';
                std::cout << ordered[i];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
