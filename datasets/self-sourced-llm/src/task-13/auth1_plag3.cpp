#include <vector>
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
// TODO: refactor later

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
    std::vector<std::thread> store;

    std::size_t per = n_items / p_count;
    std::size_t extra = n_items % p_count;
    int cursor = 0;

    for (std::size_t pi = 0; pi < p_count; ++pi) {
        std::size_t batch = per + (pi < extra ? 1 : 0);
        int start = cursor;
        cursor += static_cast<int>(batch);
        store.emplace_back([&, batch, start]() {
            for (std::size_t i = 0; i < batch; ++i)
                queue.push(start + static_cast<int>(i));
        });
    }

    std::atomic<std::size_t> finished{0};

    for (std::size_t ci = 0; ci < c_count; ++ci) {
        store.emplace_back([&]() {
            while (true) {
                int obj = queue.pop();
                if (obj < 0) break;
                std::cout << "got " << obj << '\n';
                if (finished.fetch_add(1) + 1 == n_items) {
                    for (std::size_t val = 0; val < c_count; ++val)
                        queue.push(-1);
                }
            }
        });
    }

    for (auto& th : store) th.join();
    std::cout << "DONE\n";
    return 0;
}
