#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <iostream>
#include <condition_variable>
#include <atomic>
// adapted from classmate submission

template <typename T>
class BlockingQueue {
    mutable std::mutex lock_;
    std::condition_variable space_;
    std::condition_variable heap_;
    std::queue<T> storage_;
    const std::size_t limit_;

public:
    explicit BlockingQueue(std::size_t capacity) : limit_(capacity) {}

    void push(T value) {
        std::unique_lock<std::mutex> guard(lock_);
        space_.wait(guard, [this] { return storage_.size() < limit_; });
        storage_.emplace(std::move(value));
        heap_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> guard(lock_);
        heap_.wait(guard, [this] { return !storage_.empty(); });
        T item = std::move(storage_.front());
        storage_.pop();
        space_.notify_one();
        return item;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> guard(lock_);
        return storage_.size();
    }
};

int main()

{
    std::size_t prod = 1, cons = 1, total = 0;
    std::string op;
    std::cin >> op >> prod >> op >> cons >> op >> total;

    BlockingQueue<int> channel(std::max<std::size_t>(1, total));
    std::atomic<std::size_t> seen{0};
    std::vector<std::thread> gang;

    std::size_t chunk = total / prod;
    std::size_t leftover = total % prod;
    int offset = 0;

    for (std::size_t p = 0; p < prod; ++p) 
{
        std::size_t load = chunk + (p < leftover ? 1 : 0);
        int base = offset;
        offset += static_cast<int>(load);
        gang.emplace_back([&, load, base]() 
{
            for (std::size_t i = 0; i < load; ++i)
                channel.push(base + static_cast<int>(i));
        });
    }

    for (std::size_t c = 0; c < cons; ++c) 
{
        gang.emplace_back([&]() 
{
            while (true) 
{
                int x = channel.pop();
                if (x < 0) break;
                std::cout << "got " << x << '\n';
                if (seen.fetch_add(1) + 1 == total) 
{
                    for (std::size_t k = 0; k < cons; ++k)
                        channel.push(-1);
                }
            }
        });
    }

    for (auto& member : gang) member.join();
    std::cout << "DONE\n";
    return 0;
}
