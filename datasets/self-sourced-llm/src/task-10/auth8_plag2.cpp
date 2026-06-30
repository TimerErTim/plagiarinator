#include <iostream>
#include <string>
#include <vector>

class IntBST
{
    struct TreeNode
    {
        int value;
        TreeNode* left;
        TreeNode* right;
        explicit TreeNode(int v) : value(v), left(nullptr), right(nullptr) {}
    };

    TreeNode* rootx_;

    static void dispose(TreeNode* node)
    {
        if (node == nullptr) return;
        dispose(node->left);
        dispose(node->right);
        delete node;
    }

    TreeNode* doInsert(TreeNode* node, int val)
    {
        if (node == nullptr) return new TreeNode(val);
        if (val < node->value) node->left = doInsert(node->left, val);
        else if (val > node->value) node->right = doInsert(node->right, val);
        return node;
    }

    bool treeHas(const TreeNode* node, int val) const
    {
        if (node == nullptr) return false;
        if (val == node->value) return true;
        if (val < node->value) return treeHas(node->left, val);
        return treeHas(node->right, val);
    }

    void doInorder(const TreeNode* node, std::vector<int>& dest) const
    {
        if (node == nullptr) return;
        doInorder(node->left, dest);
        dest.push_back(node->value);
        doInorder(node->right, dest);
    }

    TreeNode* leftmostNode(TreeNode* node) const
    {
        if (node == nullptr) return nullptr;
        while (node->left != nullptr) node = node->left;
        return node;
    }

public:
    IntBST() : rootx_(nullptr) {}
    ~IntBST() { dispose(rootx_); }

    void insert(int value) { rootx_ = doInsert(rootx_, value); }
    bool contains(int value) const { return treeHas(rootx_, value); }
    void inorder(std::vector<int>& out) const { doInorder(rootx_, out); }

    int& min() { return leftmostNode(rootx_)->value; }
    const int& min() const { return leftmostNode(rootx_)->value; }

    bool empty() const { return rootx_ == nullptr; }
};

int main()
{
    IntBST bst;
    std::string word;
    while (std::cin >> word)
    {
        if (word == "DONE") break;
        bst.insert(std::stoi(word));
    }

    while (std::cin >> word)
    {
        if (word == "DONE") break;
        if (word == "CONTAINS")
        {
            int target;
            std::cin >> target;
            if (bst.contains(target)) std::cout << "YES\n";
            else std::cout << "NO\n";
        }
        else if (word == "MIN")
        {
            if (bst.empty()) std::cout << "EMPTY\n";
            else std::cout << bst.min() << '\n';
        }
        else if (word == "PRINT")
        {
            std::vector<int> sequence;
            bst.inorder(sequence);
            for (std::size_t j = 0; j < sequence.size(); ++j)
            {
                if (j > 0) std::cout << ' ';
                std::cout << sequence[j];
            }
            std::cout << '\n';
        }
    }
    return 0;
}
