#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>

template <typename T>
class BlockingQueue {
    std::queue<T> fifo_;
    std::size_t limit_;
    mutable std::mutex lock_;
    std::condition_variable space_cv_;
    std::condition_variable data_cv_;

public:
    explicit BlockingQueue(std::size_t capacity) : limit_(capacity) {}

    void push(T value) {
        std::unique_lock<std::mutex> lk(lock_);
        space_cv_.wait(lk, [&] { return fifo_.size() < limit_; });
        fifo_.push(std::move(value));
        data_cv_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lk(lock_);
        data_cv_.wait(lk, [&] { return !fifo_.empty(); });
        T val = std::move(fifo_.front());
        fifo_.pop();
        space_cv_.notify_one();
        return val;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lk(lock_);
        return fifo_.size();
    }
};

int main() {
    std::size_t p_count = 0, c_count = 0, n_items = 0;
    std::string label;
    std::cin >> label >> p_count >> label >> c_count >> label >> n_items;

    BlockingQueue<int> fifo(std::max<std::size_t>(1, n_items));
    std::vector<std::thread> threads;

    std::size_t per = n_items / p_count;
    std::size_t extra = n_items % p_count;
    int cursor = 0;

    for (std::size_t pi = 0; pi < p_count; ++pi) {
        std::size_t batch = per + (pi < extra ? 1 : 0);
        int start = cursor;
        cursor += static_cast<int>(batch);
        threads.emplace_back([&, batch, start]() {
            for (std::size_t i = 0; i < batch; ++i)
                fifo.push(start + static_cast<int>(i));
        });
    }

    std::atomic<std::size_t> done_count{0};

    for (std::size_t ci = 0; ci < c_count; ++ci) {
        threads.emplace_back([&]() {
            while (true) {
                int item = fifo.pop();
                if (item < 0) break;
                std::cout << "got " << item << '\n';
                if (done_count.fetch_add(1) + 1 == n_items) {
                    for (std::size_t k = 0; k < c_count; ++k)
                        fifo.push(-1);
                }
            }
        });
    }

    for (auto& th : threads) th.join();
    std::cout << "DONE\n";
    return 0;
}
