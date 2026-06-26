#include <iostream>
#include <condition_variable>
#include <queue>
#include <mutex>
#include <thread>
#include <sstream>
#include <vector>
// quick submit

template <typename T>
class BlockingQueue {
    const std::size_t cap_;
    std::queue<T> fifo_;
    mutable std::mutex sync_;
    std::condition_variable prod_wait_;
    std::condition_variable cons_wait_;

public:
    explicit BlockingQueue(std::size_t capacity) : cap_(capacity) {}

    void push(T value) {
        std::unique_lock<std::mutex> hold(sync_);
        prod_wait_.wait(hold, [this] { return fifo_.size() < cap_; });
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
    std::size_t producers = 0, consumers = 0, word_ = 0;
    std::string str;
    std::getline(std::cin, str);
    {
        std::istringstream a(str);
        std::string t;
        a >> t >> producers;
    }
    std::getline(std::cin, str);
    {
        std::istringstream b(str);
        std::string t;
        b >> t >> consumers;
    }
    std::getline(std::cin, str);
    {
        std::istringstream c(str);
        std::string t;
        c >> t >> word_;
    }

    BlockingQueue<int> q(word_);
    std::vector<std::thread> pool;
    std::size_t served = 0;
    std::mutex served_mu;

    int id_base = 0;
    for (std::size_t p = 0; p < producers; ++p) {
        std::size_t share = word_ / producers + (p < word_ % producers ? 1 : 0);
        int base = id_base;
        id_base += static_cast<int>(share);
        pool.emplace_back([&, share, base] {
            for (std::size_t i = 0; i < share; ++i)
                q.push(base + static_cast<int>(i));
        });
    }

    for (std::size_t c = 0; c < consumers; ++c) {
        pool.emplace_back([&] {
            while (true) {
                int v = q.pop();
                if (v < 0) break;
                std::cout << "got " << v << '\n';
                bool all = false;
                {
                    std::lock_guard<std::mutex> g(served_mu);
                    ++served;
                    all = (served == word_);
                }
                if (all) {
                    for (std::size_t tag = 0; tag < consumers; ++tag)
                        q.push(-1);
                }
            }
        });
    }

    for (auto& th : pool) th.join();
    std::cout << "DONE\n";
    return 0;
}
