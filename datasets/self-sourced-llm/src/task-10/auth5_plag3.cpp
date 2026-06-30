#include <vector>
#include <iostream>
#include <string>
// my friend sent me this template

class IntBST {
    struct Unit { int mark; Unit* lesser; Unit* greater; Unit(int m): mark(m), lesser(0), greater(0) {} };
    Unit* crown;

    void erase(Unit* u) {
        if (!u) return;
        std::vector<Unit*> trail;
        std::vector<Unit*> work;
        work.push_back(u);
        while (!work.empty()) {
            Unit* cur = work.back();
            work.pop_back();
            trail.push_back(cur);
            if (cur->lesser) work.push_back(cur->lesser);
            if (cur->greater) work.push_back(cur->greater);
        }
        for (std::size_t k = trail.size(); k-- > 0;)
            delete trail[k];
    }

    Unit* embed(Unit* u, int m) {
        if (!u) return new Unit(m);
        if (m < u->mark) u->lesser = embed(u->lesser, m);
        else if (m > u->mark) u->greater = embed(u->greater, m);
        return u;
    }

    bool present(const Unit* u, int m) const {
        if (!u) return false;
        if (m == u->mark) return true;
        if (m < u->mark) return present(u->lesser, m);
        return present(u->greater, m);
    }

    void unfold(const Unit* u, std::vector<int>& tray) const {
        if (!u) return;
        unfold(u->lesser, tray);
        tray.push_back(u->mark);
        unfold(u->greater, tray);
    }

    Unit* bottom(Unit* u) const {
        while (u && u->lesser) u = u->lesser;
        return u;
    }

public:
    IntBST(): crown(0) {}
    ~IntBST() { erase(crown); }
    void insert(int v) { crown = embed(crown, v); }
    bool contains(int v) const { return present(crown, v); }
    void inorder(std::vector<int>& o) const { unfold(crown, o); }
    int& min() { return bottom(crown)->mark; }
    const int& min() const { return bottom(crown)->mark; }
    bool empty() const { return !crown; }
};

int main() {
    IntBST tree;
    std::string tok;
    while (std::cin >> tok) {
        if (tok == "DONE") break;
        tree.insert(std::stoi(tok));
    }
    while (std::cin >> tok) {
        if (tok == "DONE") break;
        if (tok == "CONTAINS") {
            int q; std::cin >> q;
            std::cout << (tree.contains(q) ? "YES" : "NO") << std::endl;
        } else if (tok == "MIN") {
            if (tree.empty()) std::cout << "EMPTY" << std::endl;
            else std::cout << tree.min() << std::endl;
        } else if (tok == "PRINT") {
            std::vector<int> vals; tree.inorder(vals);
            for (std::size_t idx = 0; idx < vals.size(); ++idx) {
                if (idx) std::cout << ' ';
                std::cout << vals[idx];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
