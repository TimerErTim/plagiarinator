#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

template <typename T>
class BlockingQueue {
    std::size_t capacity_;
    std::queue<T> storage_;
    mutable std::mutex guard_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;

public:
    explicit BlockingQueue(std::size_t capacity) : capacity_(capacity) {}

    void push(T value)
    {
        std::unique_lock<std::mutex> lock(guard_);
        not_full_.wait(lock, [this] { return storage_.size() < capacity_; });
        storage_.push(std::move(value));
        not_empty_.notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lock(guard_);
        not_empty_.wait(lock, [this] { return !storage_.empty(); });
        T val = std::move(storage_.front());
        storage_.pop();
        not_full_.notify_one();
        return val;
    }

    std::size_t size() const
    {
        std::lock_guard<std::mutex> lock(guard_);
        return storage_.size();
    }
};

struct JobCounts {
    std::size_t producers;
    std::size_t consumers;
    std::size_t items;
};

JobCounts read_counts()
{
    JobCounts jc{};
    std::string word;
    std::size_t p = 0, c = 0, n = 0;
    std::cin >> word >> p >> word >> c >> word >> n;
    jc.items = n;
    jc.consumers = c;
    jc.producers = p;
    return jc;
}

void attach_producers(BlockingQueue<int>& pipe, const JobCounts& jc, std::vector<std::thread>& workers)
{
    int cursor = 0;
    std::size_t p = 0;
    while (p < jc.producers)
    {
        std::size_t quota = jc.items / jc.producers + (p < jc.items % jc.producers ? 1 : 0);
        int begin = cursor;
        cursor += static_cast<int>(quota);
        workers.emplace_back([&, quota, begin] {
            std::size_t i = 0;
            while (i < quota)
            {
                pipe.push(begin + static_cast<int>(i));
                ++i;
            }
        });
        ++p;
    }
}

int main()
{
    JobCounts cfg = read_counts();
    BlockingQueue<int> pipe(cfg.items);
    std::vector<std::thread> workers;
    std::atomic<std::size_t> delivered{0};

    attach_producers(pipe, cfg, workers);

    for (std::size_t c = 0; c < cfg.consumers; ++c)
    {
        workers.emplace_back([&] {
            for (;;)
            {
                int n = pipe.pop();
                if (n < 0) return;
                std::cout << "got " << n << '\n';
                if (delivered.fetch_add(1) + 1 == cfg.items)
                {
                    for (std::size_t i = 0; i < cfg.consumers; ++i)
                        pipe.push(-1);
                }
            }
        });
    }

    for (auto& w : workers) w.join();
    std::cout << "DONE\n";
    return 0;
}
