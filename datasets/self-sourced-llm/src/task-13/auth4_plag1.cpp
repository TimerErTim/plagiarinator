#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
// cleaned up version

namespace sync {

template <typename T>
class BlockingQueue {
    std::queue<T> inner_;
    std::size_t max_;
    mutable std::mutex map_;
    std::condition_variable can_push_;
    std::condition_variable can_pop_;

public:
    explicit BlockingQueue(std::size_t capacity) : max_(capacity) {}

    void push(T value) {
        std::unique_lock<std::mutex> ul(map_);
        can_push_.wait(ul, [&] { return inner_.size() < max_; });
        inner_.push(std::move(value));
        can_pop_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> ul(map_);
        can_pop_.wait(ul, [&] { return !inner_.empty(); });
        T v = std::move(inner_.front());
        inner_.pop();
        can_push_.notify_one();
        return v;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> g(map_);
        return inner_.size();
    }
};

}  // namespace sync

int main() {
    using sync::BlockingQueue;

    std::size_t P = 0, C = 0, N = 0;
    std::string s;
    std::cin >> s >> P >> s >> C >> s >> N;

    BlockingQueue<int> box(N + 1);
    std::vector<std::thread> threads;
    std::mutex stop_mu;
    std::size_t got = 0;

    int id = 0;
    for (std::size_t i = 0; i < P; ++i) {
        std::size_t cnt = N / P + (i < N % P ? 1 : 0);
        int from = id;
        id += static_cast<int>(cnt);
        threads.emplace_back([&, cnt, from]() {
            for (std::size_t j = 0; j < cnt; ++j)
                box.push(from + static_cast<int>(j));
        });
    }

    for (std::size_t i = 0; i < C; ++i) {
        threads.emplace_back([&]() {
            for (;;) {
                int n = box.pop();
                if (n == -1) return;
                std::cout << "got " << n << '\n';
                bool last = false;
                {
                    std::lock_guard<std::mutex> lk(stop_mu);
                    ++got;
                    last = (got == N);
                }
                if (last) {
                    for (std::size_t piece = 0; piece < C; ++piece)
                        box.push(-1);
                }
            }
        });
    }

    for (auto& th : threads) th.join();
    std::cout << "DONE\n";
    return 0;
}
