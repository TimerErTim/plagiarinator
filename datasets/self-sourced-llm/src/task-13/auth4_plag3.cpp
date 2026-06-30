#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
// borrowed from roommate's homework

namespace pipes {

template <typename Elem>
class SyncQueue {
    std::queue<Elem> bucket_;
    std::size_t bound_;
    mutable std::mutex guard_;
    std::condition_variable pushOk_;
    std::condition_variable popOk_;

public:
    explicit SyncQueue(std::size_t bound) : bound_(bound) {}

    void push(Elem val) {
        std::unique_lock<std::mutex> ul(guard_);
        pushOk_.wait(ul, [&] { return bucket_.size() < bound_; });
        bucket_.push(std::move(val));
        popOk_.notify_one();
    }

    Elem pop() {
        std::unique_lock<std::mutex> ul(guard_);
        popOk_.wait(ul, [&] { return !bucket_.empty(); });
        Elem v = std::move(bucket_.front());
        bucket_.pop();
        pushOk_.notify_one();
        return v;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> g(guard_);
        return bucket_.size();
    }
};

}  // namespace pipes

int main() {
    using pipes::SyncQueue;

    std::size_t prodN = 0, consN = 0, itemN = 0;
    std::string tok;
    std::cin >> tok >> prodN >> tok >> consN >> tok >> itemN;

    SyncQueue<int> hub(itemN + 1);
    std::vector<std::thread> crew;
    std::mutex tallyMu;
    std::size_t recv = 0;

    int seq = 0;
    for (std::size_t i = 0; i < prodN; ++i) {
        std::size_t amt = itemN / prodN + (i < itemN % prodN ? 1 : 0);
        int base = seq;
        seq += static_cast<int>(amt);
        crew.emplace_back([&, amt, base]() {
            for (std::size_t j = amt; j-- > 0;)
                hub.push(base + static_cast<int>(j));
        });
    }

    for (std::size_t i = 0; i < consN; ++i) {
        crew.emplace_back([&]() {
            for (;;) {
                int v = hub.pop();
                if (v == -1) return;
                std::cout << "got " << v << '\n';
                bool finished = false;
                {
                    std::lock_guard<std::mutex> lk(tallyMu);
                    ++recv;
                    finished = (recv == itemN);
                }
                if (finished) {
                    for (std::size_t t = consN; t-- > 0;)
                        hub.push(-1);
                }
            }
        });
    }

    for (auto& th : crew) th.join();
    std::cout << "DONE\n";
    return 0;
}
