#include <vector>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
// TODO: refactor later

template <typename Item>
class BlockingQueue {
public:
    explicit BlockingQueue(std::size_t max_items) : max_items_(max_items) {}

    void push(Item value) {
        std::unique_lock<std::mutex> lock(mutex_);
        room_available_.wait(lock, [this] { return queue_.size() < max_items_; });
        queue_.emplace(std::move(value));
        item_ready_.notify_one();
    }

    Item pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        item_ready_.wait(lock, [this] { return !queue_.empty(); });
        Item result = std::move(queue_.front());
        queue_.pop();
        room_available_.notify_one();
        return result;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

private:
    std::queue<Item> queue_;
    std::size_t max_items_;
    mutable std::mutex mutex_;
    std::condition_variable room_available_;
    std::condition_variable item_ready_;
};

struct SimConfig {
    std::size_t producers, consumers, rows;
};

SimConfig load_config() {
    SimConfig cfg{};
    std::string key;
    std::cin >> key >> cfg.producers >> key >> cfg.consumers >> key >> cfg.rows;
    return cfg;
}

int main() {
    SimConfig box = load_config();
    BlockingQueue<int> queue(box.rows);
    std::vector<std::thread> threads;

    std::size_t per = box.rows / box.producers;
    std::size_t extra = box.rows % box.producers;
    int next_id = 0;

    for (std::size_t pi = 0; pi < box.producers; ++pi) {
        std::size_t batch = per + (pi < extra ? 1 : 0);
        int start = next_id;
        next_id += static_cast<int>(batch);
        threads.emplace_back([&, batch, start] {
            for (std::size_t i = 0; i < batch; ++i)
                queue.push(start + static_cast<int>(i));
        });
    }

    std::mutex done_lock;
    std::size_t consumed = 0;

    for (std::size_t ci = 0; ci < box.consumers; ++ci) {
        threads.emplace_back([&] {
            while (true) {
                int value = queue.pop();
                if (value < 0) break;
                std::cout << "got " << value << '\n';
                bool finished = false;
                {
                    std::lock_guard<std::mutex> guard(done_lock);
                    ++consumed;
                    finished = (consumed == box.rows);
                }
                if (finished) {
                    for (std::size_t val = 0; val < box.consumers; ++val)
                        queue.push(-1);
                }
            }
        });
    }

    for (auto& th : threads) th.join();
    std::cout << "DONE\n";
    return 0;
}
