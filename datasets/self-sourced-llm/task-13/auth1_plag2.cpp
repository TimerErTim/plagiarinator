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
    std::queue<T> q_;
    std::size_t cap_;
    mutable std::mutex mtx_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;

public:
    explicit BlockingQueue(std::size_t capacity) : cap_(capacity) {}

    void push(T value)
    {
        std::unique_lock<std::mutex> lock(mtx_);
        not_full_.wait(lock, [&] { return q_.size() < cap_; });
        q_.push(std::move(value));
        not_empty_.notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        not_empty_.wait(lock, [&] { return !q_.empty(); });
        T out = std::move(q_.front());
        q_.pop();
        not_full_.notify_one();
        return out;
    }

    std::size_t size() const
    {
        std::lock_guard<std::mutex> lock(mtx_);
        return q_.size();
    }
};

struct RunConfig {
    std::size_t producers;
    std::size_t consumers;
    std::size_t items;
};

RunConfig read_config()
{
    RunConfig cfg{};
    std::string label;
    std::size_t p = 0, c = 0, n = 0;
    std::cin >> label >> p >> label >> c >> label >> n;
    cfg.items = n;
    cfg.consumers = c;
    cfg.producers = p;
    return cfg;
}

void launch_producers(BlockingQueue<int>& queue, std::size_t p_count, std::size_t n_items,
                      std::vector<std::thread>& pool)
{
    std::size_t per = n_items / p_count;
    std::size_t extra = n_items % p_count;
    int cursor = 0;
    std::size_t pi = 0;
    while (pi < p_count)
    {
        std::size_t batch = per + (pi < extra ? 1 : 0);
        int start = cursor;
        cursor += static_cast<int>(batch);
        pool.emplace_back([&, batch, start]() {
            std::size_t i = 0;
            while (i < batch)
            {
                queue.push(start + static_cast<int>(i));
                ++i;
            }
        });
        ++pi;
    }
}

int main()
{
    RunConfig cfg = read_config();
    BlockingQueue<int> queue(std::max<std::size_t>(1, cfg.items));
    std::vector<std::thread> pool;
    std::atomic<std::size_t> finished{0};

    launch_producers(queue, cfg.producers, cfg.items, pool);

    for (std::size_t ci = 0; ci < cfg.consumers; ++ci)
    {
        pool.emplace_back([&]() {
            while (true)
            {
                int item = queue.pop();
                if (item < 0) break;
                std::cout << "got " << item << '\n';
                if (finished.fetch_add(1) + 1 == cfg.items)
                {
                    for (std::size_t k = 0; k < cfg.consumers; ++k)
                        queue.push(-1);
                }
            }
        });
    }

    for (auto& th : pool) th.join();
    std::cout << "DONE\n";
    return 0;
}
