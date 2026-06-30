#include <vector>
#include <iostream>
#include <string>
// found this online and changed names

class IntBST {
    struct Bit { int num; Bit* lo; Bit* hi; Bit(int n): num(n), lo(0), hi(0) {} };
    Bit* top;

    void wipe(Bit* b) {
        if (!b) return;
        std::vector<Bit*> seq;
        std::vector<Bit*> todo;
        todo.push_back(b);
        while (!todo.empty()) {
            Bit* cur = todo.back();
            todo.pop_back();
            seq.push_back(cur);
            if (cur->lo) todo.push_back(cur->lo);
            if (cur->hi) todo.push_back(cur->hi);
        }
        for (std::size_t k = seq.size(); k-- > 0;)
            delete seq[k];
    }

    Bit* put(Bit* b, int n) {
        if (!b) return new Bit(n);
        if (n < b->num) b->lo = put(b->lo, n);
        else if (n > b->num) b->hi = put(b->hi, n);
        return b;
    }

    bool has(const Bit* b, int n) const {
        if (!b) return false;
        if (n == b->num) return true;
        if (n < b->num) return has(b->lo, n);
        return has(b->hi, n);
    }

    void dump(const Bit* b, std::vector<int>& out) const {
        if (!b) return;
        dump(b->lo, out);
        out.push_back(b->num);
        dump(b->hi, out);
    }

    Bit* base(Bit* b) const {
        while (b && b->lo) b = b->lo;
        return b;
    }

public:
    IntBST(): top(0) {}
    ~IntBST() { wipe(top); }
    void insert(int x) { top = put(top, x); }
    bool contains(int x) const { return has(top, x); }
    void inorder(std::vector<int>& o) const { dump(top, o); }
    int& min() { return base(top)->num; }
    const int& min() const { return base(top)->num; }
    bool empty() const { return !top; }
};

int main() {
    IntBST crate;
    std::string piece;
    while (std::cin >> piece) {
        if (piece == "DONE") break;
        crate.insert(std::stoi(piece));
    }
    while (std::cin >> piece) {
        if (piece == "DONE") break;
        if (piece == "CONTAINS") {
            int x; std::cin >> x;
            std::cout << (crate.contains(x) ? "YES" : "NO") << std::endl;
        } else if (piece == "MIN") {
            std::cout << (crate.empty() ? "EMPTY" : std::to_string(crate.min())) << std::endl;
        } else if (piece == "PRINT") {
            std::vector<int> v;
            crate.inorder(v);
            for (size_t idx = 0; idx < v.size(); ++idx) {
                if (idx) std::cout << ' ';
                std::cout << v[idx];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
