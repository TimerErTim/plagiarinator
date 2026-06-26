#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
#include <vector>
// cleaned up version

template <typename T>
class BlockingQueue {
public:
    explicit BlockingQueue(std::size_t capacity) : capacity_(capacity) {}

    void push(T value) {
        std::unique_lock<std::mutex> lk(mu_);
        cv_producer_.wait(lk, [this] { return elems_.size() < capacity_; });
        elems_.push(std::move(value));
        cv_consumer_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lk(mu_);
        cv_consumer_.wait(lk, [this] { return !elems_.empty(); });
        T front = std::move(elems_.front());
        elems_.pop();
        cv_producer_.notify_one();
        return front;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lk(mu_);
        return elems_.size();
    }

private:
    std::queue<T> elems_;
    std::size_t capacity_;
    mutable std::mutex mu_;
    std::condition_variable cv_producer_;
    std::condition_variable cv_consumer_;
};

struct Script {
    std::size_t producers, consumers, items;
};

Script load_script() {
    Script s{};
    std::string w;
    std::cin >> w >> s.producers >> w >> s.consumers >> w >> s.items;
    return s;
}

int main() {
    Script conf = load_script();
    BlockingQueue<int> q(conf.items + 2);
    std::vector<std::thread> workers;
    std::mutex tally_mu;
    std::size_t tally = 0;

    int next = 0;
    for (std::size_t i = 0; i < conf.producers; ++i) {
        std::size_t share = conf.items / conf.producers + (i < conf.items % conf.producers ? 1 : 0);
        int begin = next;
        next += static_cast<int>(share);
        workers.emplace_back([&, share, begin]() {
            for (std::size_t j = 0; j < share; ++j)
                q.push(begin + static_cast<int>(j));
        });
    }

    for (std::size_t i = 0; i < conf.consumers; ++i) {
        workers.emplace_back([&]() {
            while (true) {
                int n = q.pop();
                if (n == -1) return;
                std::cout << "got " << n << '\n';
                bool done = false;
                {
                    std::lock_guard<std::mutex> g(tally_mu);
                    ++tally;
                    done = (tally == conf.items);
                }
                if (done) {
                    for (std::size_t id = 0; id < conf.consumers; ++id)
                        q.push(-1);
                }
            }
        });
    }

    for (auto& piece : workers) piece.join();
    std::cout << "DONE\n";
    return 0;
}
