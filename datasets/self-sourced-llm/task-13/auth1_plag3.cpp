#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>
// borrowed from roommate's homework

template <typename ElemTy>
class SafeFifo {
    std::queue<ElemTy> buffer_;
    std::size_t maxLen_;
    mutable std::mutex gate_;
    std::condition_variable hasRoom_;
    std::condition_variable hasData_;

public:
    explicit SafeFifo(std::size_t maxLen) : maxLen_(maxLen) {}

    void push(ElemTy payload) {
        std::unique_lock<std::mutex> lk(gate_);
        hasRoom_.wait(lk, [&] { return buffer_.size() < maxLen_; });
        buffer_.push(std::move(payload));
        hasData_.notify_one();
    }

    ElemTy pop() {
        std::unique_lock<std::mutex> lk(gate_);
        hasData_.wait(lk, [&] { return !buffer_.empty(); });
        ElemTy picked = std::move(buffer_.front());
        buffer_.pop();
        hasRoom_.notify_one();
        return picked;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lk(gate_);
        return buffer_.size();
    }
};

int main() {
    std::size_t numProd = 0, numCons = 0, totalItems = 0;
    std::string tok;
    std::cin >> tok >> numProd >> tok >> numCons >> tok >> totalItems;

    SafeFifo<int> pipe(std::max<std::size_t>(1, totalItems));
    std::vector<std::thread> crew;

    std::size_t baseShare = totalItems / numProd;
    std::size_t bonus = totalItems % numProd;
    int nextId = 0;

    for (std::size_t prodIdx = 0; prodIdx < numProd; ++prodIdx) {
        std::size_t quota = baseShare + (prodIdx < bonus ? 1 : 0);
        int idStart = nextId;
        nextId += static_cast<int>(quota);
        crew.emplace_back([&, quota, idStart]() {
            for (std::size_t slot = quota; slot-- > 0;)
                pipe.push(idStart + static_cast<int>(slot));
        });
    }

    std::atomic<std::size_t> handled{0};

    for (std::size_t consIdx = 0; consIdx < numCons; ++consIdx) {
        crew.emplace_back([&]() {
            while (true) {
                int val = pipe.pop();
                if (val < 0) break;
                std::cout << "got " << val << '\n';
                if (handled.fetch_add(1) + 1 == totalItems) {
                    for (std::size_t poison = numCons; poison-- > 0;)
                        pipe.push(-1);
                }
            }
        });
    }

    for (auto& worker : crew) worker.join();
    std::cout << "DONE\n";
    return 0;
}
