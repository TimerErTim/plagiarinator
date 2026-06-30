#include <iostream>
#include <string>
#include <vector>

class IntBST
{
    struct Node
    {
        int key;
        Node* left;
        Node* right;
        explicit Node(int k) : key(k), left(nullptr), right(nullptr) {}
    };

    Node* rootx_;

    static void deleteTree(Node* node)
    {
        if (node == nullptr) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

    Node* insertRecursive(Node* node, int value)
    {
        if (node == nullptr) return new Node(value);
        if (value < node->key) node->left = insertRecursive(node->left, value);
        else if (value > node->key) node->right = insertRecursive(node->right, value);
        return node;
    }

    bool matchKey(const Node* node, int value) const
    {
        if (node == nullptr) return false;
        if (value == node->key) return true;
        if (value < node->key) return matchKey(node->left, value);
        return matchKey(node->right, value);
    }

    void collectInorder(const Node* node, std::vector<int>& output) const
    {
        if (node == nullptr) return;
        collectInorder(node->left, output);
        output.push_back(node->key);
        collectInorder(node->right, output);
    }

    Node* findMinimum(Node* node) const
    {
        if (node == nullptr) return nullptr;
        while (node->left != nullptr) node = node->left;
        return node;
    }

public:
    IntBST() : rootx_(nullptr) {}
    ~IntBST() { deleteTree(rootx_); }

    void insert(int value) { rootx_ = insertRecursive(rootx_, value); }

    bool contains(int value) const { return matchKey(rootx_, value); }

    void inorder(std::vector<int>& out) const { collectInorder(rootx_, out); }

    int& min() { return findMinimum(rootx_)->key; }

    const int& min() const { return findMinimum(rootx_)->key; }

    bool empty() const { return rootx_ == nullptr; }
};

int main()
{
    IntBST bst;
    std::string item;
    while (std::cin >> item)
    {
        if (item == "DONE") break;
        bst.insert(std::stoi(item));
    }

    std::string command;
    while (std::cin >> command)
    {
        if (command == "DONE") break;
        if (command == "CONTAINS")
        {
            int query;
            std::cin >> query;
            if (bst.contains(query)) std::cout << "YES\n";
            else std::cout << "NO\n";
        }
        else if (command == "MIN")
        {
            if (bst.empty()) std::cout << "EMPTY\n";
            else std::cout << bst.min() << '\n';
        }
        else if (command == "PRINT")
        {
            std::vector<int> sorted;
            bst.inorder(sorted);
            for (std::size_t j = 0; j < sorted.size(); ++j)
            {
                if (j > 0) std::cout << ' ';
                std::cout << sorted[j];
            }
            std::cout << '\n';
        }
    }
    return 0;
}
