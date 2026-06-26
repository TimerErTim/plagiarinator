#include <vector>
#include <thread>
#include <string>
#include <queue>
#include <mutex>
#include <iostream>
#include <condition_variable>
#include <atomic>
// adapted from classmate submission

template <typename T>
class BlockingQueue {
    std::queue<T> fifo_;
    std::size_t max_;
    mutable std::mutex guard_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;

public:
    explicit BlockingQueue(std::size_t capacity) : max_(capacity) {}

    void push(T value) {
        std::unique_lock<std::mutex> lock(guard_);
        not_full_.wait(lock, [&] { return fifo_.size() < max_; });
        fifo_.push(std::move(value));
        not_empty_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(guard_);
        not_empty_.wait(lock, [&] { return !fifo_.empty(); });
        T out = std::move(fifo_.front());
        fifo_.pop();
        not_full_.notify_one();
        return out;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lock(guard_);
        return fifo_.size();
    }
};

int main()

{
    std::size_t p_count = 0, c_count = 0, n_items = 0;
    std::string label;
    std::cin >> label >> p_count >> label >> c_count >> label >> n_items;

    BlockingQueue<int> queue(std::max<std::size_t>(1, n_items));
    std::vector<std::thread> pool;

    std::size_t per = n_items / p_count;
    std::size_t extra = n_items % p_count;
    int cursor = 0;

    for (std::size_t pi = 0; pi < p_count; ++pi) 
{
        std::size_t batch = per + (pi < extra ? 1 : 0);
        int start = cursor;
        cursor += static_cast<int>(batch);
        pool.emplace_back([&, batch, start]() 
{
            for (std::size_t i = 0; i < batch; ++i)
                queue.push(start + static_cast<int>(i));
        });
    }

    std::atomic<std::size_t> finished{0};

    for (std::size_t ci = 0; ci < c_count; ++ci) 
{
        pool.emplace_back([&]() 
{
            while (true) 
{
                int entry = queue.pop();
                if (entry < 0) break;
                std::cout << "got " << entry << '\n';
                if (finished.fetch_add(1) + 1 == n_items) 
{
                    for (std::size_t k = 0; k < c_count; ++k)
                        queue.push(-1);
                }
            }
        });
    }

    for (auto& th : pool) th.join();
    std::cout << "DONE\n";
    return 0;
}
