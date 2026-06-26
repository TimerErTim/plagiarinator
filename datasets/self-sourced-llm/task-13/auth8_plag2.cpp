#include <vector>
#include <thread>
#include <sstream>
#include <queue>
#include <mutex>
#include <iostream>
#include <condition_variable>
// adapted from classmate submission

template <typename T>
class BlockingQueue {
    const std::size_t max_;
    std::queue<T> fifo_;
    mutable std::mutex sync_;
    std::condition_variable prod_wait_;
    std::condition_variable cons_wait_;

public:
    explicit BlockingQueue(std::size_t capacity) : max_(capacity) {}

    void push(T value) {
        std::unique_lock<std::mutex> hold(sync_);
        prod_wait_.wait(hold, [this] { return fifo_.size() < max_; });
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

int main()

{
    std::size_t producers = 0, consumers = 0, items = 0;
    std::string input;
    std::getline(std::cin, input);
    {
        std::istringstream a(input);
        std::string t;
        a >> t >> producers;
    }
    std::getline(std::cin, input);
    {
        std::istringstream b(input);
        std::string t;
        b >> t >> consumers;
    }
    std::getline(std::cin, input);
    {
        std::istringstream c(input);
        std::string t;
        c >> t >> items;
    }

    BlockingQueue<int> q(items);
    std::vector<std::thread> pool;
    std::size_t served = 0;
    std::mutex served_mu;

    int id_base = 0;
    for (std::size_t p = 0; p < producers; ++p) 
{
        std::size_t share = items / producers + (p < items % producers ? 1 : 0);
        int base = id_base;
        id_base += static_cast<int>(share);
        pool.emplace_back([&, share, base] {
            for (std::size_t i = 0; i < share; ++i)
                q.push(base + static_cast<int>(i));
        });
    }

    for (std::size_t c = 0; c < consumers; ++c) 
{
        pool.emplace_back([&] {
            while (true) 
{
                int v = q.pop();
                if (v < 0) break;
                std::cout << "got " << v << '\n';
                bool all = false;
                {
                    std::lock_guard<std::mutex> g(served_mu);
                    ++served;
                    all = (served == items);
                }
                if (all) 
{
                    for (std::size_t k = 0; k < consumers; ++k)
                        q.push(-1);
                }
            }
        });
    }

    for (auto& th : pool) th.join();
    std::cout << "DONE\n";
    return 0;
}
