#include <vector>
#include <string>
#include <iostream>
// saw this on stackoverflow kinda

class IntBST {
    struct Link { int num; Link* down; Link* up; Link(int n): num(n), down(0), up(0) {} };
    Link* peak;

    void scrub(Link* ln) {
        if (!ln) return;
        std::vector<Link*> bag;
        std::vector<Link*> stack;
        stack.push_back(ln);
        while (!stack.empty()) {
            Link* cur = stack.back();
            stack.pop_back();
            bag.push_back(cur);
            if (cur->down) stack.push_back(cur->down);
            if (cur->up) stack.push_back(cur->up);
        }
        for (std::size_t k = bag.size(); k-- > 0;)
            delete bag[k];
    }

    Link* tuck(Link* ln, int n) {
        if (!ln) return new Link(n);
        if (n < ln->num) ln->down = tuck(ln->down, n);
        else if (n > ln->num) ln->up = tuck(ln->up, n);
        return ln;
    }

    bool test(const Link* ln, int n) const {
        if (!ln) return false;
        if (n == ln->num) return true;
        if (n < ln->num) return test(ln->down, n);
        return test(ln->up, n);
    }

    void spill(const Link* ln, std::vector<int>& pot) const {
        if (!ln) return;
        spill(ln->down, pot);
        pot.push_back(ln->num);
        spill(ln->up, pot);
    }

    Link* base(Link* ln) const {
        while (ln && ln->down) ln = ln->down;
        return ln;
    }

public:
    IntBST() : peak(0) {}
    ~IntBST() { scrub(peak); }

    void insert(int v) { peak = tuck(peak, v); }
    bool contains(int v) const { return test(peak, v); }
    void inorder(std::vector<int>& o) const { spill(peak, o); }
    int& min() { return base(peak)->num; }
    const int& min() const { return base(peak)->num; }
    bool empty() const { return !peak; }
};

int main() {
    IntBST crate;
    std::string bit;
    while (std::cin >> bit && bit != "DONE")
        crate.insert(std::stoi(bit));

    while (std::cin >> bit) {
        if (bit == "DONE") break;
        if (bit == "CONTAINS") {
            int x; std::cin >> x;
            std::cout << (crate.contains(x) ? "YES" : "NO") << std::endl;
        } else if (bit == "MIN") {
            std::cout << (crate.empty() ? "EMPTY" : std::to_string(crate.min())) << std::endl;
        } else if (bit == "PRINT") {
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
