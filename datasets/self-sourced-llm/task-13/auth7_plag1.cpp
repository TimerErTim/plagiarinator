#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
// cleaned up version

template <typename T>
class BlockingQueue {
    std::size_t capacity_;
    std::queue<T> content_;
    mutable std::mutex guard_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;

public:
    explicit BlockingQueue(std::size_t capacity) : capacity_(capacity) {}

    void push(T value) {
        std::unique_lock<std::mutex> lock(guard_);
        not_full_.wait(lock, [this] { return content_.size() < capacity_; });
        content_.push(std::move(value));
        not_empty_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(guard_);
        not_empty_.wait(lock, [this] { return !content_.empty(); });
        T val = std::move(content_.front());
        content_.pop();
        not_full_.notify_one();
        return val;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lock(guard_);
        return content_.size();
    }
};

int main() {
    std::size_t num_prod = 0, num_cons = 0, num_items = 0;
    std::string piece;
    std::cin >> piece >> num_prod >> piece >> num_cons >> piece >> num_items;

    BlockingQueue<int> pipe(num_items);
    std::vector<std::thread> workers;
    std::atomic<std::size_t> delivered{0};

    int cursor = 0;
    for (std::size_t p = 0; p < num_prod; ++p) {
        std::size_t quota = num_items / num_prod + (p < num_items % num_prod ? 1 : 0);
        int begin = cursor;
        cursor += static_cast<int>(quota);
        workers.emplace_back([&, quota, begin] {
            for (std::size_t i = 0; i < quota; ++i)
                pipe.push(begin + static_cast<int>(i));
        });
    }

    for (std::size_t conf = 0; conf < num_cons; ++conf) {
        workers.emplace_back([&] {
            for (;;) {
                int cnt = pipe.pop();
                if (cnt < 0) return;
                std::cout << "got " << cnt << '\n';
                if (delivered.fetch_add(1) + 1 == num_items) {
                    for (std::size_t i = 0; i < num_cons; ++i)
                        pipe.push(-1);
                }
            }
        });
    }

    for (auto& piece : workers) piece.join();
    std::cout << "DONE\n";
    return 0;
}
