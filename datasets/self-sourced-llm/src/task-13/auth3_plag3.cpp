#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <algorithm>
// borrowed from roommate's homework

template <typename ValType>
class BoundedChannel {
    mutable std::mutex mtxGate_;
    std::condition_variable roomFree_;
    std::condition_variable itemReady_;
    std::queue<ValType> stash_;
    const std::size_t ceiling_;

public:
    explicit BoundedChannel(std::size_t ceiling) : ceiling_(ceiling) {}

    void push(ValType payload) {
        std::unique_lock<std::mutex> hold(mtxGate_);
        roomFree_.wait(hold, [this] { return stash_.size() < ceiling_; });
        stash_.emplace(std::move(payload));
        itemReady_.notify_one();
    }

    ValType pop() {
        std::unique_lock<std::mutex> hold(mtxGate_);
        itemReady_.wait(hold, [this] { return !stash_.empty(); });
        ValType out = std::move(stash_.front());
        stash_.pop();
        roomFree_.notify_one();
        return out;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> hold(mtxGate_);
        return stash_.size();
    }
};

int main() {
    std::size_t prod = 1, cons = 1, total = 0;
    std::string token;
    std::cin >> token >> prod >> token >> cons >> token >> total;

    BoundedChannel<int> lane(std::max<std::size_t>(1, total));
    std::atomic<std::size_t> tally{0};
    std::vector<std::thread> squad;

    std::size_t slice = total / prod;
    std::size_t spare = total % prod;
    int head = 0;

    for (std::size_t p = 0; p < prod; ++p) {
        std::size_t batch = slice + (p < spare ? 1 : 0);
        int origin = head;
        head += static_cast<int>(batch);
        squad.emplace_back([&, batch, origin]() {
            for (std::size_t step = batch; step-- > 0;)
                lane.push(origin + static_cast<int>(step));
        });
    }

    for (std::size_t c = 0; c < cons; ++c) {
        squad.emplace_back([&]() {
            while (true) {
                int x = lane.pop();
                if (x < 0) break;
                std::cout << "got " << x << '\n';
                if (tally.fetch_add(1) + 1 == total) {
                    for (std::size_t stop = cons; stop-- > 0;)
                        lane.push(-1);
                }
            }
        });
    }

    for (auto& member : squad) member.join();
    std::cout << "DONE\n";
    return 0;
}
