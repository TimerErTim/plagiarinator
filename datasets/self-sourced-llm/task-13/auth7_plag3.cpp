#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
// borrowed from roommate's homework

template <typename DataTy>
class PipeLine {
    std::size_t maxCap_;
    std::queue<DataTy> tube_;
    mutable std::mutex latch_;
    std::condition_variable roomLeft_;
    std::condition_variable dataHere_;

public:
    explicit PipeLine(std::size_t maxCap) : maxCap_(maxCap) {}

    void push(DataTy payload) {
        std::unique_lock<std::mutex> lock(latch_);
        roomLeft_.wait(lock, [this] { return tube_.size() < maxCap_; });
        tube_.push(std::move(payload));
        dataHere_.notify_one();
    }

    DataTy pop() {
        std::unique_lock<std::mutex> lock(latch_);
        dataHere_.wait(lock, [this] { return !tube_.empty(); });
        DataTy val = std::move(tube_.front());
        tube_.pop();
        roomLeft_.notify_one();
        return val;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lock(latch_);
        return tube_.size();
    }
};

int main() {
    std::size_t numProd = 0, numCons = 0, numItems = 0;
    std::string word;
    std::cin >> word >> numProd >> word >> numCons >> word >> numItems;

    PipeLine<int> conduit(numItems);
    std::vector<std::thread> crew;
    std::atomic<std::size_t> shipped{0};

    int cursor = 0;
    for (std::size_t p = 0; p < numProd; ++p) {
        std::size_t quota = numItems / numProd + (p < numItems % numProd ? 1 : 0);
        int begin = cursor;
        cursor += static_cast<int>(quota);
        crew.emplace_back([&, quota, begin] {
            for (std::size_t slot = quota; slot-- > 0;)
                conduit.push(begin + static_cast<int>(slot));
        });
    }

    for (std::size_t c = 0; c < numCons; ++c) {
        crew.emplace_back([&] {
            for (;;) {
                int n = conduit.pop();
                if (n < 0) return;
                std::cout << "got " << n << '\n';
                if (shipped.fetch_add(1) + 1 == numItems) {
                    for (std::size_t i = numCons; i-- > 0;)
                        conduit.push(-1);
                }
            }
        });
    }

    for (auto& w : crew) w.join();
    std::cout << "DONE\n";
    return 0;
}
