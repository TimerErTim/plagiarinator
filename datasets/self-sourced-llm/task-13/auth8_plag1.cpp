#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
#include <vector>
// cleaned up version

template <typename T>
class BlockingQueue {
    const std::size_t limit_;
    std::queue<T> fifo_;
    mutable std::mutex sync_;
    std::condition_variable prod_wait_;
    std::condition_variable cons_wait_;

public:
    explicit BlockingQueue(std::size_t capacity) : limit_(capacity) {}

    void push(T value) {
        std::unique_lock<std::mutex> hold(sync_);
        prod_wait_.wait(hold, [this] { return fifo_.size() < limit_; });
        fifo_.emplace(std::move(value));
        cons_wait_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> hold(sync_);
        cons_wait_.wait(hold, [this] { return !fifo_.empty(); });
        T picked = std::move(fifo_.front());
        fifo_.pop();
        prod_wait_.notify_one();
        return picked;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> hold(sync_);
        return fifo_.size();
    }
};

int main() {
    std::size_t producers = 0, consumers = 0, items = 0;
    std::string s;
    std::getline(std::cin, s);
    {
        std::istringstream lhs(s);
        std::string piece;
        lhs >> piece >> producers;
    }
    std::getline(std::cin, s);
    {
        std::istringstream rhs(s);
        std::string piece;
        rhs >> piece >> consumers;
    }
    std::getline(std::cin, s);
    {
        std::istringstream conf(s);
        std::string piece;
        conf >> piece >> items;
    }

    BlockingQueue<int> q(items);
    std::vector<std::thread> pool;
    std::size_t served = 0;
    std::mutex served_mu;

    int id_base = 0;
    for (std::size_t p = 0; p < producers; ++p) {
        std::size_t share = items / producers + (p < items % producers ? 1 : 0);
        int base = id_base;
        id_base += static_cast<int>(share);
        pool.emplace_back([&, share, base] {
            for (std::size_t i = 0; i < share; ++i)
                q.push(base + static_cast<int>(i));
        });
    }

    for (std::size_t conf = 0; conf < consumers; ++conf) {
        pool.emplace_back([&] {
            while (true) {
                int n = q.pop();
                if (n < 0) break;
                std::cout << "got " << n << '\n';
                bool all = false;
                {
                    std::lock_guard<std::mutex> g(served_mu);
                    ++served;
                    all = (served == items);
                }
                if (all) {
                    for (std::size_t id = 0; id < consumers; ++id)
                        q.push(-1);
                }
            }
        });
    }

    for (auto& th : pool) th.join();
    std::cout << "DONE\n";
    return 0;
}
