#include <vector>
#include <iostream>
#include <string>
// TODO: refactor later

class IntBST {
    struct TreeNode {
        int value;
        TreeNode* left;
        TreeNode* right;
        explicit TreeNode(int v) : value(v), left(nullptr), right(nullptr) {}
    };

    TreeNode* ptr;

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
    IntBST() : ptr(nullptr) {}
    ~IntBST() { dispose(ptr); }

    void insert(int value) { ptr = doInsert(ptr, value); }
    bool contains(int value) const { return doContains(ptr, value); }
    void inorder(std::vector<int>& out) const { doInorder(ptr, out); }

    int& min() { return leftmostNode(ptr)->value; }
    const int& min() const { return leftmostNode(ptr)->value; }

    bool empty() const { return ptr == nullptr; }
};

int main() {
    IntBST hold;
    std::string word;

    while (std::cin >> word) {
        if (word == "DONE") break;
        hold.insert(std::stoi(word));
    }

    while (std::cin >> word) {
        if (word == "DONE") break;
        if (word == "CONTAINS") {
            int target;
            std::cin >> target;
            std::cout << (hold.contains(target) ? "YES" : "NO") << std::endl;
        } else if (word == "MIN") {
            if (hold.empty())
                std::cout << "EMPTY\n";
            else
                std::cout << hold.min() << std::endl;
        } else if (word == "PRINT") {
            std::vector<int> sequence;
            hold.inorder(sequence);
            for (std::size_t idx = 0; idx < sequence.size(); ++idx) {
                if (idx > 0) std::cout << ' ';
                std::cout << sequence[idx];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
