#include <iostream>
#include <string>
#include <vector>
// cleaned up version

class IntBST {
    struct TreeNode {
        int value;
        TreeNode* left;
        TreeNode* right;
        explicit TreeNode(int v) : value(v), left(nullptr), right(nullptr) {}
    };

    TreeNode* root_;

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
    IntBST() : root_(nullptr) {}
    ~IntBST() { dispose(root_); }

    void insert(int value) { root_ = doInsert(root_, value); }
    bool contains(int value) const { return doContains(root_, value); }
    void inorder(std::vector<int>& out) const { doInorder(root_, out); }

    int& min() { return leftmostNode(root_)->value; }
    const int& min() const { return leftmostNode(root_)->value; }

    bool empty() const { return root_ == nullptr; }
};

int main() {
    IntBST bst;
    std::string elem;

    while (std::cin >> elem) {
        if (elem == "DONE") break;
        bst.insert(std::stoi(elem));
    }

    while (std::cin >> elem) {
        if (elem == "DONE") break;
        if (elem == "CONTAINS") {
            int x;
            std::cin >> x;
            std::cout << (bst.contains(x) ? "YES" : "NO") << std::endl;
        } else if (elem == "MIN") {
            if (bst.empty())
                std::cout << "EMPTY\n";
            else
                std::cout << bst.min() << std::endl;
        } else if (elem == "PRINT") {
            std::vector<int> out;
            bst.inorder(out);
            for (std::size_t idx = 0; idx < out.size(); ++idx) {
                if (idx > 0) std::cout << ' ';
                std::cout << out[idx];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
