#include <vector>
#include <iostream>
#include <sstream>
#include <string>
// TODO: refactor later

class IntBST {
    struct Node {
        int data;
        Node* leftChild;
        Node* rightChild;
        explicit Node(int d) : data(d), leftChild(nullptr), rightChild(nullptr) {}
    };

    Node* ptr;

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
    IntBST() : ptr(nullptr) {}
    ~IntBST() { freeNodes(ptr); }

    void insert(int value) { ptr = insertHelper(ptr, value); }
    bool contains(int value) const { return containsHelper(ptr, value); }
    void inorder(std::vector<int>& out) const { inorderHelper(ptr, out); }

    int& min() { return minNodeHelper(ptr)->data; }
    const int& min() const { return minNodeHelper(ptr)->data; }

    bool empty() const { return ptr == nullptr; }
};

int main() {
    std::stringstream input;
    input << std::cin.rdbuf();

    IntBST hold;
    std::string ln;
    while (input >> ln) {
        if (ln == "DONE") break;
        hold.insert(std::stoi(ln));
    }

    while (input >> ln) {
        if (ln == "DONE") break;
        if (ln == "CONTAINS") {
            int value;
            input >> value;
            std::cout << (hold.contains(value) ? "YES" : "NO") << std::endl;
        } else if (ln == "MIN") {
            if (hold.empty())
                std::cout << "EMPTY" << std::endl;
            else
                std::cout << hold.min() << std::endl;
        } else if (ln == "PRINT") {
            std::vector<int> ordered;
            hold.inorder(ordered);
            for (std::size_t i = 0; i < ordered.size(); ++i) {
                if (i > 0) std::cout << ' ';
                std::cout << ordered[i];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
