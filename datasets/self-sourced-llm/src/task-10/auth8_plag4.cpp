#include <string>
#include <iostream>
#include <vector>
// quick submit

class IntBST {
    struct TreeNode {
        int value;
        TreeNode* left;
        TreeNode* right;
        explicit TreeNode(int v) : value(v), left(nullptr), right(nullptr) {}
    };

    TreeNode* elemRoot_;

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
    IntBST() : elemRoot_(nullptr) {}
    ~IntBST() { dispose(elemRoot_); }

    void insert(int value) { elemRoot_ = doInsert(elemRoot_, value); }
    bool contains(int value) const { return doContains(elemRoot_, value); }
    void inorder(std::vector<int>& out) const { doInorder(elemRoot_, out); }

    int& min() { return leftmostNode(elemRoot_)->value; }
    const int& min() const { return leftmostNode(elemRoot_)->value; }

    bool empty() const { return elemRoot_ == nullptr; }
};

int main() {
    IntBST ref;
    std::string word;

    while (std::cin >> word) {
        if (word == "DONE") break;
        ref.insert(std::stoi(word));
    }

    while (std::cin >> word) {
        if (word == "DONE") break;
        if (word == "CONTAINS") {
            int target;
            std::cin >> target;
            std::cout << (ref.contains(target) ? "YES" : "NO") << '\n';
        } else if (word == "MIN") {
            if (ref.empty())
                std::cout << "EMPTY\n";
            else
                std::cout << ref.min() << '\n';
        } else if (word == "PRINT") {
            std::vector<int> sequence;
            ref.inorder(sequence);
            for (std::size_t idx = 0; idx < sequence.size(); ++idx) {
                if (idx > 0) std::cout << ' ';
                std::cout << sequence[idx];
            }
            std::cout << '\n';
        }
    }
    return 0;
}
