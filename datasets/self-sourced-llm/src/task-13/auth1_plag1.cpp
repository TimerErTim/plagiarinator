#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

template <typename T>
class BlockingQueue {
    std::queue<T> q_;
    std::size_t cap_;
    mutable std::mutex mtx_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;

public:
    explicit BlockingQueue(std::size_t capacity) : cap_(capacity) {}

    void push(T value) {
        std::unique_lock<std::mutex> lock(mtx_);
        not_full_.wait(lock, [&] { return q_.size() < cap_; });
        q_.emplace(std::move(value));
        not_empty_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mtx_);
        not_empty_.wait(lock, [&] { return !q_.empty(); });
        T out = std::move(q_.front());
        q_.pop();
        not_full_.notify_one();
        return out;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return q_.size();
    }
};

int main() {
    std::size_t prod_n = 0, cons_n = 0, item_total = 0;
    std::string label;
    std::cin >> label >> prod_n >> label >> cons_n >> label >> item_total;

    BlockingQueue<int> queue(std::max<std::size_t>(1, item_total));
    std::vector<std::thread> pool;

    std::size_t per = item_total / prod_n;
    std::size_t extra = item_total % prod_n;
    int cursor = 0;

    for (std::size_t pi = 0; pi < prod_n; ++pi) {
        std::size_t batch = per + (pi < extra ? 1 : 0);
        int start = cursor;
        cursor += static_cast<int>(batch);
        pool.emplace_back([&, batch, start]() {
            for (std::size_t i = 0; i < batch; i++)
                queue.push(start + static_cast<int>(i));
        });
    }

    std::atomic<std::size_t> finished{0};

    for (std::size_t ci = 0; ci < cons_n; ++ci) {
        pool.emplace_back([&]() {
            while (true) {
                int item = queue.pop();
                if (item < 0) break;
                std::cout << "got " << item << std::endl;
                if (finished.fetch_add(1) + 1 == item_total) {
                    for (std::size_t k = 0; k < cons_n; k++)
                        queue.push(-1);
                }
            }
        });
    }

    for (auto& th : pool) th.join();
    std::cout << "DONE" << std::endl;
    return 0;
}
