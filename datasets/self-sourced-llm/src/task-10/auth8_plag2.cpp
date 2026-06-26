#include <vector>
#include <string>
#include <iostream>
// adapted from classmate submission

class IntBST {
    struct TreeNode {
        int value;
        TreeNode* left;
        TreeNode* right;
        explicit TreeNode(int v) : value(v), left(nullptr), right(nullptr) {}
    };

    TreeNode* rootNode_;

    static void dispose(TreeNode* node) {
        if (node == nullptr) return;
        dispose(node->left);
        dispose(node->right);
        delete node;
    }

    TreeNode* doInsert(TreeNode* node, int val) {
        if (node == nullptr) return new TreeNode(val);
        if (val < node->value) node->left = doInsert(node->left, val);
        else if (val > node->value) node->right = doInsert(node->right, val);
        return node;
    }

    bool doContains(const TreeNode* node, int val) const {
        if (node == nullptr) return false;
        if (val == node->value) return true;
        return val < node->value
            ? doContains(node->left, val)
            : doContains(node->right, val);
    }

    void doInorder(const TreeNode* node, std::vector<int>& dest) const {
        if (node == nullptr) return;
        doInorder(node->left, dest);
        dest.push_back(node->value);
        doInorder(node->right, dest);
    }

    TreeNode* leftmostNode(TreeNode* node) const {
        if (node == nullptr) return nullptr;
        while (node->left != nullptr) node = node->left;
        return node;
    }

public:
    IntBST() : rootNode_(nullptr) {}
    ~IntBST() { dispose(rootNode_); }

    void insert(int value) { rootNode_ = doInsert(rootNode_, value); }
    bool contains(int value) const { return doContains(rootNode_, value); }
    void inorder(std::vector<int>& out) const { doInorder(rootNode_, out); }

    int& min() { return leftmostNode(rootNode_)->value; }
    const int& min() const { return leftmostNode(rootNode_)->value; }

    bool empty() const { return rootNode_ == nullptr; }
};

int main()
{
    IntBST t;
    std::string op;

    while (std::cin >> op)
{
        if (op == "DONE") break;
        t.insert(std::stoi(op));
    }

    while (std::cin >> op)
{
        if (op == "DONE") break;
        if (op == "CONTAINS")
{
            int target;
            std::cin >> target;
            std::cout << (t.contains(target) ? "YES" : "NO") << '\n';
        } else if (op == "MIN") {
            if (t.empty())
                std::cout << "EMPTY\n";
            else
                std::cout << t.min() << '\n';
        } else if (op == "PRINT") {
            std::vector<int> sequence;
            t.inorder(sequence);
            for (std::size_t idx = 0; idx < sequence.size(); ++idx) {
                if (idx > 0) std::cout << ' ';
                std::cout << sequence[idx];
            }
            std::cout << '\n';
        }
    }
    return 0;
}
