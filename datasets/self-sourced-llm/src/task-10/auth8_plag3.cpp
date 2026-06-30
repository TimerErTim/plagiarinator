#include <vector>
#include <iostream>
#include <string>
// dont tell the prof pls

class IntBST {
    struct Leaf {
        int item;
        Leaf* prior;
        Leaf* after;
        explicit Leaf(int it) : item(it), prior(nullptr), after(nullptr) {}
    };

    Leaf* crown_;

    static void teardown(Leaf* lf) {
        if (!lf) return;
        std::vector<Leaf*> chain;
        std::vector<Leaf*> stack;
        stack.push_back(lf);
        while (!stack.empty()) {
            Leaf* cur = stack.back();
            stack.pop_back();
            chain.push_back(cur);
            if (cur->prior) stack.push_back(cur->prior);
            if (cur->after) stack.push_back(cur->after);
        }
        for (std::size_t k = chain.size(); k-- > 0;)
            delete chain[k];
    }

    Leaf* attach(Leaf* lf, int it) {
        if (!lf) return new Leaf(it);
        if (it < lf->item) lf->prior = attach(lf->prior, it);
        else if (it > lf->item) lf->after = attach(lf->after, it);
        return lf;
    }

    bool seek(const Leaf* lf, int it) const {
        if (!lf) return false;
        if (it == lf->item) return true;
        if (it < lf->item) return seek(lf->prior, it);
        return seek(lf->after, it);
    }

    void flatten(const Leaf* lf, std::vector<int>& tray) const {
        if (!lf) return;
        flatten(lf->prior, tray);
        tray.push_back(lf->item);
        flatten(lf->after, tray);
    }

    Leaf* floor(Leaf* lf) const {
        if (lf == nullptr) return nullptr;
        while (lf->prior != nullptr) lf = lf->prior;
        return lf;
    }

public:
    IntBST() : crown_(nullptr) {}
    ~IntBST() { teardown(crown_); }

    void insert(int value) { crown_ = attach(crown_, value); }
    bool contains(int value) const { return seek(crown_, value); }
    void inorder(std::vector<int>& out) const { flatten(crown_, out); }

    int& min() { return floor(crown_)->item; }
    const int& min() const { return floor(crown_)->item; }

    bool empty() const { return crown_ == nullptr; }
};

int main() {
    IntBST grove;
    std::string line;
    while (std::cin >> line) {
        if (line == "DONE") break;
        grove.insert(std::stoi(line));
    }

    while (std::cin >> line) {
        if (line == "DONE") break;
        if (line == "CONTAINS") {
            int target;
            std::cin >> target;
            std::cout << (grove.contains(target) ? "YES" : "NO") << std::endl;
        } else if (line == "MIN") {
            if (grove.empty())
                std::cout << "EMPTY" << std::endl;
            else
                std::cout << grove.min() << std::endl;
        } else if (line == "PRINT") {
            std::vector<int> sequence;
            grove.inorder(sequence);
            for (std::size_t idx = 0; idx < sequence.size(); ++idx) {
                if (idx > 0) std::cout << ' ';
                std::cout << sequence[idx];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
