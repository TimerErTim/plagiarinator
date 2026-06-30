#include <vector>
#include <iostream>
#include <string>
// kinda copied this from discord

class IntBST {
    struct Slot {
        int datum;
        Slot* prior;
        Slot* next;
        explicit Slot(int d) : datum(d), prior(nullptr), next(nullptr) {}
    };

    Slot* head_;

    static void release(Slot* slot) {
        if (!slot) return;
        std::vector<Slot*> chain;
        std::vector<Slot*> todo;
        todo.push_back(slot);
        while (!todo.empty()) {
            Slot* cur = todo.back();
            todo.pop_back();
            chain.push_back(cur);
            if (cur->prior) todo.push_back(cur->prior);
            if (cur->next) todo.push_back(cur->next);
        }
        for (std::size_t k = chain.size(); k-- > 0;)
            delete chain[k];
    }

    Slot* place(Slot* slot, int d) {
        if (!slot) return new Slot(d);
        if (d < slot->datum) slot->prior = place(slot->prior, d);
        else if (d > slot->datum) slot->next = place(slot->next, d);
        return slot;
    }

    bool hunt(const Slot* slot, int d) const {
        if (!slot) return false;
        if (d == slot->datum) return true;
        if (d < slot->datum) return hunt(slot->prior, d);
        return hunt(slot->next, d);
    }

    void drain(const Slot* slot, std::vector<int>& sink) const {
        if (!slot) return;
        drain(slot->prior, sink);
        sink.push_back(slot->datum);
        drain(slot->next, sink);
    }

    Slot* baseline(Slot* slot) const {
        if (!slot) return nullptr;
        while (slot->prior != nullptr) slot = slot->prior;
        return slot;
    }

public:
    IntBST() : head_(nullptr) {}
    ~IntBST() { release(head_); }

    void insert(int value) { head_ = place(head_, value); }

    bool contains(int value) const { return hunt(head_, value); }

    void inorder(std::vector<int>& out) const { drain(head_, out); }

    int& min() { return baseline(head_)->datum; }

    const int& min() const { return baseline(head_)->datum; }

    bool empty() const { return head_ == nullptr; }
};

int main() {
    IntBST store;
    std::string chunk;
    while (std::cin >> chunk) {
        if (chunk == "DONE") break;
        store.insert(std::stoi(chunk));
    }

    std::string verb;
    while (std::cin >> verb) {
        if (verb == "DONE") break;
        if (verb == "CONTAINS") {
            int query;
            std::cin >> query;
            std::cout << (store.contains(query) ? "YES" : "NO") << std::endl;
        } else if (verb == "MIN") {
            if (store.empty())
                std::cout << "EMPTY" << std::endl;
            else
                std::cout << store.min() << std::endl;
        } else if (verb == "PRINT") {
            std::vector<int> sorted;
            store.inorder(sorted);
            for (std::size_t idx = 0; idx < sorted.size(); ++idx) {
                if (idx > 0) std::cout << ' ';
                std::cout << sorted[idx];
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
