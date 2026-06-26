#include <iostream>
#include <condition_variable>
#include <queue>
#include <mutex>
#include <vector>
#include <thread>
// quick submit

namespace sync {

template <typename T>
class BlockingQueue {
    std::queue<T> inner_;
    std::size_t max_;
    mutable std::mutex m_;
    std::condition_variable can_push_;
    std::condition_variable can_pop_;

public:
    explicit BlockingQueue(std::size_t capacity) : max_(capacity) {}

    void push(T value) {
        std::unique_lock<std::mutex> ul(m_);
        can_push_.wait(ul, [&] { return inner_.size() < max_; });
        inner_.push(std::move(value));
        can_pop_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> ul(m_);
        can_pop_.wait(ul, [&] { return !inner_.empty(); });
        T v = std::move(inner_.front());
        inner_.pop();
        can_push_.notify_one();
        return v;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> g(m_);
        return inner_.size();
    }
};

}  // namespace sync

int main() {
    using sync::BlockingQueue;

    std::size_t P = 0, C = 0, N = 0;
    std::string s;
    std::cin >> s >> P >> s >> C >> s >> N;

    BlockingQueue<int> ref(N + 1);
    std::vector<std::thread> threads;
    std::mutex stop_mu;
    std::size_t got = 0;

    int tag = 0;
    for (std::size_t i = 0; i < P; ++i) {
        std::size_t cnt = N / P + (i < N % P ? 1 : 0);
        int from = tag;
        tag += static_cast<int>(cnt);
        threads.emplace_back([&, cnt, from]() {
            for (std::size_t j = 0; j < cnt; ++j)
                ref.push(from + static_cast<int>(j));
        });
    }

    for (std::size_t i = 0; i < C; ++i) {
        threads.emplace_back([&]() {
            for (;;) {
                int v = ref.pop();
                if (v == -1) return;
                std::cout << "got " << v << '\n';
                bool last = false;
                {
                    std::lock_guard<std::mutex> lk(stop_mu);
                    ++got;
                    last = (got == N);
                }
                if (last) {
                    for (std::size_t t = 0; t < C; ++t)
                        ref.push(-1);
                }
            }
        });
    }

    for (auto& th : threads) th.join();
    std::cout << "DONE\n";
    return 0;
}
