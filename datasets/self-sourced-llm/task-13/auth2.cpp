#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
#include <vector>

template <typename T>
class BlockingQueue {
public:
    explicit BlockingQueue(std::size_t capacity) : capacity_(capacity) {}

    void push(T value) {
        std::unique_lock<std::mutex> lk(mu_);
        cv_producer_.wait(lk, [this] { return data_.size() < capacity_; });
        data_.push(std::move(value));
        cv_consumer_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lk(mu_);
        cv_consumer_.wait(lk, [this] { return !data_.empty(); });
        T front = std::move(data_.front());
        data_.pop();
        cv_producer_.notify_one();
        return front;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lk(mu_);
        return data_.size();
    }

private:
    std::queue<T> data_;
    std::size_t capacity_;
    mutable std::mutex mu_;
    std::condition_variable cv_producer_;
    std::condition_variable cv_consumer_;
};

struct Script {
    std::size_t producers, consumers, items;
};

Script read_script() {
    Script s{};
    std::string w;
    std::cin >> w >> s.producers >> w >> s.consumers >> w >> s.items;
    return s;
}

int main() {
    Script cfg = read_script();
    BlockingQueue<int> q(cfg.items + 2);
    std::vector<std::thread> workers;
    std::mutex tally_mu;
    std::size_t tally = 0;

    int next = 0;
    for (std::size_t i = 0; i < cfg.producers; ++i) {
        std::size_t share = cfg.items / cfg.producers + (i < cfg.items % cfg.producers ? 1 : 0);
        int begin = next;
        next += static_cast<int>(share);
        workers.emplace_back([&, share, begin]() {
            for (std::size_t j = 0; j < share; ++j)
                q.push(begin + static_cast<int>(j));
        });
    }

    for (std::size_t i = 0; i < cfg.consumers; ++i) {
        workers.emplace_back([&]() {
            while (true) {
                int val = q.pop();
                if (val == -1) return;
                std::cout << "got " << val << '\n';
                bool done = false;
                {
                    std::lock_guard<std::mutex> g(tally_mu);
                    ++tally;
                    done = (tally == cfg.items);
                }
                if (done) {
                    for (std::size_t k = 0; k < cfg.consumers; ++k)
                        q.push(-1);
                }
            }
        });
    }

    for (auto& t : workers) t.join();
    std::cout << "DONE\n";
    return 0;
}
