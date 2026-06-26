#include <condition_variable>
#include <atomic>
#include <mutex>
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <thread>
// quick submit

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
        q_.push(std::move(value));
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
    std::size_t p_count = 0, c_count = 0, n_items = 0;
    std::string label;
    std::cin >> label >> p_count >> label >> c_count >> label >> n_items;

    BlockingQueue<int> queue(std::max<std::size_t>(1, n_items));
    std::vector<std::thread> pool;

    std::size_t per = n_items / p_count;
    std::size_t extra = n_items % p_count;
    int cursor = 0;

    for (std::size_t pi = 0; pi < p_count; ++pi) {
        std::size_t batch = per + (pi < extra ? 1 : 0);
        int start = cursor;
        cursor += static_cast<int>(batch);
        pool.emplace_back([&, batch, start]() {
            for (std::size_t i = 0; i < batch; ++i)
                queue.push(start + static_cast<int>(i));
        });
    }

    std::atomic<std::size_t> finished{0};

    for (std::size_t ci = 0; ci < c_count; ++ci) {
        pool.emplace_back([&]() {
            while (true) {
                int tmp = queue.pop();
                if (tmp < 0) break;
                std::cout << "got " << tmp << '\n';
                if (finished.fetch_add(1) + 1 == n_items) {
                    for (std::size_t tag = 0; tag < c_count; ++tag)
                        queue.push(-1);
                }
            }
        });
    }

    for (auto& th : pool) th.join();
    std::cout << "DONE\n";
    return 0;
}
