#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class IntBST
{
    struct Node
    {
        int data;
        Node* leftChild;
        Node* rightChild;
        explicit Node(int d) : data(d), leftChild(nullptr), rightChild(nullptr) {}
    };

    Node* rootx_;

    void freeNodes(Node* node)
    {
        if (!node) return;
        freeNodes(node->leftChild);
        freeNodes(node->rightChild);
        delete node;
    }

    Node* insertHelper(Node* node, int value)
    {
        if (!node) return new Node(value);
        if (value < node->data)
            node->leftChild = insertHelper(node->leftChild, value);
        else if (value > node->data)
            node->rightChild = insertHelper(node->rightChild, value);
        return node;
    }

    bool nodeHas(const Node* node, int value) const
    {
        if (!node) return false;
        if (value == node->data) return true;
        if (value < node->data) return nodeHas(node->leftChild, value);
        return nodeHas(node->rightChild, value);
    }

    void inorderHelper(const Node* node, std::vector<int>& result) const
    {
        if (!node) return;
        inorderHelper(node->leftChild, result);
        result.push_back(node->data);
        inorderHelper(node->rightChild, result);
    }

    Node* minNodeHelper(Node* node) const
    {
        while (node && node->leftChild) node = node->leftChild;
        return node;
    }

public:
    IntBST() : rootx_(nullptr) {}
    ~IntBST() { freeNodes(rootx_); }

    void insert(int value) { rootx_ = insertHelper(rootx_, value); }
    bool contains(int value) const { return nodeHas(rootx_, value); }
    void inorder(std::vector<int>& out) const { inorderHelper(rootx_, out); }

    int& min() { return minNodeHelper(rootx_)->data; }
    const int& min() const { return minNodeHelper(rootx_)->data; }

    bool empty() const { return rootx_ == nullptr; }
};

int main()
{
    std::stringstream input;
    input << std::cin.rdbuf();

    IntBST tree;
    std::string token;
    while (input >> token)
    {
        if (token == "DONE") break;
        tree.insert(std::stoi(token));
    }

    while (input >> token)
    {
        if (token == "DONE") break;
        if (token == "CONTAINS")
        {
            int value;
            input >> value;
            if (tree.contains(value)) std::cout << "YES\n";
            else std::cout << "NO\n";
        }
        else if (token == "MIN")
        {
            if (tree.empty()) std::cout << "EMPTY\n";
            else std::cout << tree.min() << '\n';
        }
        else if (token == "PRINT")
        {
            std::vector<int> ordered;
            tree.inorder(ordered);
            for (std::size_t j = 0; j < ordered.size(); ++j)
            {
                if (j > 0) std::cout << ' ';
                std::cout << ordered[j];
            }
            std::cout << '\n';
        }
    }
    return 0;
}
