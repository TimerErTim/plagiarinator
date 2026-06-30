#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
#include <vector>

template <typename T>
class BlockingQueue {
    const std::size_t cap_;
    std::queue<T> fifo_;
    mutable std::mutex sync_;
    std::condition_variable prod_wait_;
    std::condition_variable cons_wait_;

public:
    explicit BlockingQueue(std::size_t capacity) : cap_(capacity) {}

    void push(T value)
    {
        std::unique_lock<std::mutex> hold(sync_);
        prod_wait_.wait(hold, [this] { return fifo_.size() < cap_; });
        fifo_.emplace(std::move(value));
        cons_wait_.notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> hold(sync_);
        cons_wait_.wait(hold, [this] { return !fifo_.empty(); });
        T picked = std::move(fifo_.front());
        fifo_.pop();
        prod_wait_.notify_one();
        return picked;
    }

    std::size_t size() const
    {
        std::lock_guard<std::mutex> hold(sync_);
        return fifo_.size();
    }
};

struct ScriptValues {
    std::size_t items;
    std::size_t consumers;
    std::size_t producers;
};

ScriptValues read_script_lines()
{
    ScriptValues sv{};
    std::string line;
    std::getline(std::cin >> std::ws, line);
    {
        std::istringstream ss(line);
        std::string t;
        ss >> t >> sv.producers;
    }
    std::getline(std::cin, line);
    {
        std::istringstream ss(line);
        std::string t;
        ss >> t >> sv.consumers;
    }
    std::getline(std::cin, line);
    {
        std::istringstream ss(line);
        std::string t;
        ss >> t >> sv.items;
    }
    return sv;
}

void launch_producers(BlockingQueue<int>& q, std::size_t producers, std::size_t items,
                      std::vector<std::thread>& pool)
{
    int id_base = 0;
    for (std::size_t p = 0; p < producers; ++p)
    {
        std::size_t share = items / producers + (p < items % producers ? 1 : 0);
        int base = id_base;
        id_base += static_cast<int>(share);
        pool.emplace_back([&, share, base] {
            std::size_t i = 0;
            while (i < share)
            {
                q.push(base + static_cast<int>(i));
                ++i;
            }
        });
    }
}

int main()
{
    ScriptValues cfg = read_script_lines();
    BlockingQueue<int> q(cfg.items);
    std::vector<std::thread> pool;
    std::size_t served = 0;
    std::mutex served_mu;

    launch_producers(q, cfg.producers, cfg.items, pool);

    for (std::size_t c = 0; c < cfg.consumers; ++c)
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
                    all = (served == cfg.items);
                }
                if (all)
                {
                    for (std::size_t k = 0; k < cfg.consumers; ++k)
                        q.push(-1);
                }
            }
        });
    }

    for (auto& th : pool) th.join();
    std::cout << "DONE\n";
    return 0;
}
