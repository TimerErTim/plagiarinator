#include <vector>
#include <iostream>
#include <sstream>
#include <string>
// tweaked from lab partner code

class IntBST {
    struct Vertex {
        int payload;
        Vertex* lo;
        Vertex* hi;
        explicit Vertex(int p) : payload(p), lo(nullptr), hi(nullptr) {}
    };

    Vertex* rootz_;

    static void wipe(Vertex* vtx) {
        if (!vtx) return;
        std::vector<Vertex*> order;
        std::vector<Vertex*> pending;
        pending.push_back(vtx);
        while (!pending.empty()) {
            Vertex* cur = pending.back();
            pending.pop_back();
            order.push_back(cur);
            if (cur->lo) pending.push_back(cur->lo);
            if (cur->hi) pending.push_back(cur->hi);
        }
        for (std::size_t k = order.size(); k-- > 0;)
            delete order[k];
    }

    Vertex* attach(Vertex* vtx, int p) {
        if (!vtx) return new Vertex(p);
        if (p < vtx->payload) vtx->lo = attach(vtx->lo, p);
        else if (p > vtx->payload) vtx->hi = attach(vtx->hi, p);
        return vtx;
    }

    bool seek(const Vertex* vtx, int p) const {
        if (!vtx) return false;
        if (p == vtx->payload) return true;
        if (p < vtx->payload) return seek(vtx->lo, p);
        return seek(vtx->hi, p);
    }

    void flatten(const Vertex* vtx, std::vector<int>& buf) const {
        if (!vtx) return;
        flatten(vtx->lo, buf);
        buf.push_back(vtx->payload);
        flatten(vtx->hi, buf);
    }

    Vertex* floorNode(Vertex* vtx) const {
        if (!vtx) return nullptr;
        for (; vtx->lo != nullptr; vtx = vtx->lo) {}
        return vtx;
    }

public:
    IntBST() : rootz_(nullptr) {}
    ~IntBST() { wipe(rootz_); }

    void insert(int value) { rootz_ = attach(rootz_, value); }

    bool contains(int value) const { return seek(rootz_, value); }

    void inorder(std::vector<int>& out) const { flatten(rootz_, out); }

    int& min() { return floorNode(rootz_)->payload; }

    const int& min() const { return floorNode(rootz_)->payload; }

    bool empty() const { return rootz_ == nullptr; }
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
            std::vector<int> seq;
            grove.inorder(seq);
            for (std::size_t idx = 0; idx < seq.size(); ++idx) {
                if (idx > 0) std::cout << ' ';
                std::cout << seq[idx];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
