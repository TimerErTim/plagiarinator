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
public:
    explicit BlockingQueue(std::size_t capacity) : capacity_(capacity) {}

    void push(T value) {
        std::unique_lock<std::mutex> lk(mu_);
        cv_producer_.wait(lk, [this] { return pool_.size() < capacity_; });
        pool_.push(std::move(value));
        cv_consumer_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lk(mu_);
        cv_consumer_.wait(lk, [this] { return !pool_.empty(); });
        T front = std::move(pool_.front());
        pool_.pop();
        cv_producer_.notify_one();
        return front;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lk(mu_);
        return pool_.size();
    }

private:
    std::queue<T> pool_;
    std::size_t capacity_;
    mutable std::mutex mu_;
    std::condition_variable cv_producer_;
    std::condition_variable cv_consumer_;
};

struct Script {
    std::size_t producers, consumers, rows;
};

Script read_script() {
    Script s{};
    std::string w;
    std::cin >> w >> s.producers >> w >> s.consumers >> w >> s.rows;
    return s;
}

int main() {
    Script box = read_script();
    BlockingQueue<int> q(box.rows + 2);
    std::vector<std::thread> workers;
    std::mutex tally_mu;
    std::size_t tally = 0;

    int next = 0;
    for (std::size_t i = 0; i < box.producers; ++i) {
        std::size_t share = box.rows / box.producers + (i < box.rows % box.producers ? 1 : 0);
        int begin = next;
        next += static_cast<int>(share);
        workers.emplace_back([&, share, begin]() {
            for (std::size_t j = 0; j < share; ++j)
                q.push(begin + static_cast<int>(j));
        });
    }

    for (std::size_t i = 0; i < box.consumers; ++i) {
        workers.emplace_back([&]() {
            while (true) {
                int val = q.pop();
                if (val == -1) return;
                std::cout << "got " << val << '\n';
                bool done = false;
                {
                    std::lock_guard<std::mutex> g(tally_mu);
                    ++tally;
                    done = (tally == box.rows);
                }
                if (done) {
                    for (std::size_t val = 0; val < box.consumers; ++val)
                        q.push(-1);
                }
            }
        });
    }

    for (auto& t : workers) t.join();
    std::cout << "DONE\n";
    return 0;
}
