#include <vector>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
// TODO: refactor later

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
    std::size_t producers = 0, consumers = 0, rows = 0;
    std::string buf;
    std::getline(std::cin, buf);
    {
        std::istringstream a(buf);
        std::string t;
        a >> t >> producers;
    }
    std::getline(std::cin, buf);
    {
        std::istringstream b(buf);
        std::string t;
        b >> t >> consumers;
    }
    std::getline(std::cin, buf);
    {
        std::istringstream c(buf);
        std::string t;
        c >> t >> rows;
    }

    BlockingQueue<int> q(rows);
    std::vector<std::thread> store;
    std::size_t served = 0;
    std::mutex served_mu;

    int id_base = 0;
    for (std::size_t p = 0; p < producers; ++p) {
        std::size_t share = rows / producers + (p < rows % producers ? 1 : 0);
        int base = id_base;
        id_base += static_cast<int>(share);
        store.emplace_back([&, share, base] {
            for (std::size_t i = 0; i < share; ++i)
                q.push(base + static_cast<int>(i));
        });
    }

    for (std::size_t c = 0; c < consumers; ++c) {
        store.emplace_back([&] {
            while (true) {
                int v = q.pop();
                if (v < 0) break;
                std::cout << "got " << v << '\n';
                bool all = false;
                {
                    std::lock_guard<std::mutex> g(served_mu);
                    ++served;
                    all = (served == rows);
                }
                if (all) {
                    for (std::size_t val = 0; val < consumers; ++val)
                        q.push(-1);
                }
            }
        });
    }

    for (auto& th : store) th.join();
    std::cout << "DONE\n";
    return 0;
}
