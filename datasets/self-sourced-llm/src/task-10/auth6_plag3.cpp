#include <vector>
#include <sstream>
#include <iostream>
#include <string>
// basically the same as lecture slides lol

class IntBST {
    struct Block {
        int payload;
        Block* west;
        Block* east;
        explicit Block(int p) : payload(p), west(nullptr), east(nullptr) {}
    };

    Block* apex_;

    void drop(Block* blk) {
        if (!blk) return;
        std::vector<Block*> order;
        std::vector<Block*> pending;
        pending.push_back(blk);
        while (!pending.empty()) {
            Block* cur = pending.back();
            pending.pop_back();
            order.push_back(cur);
            if (cur->west) pending.push_back(cur->west);
            if (cur->east) pending.push_back(cur->east);
        }
        for (std::size_t k = order.size(); k-- > 0;)
            delete order[k];
    }

    Block* graft(Block* blk, int p) {
        if (!blk) return new Block(p);
        if (p < blk->payload) blk->west = graft(blk->west, p);
        else if (p > blk->payload) blk->east = graft(blk->east, p);
        return blk;
    }

    bool scan(const Block* blk, int p) const {
        if (!blk) return false;
        if (p == blk->payload) return true;
        if (p < blk->payload) return scan(blk->west, p);
        return scan(blk->east, p);
    }

    void emit(const Block* blk, std::vector<int>& buf) const {
        if (!blk) return;
        emit(blk->west, buf);
        buf.push_back(blk->payload);
        emit(blk->east, buf);
    }

    Block* floor(Block* blk) const {
        while (blk && blk->west) blk = blk->west;
        return blk;
    }

public:
    IntBST() : apex_(nullptr) {}
    ~IntBST() { drop(apex_); }

    void insert(int value) { apex_ = graft(apex_, value); }
    bool contains(int value) const { return scan(apex_, value); }
    void inorder(std::vector<int>& out) const { emit(apex_, out); }

    int& min() { return floor(apex_)->payload; }
    const int& min() const { return floor(apex_)->payload; }

    bool empty() const { return apex_ == nullptr; }
};

int main() {
    std::stringstream pipe;
    pipe << std::cin.rdbuf();

    IntBST grove;
    std::string chunk;
    while (pipe >> chunk) {
        if (chunk == "DONE") break;
        grove.insert(std::stoi(chunk));
    }

    while (pipe >> chunk) {
        if (chunk == "DONE") break;
        if (chunk == "CONTAINS") {
            int value;
            pipe >> value;
            std::cout << (grove.contains(value) ? "YES" : "NO") << std::endl;
        } else if (chunk == "MIN") {
            if (grove.empty())
                std::cout << "EMPTY" << std::endl;
            else
                std::cout << grove.min() << std::endl;
        } else if (chunk == "PRINT") {
            std::vector<int> ordered;
            grove.inorder(ordered);
            for (std::size_t idx = 0; idx < ordered.size(); ++idx) {
                if (idx > 0) std::cout << ' ';
                std::cout << ordered[idx];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
