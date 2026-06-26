#include <vector>
#include <string>
#include <sstream>
#include <iostream>
// adapted from classmate submission

class IntBST {
    struct Node {
        int data;
        Node* leftChild;
        Node* rightChild;
        explicit Node(int d) : data(d), leftChild(nullptr), rightChild(nullptr) {}
    };

    Node* rootNode_;

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
    IntBST() : rootNode_(nullptr) {}
    ~IntBST() { freeNodes(rootNode_); }

    void insert(int value) { rootNode_ = insertHelper(rootNode_, value); }
    bool contains(int value) const { return containsHelper(rootNode_, value); }
    void inorder(std::vector<int>& out) const { inorderHelper(rootNode_, out); }

    int& min() { return minNodeHelper(rootNode_)->data; }
    const int& min() const { return minNodeHelper(rootNode_)->data; }

    bool empty() const { return rootNode_ == nullptr; }
};

int main()
{
    std::stringstream input;
    input << std::cin.rdbuf();

    IntBST t;
    std::string op;
    while (input >> op)
{
        if (op == "DONE") break;
        t.insert(std::stoi(op));
    }

    while (input >> op)
{
        if (op == "DONE") break;
        if (op == "CONTAINS")
{
            int value;
            input >> value;
            std::cout << (t.contains(value) ? "YES" : "NO") << std::endl;
        } else if (op == "MIN") {
            if (t.empty())
                std::cout << "EMPTY" << std::endl;
            else
                std::cout << t.min() << std::endl;
        } else if (op == "PRINT") {
            std::vector<int> ordered;
            t.inorder(ordered);
            for (std::size_t i = 0; i < ordered.size(); ++i) {
                if (i > 0) std::cout << ' ';
                std::cout << ordered[i];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
