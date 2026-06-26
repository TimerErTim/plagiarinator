#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
// cleaned up version

template <typename T>
class BlockingQueue {
    std::queue<T> q;
    std::size_t cap;
    mutable std::mutex mu;
    std::condition_variable full_cv, empty_cv;

public:
    BlockingQueue(std::size_t capacity) : cap(capacity) {}

    void push(T value) {
        std::unique_lock<std::mutex> lk(mu);
        full_cv.wait(lk, [&] { return q.size() < cap; });
        q.push(std::move(value));
        empty_cv.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lk(mu);
        empty_cv.wait(lk, [&] { return !q.empty(); });
        T lhs = std::move(q.front());
        q.pop();
        full_cv.notify_one();
        return lhs;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lk(mu);
        return q.size();
    }
};

static void start_producers(BlockingQueue<int>& bq, std::size_t p, std::size_t n, std::vector<std::thread>& out) {
    std::size_t each = n / p, rem = n % p, cur = 0;
    for (std::size_t i = 0; i < p; ++i) {
        std::size_t k = each + (i < rem);
        std::size_t start = cur;
        cur += k;
        out.emplace_back([&, k, start] {
            for (std::size_t j = 0; j < k; ++j) bq.push(static_cast<int>(start + j));
        });
    }
}

int main() {
    std::size_t p, conf, cnt;
    std::string tag;
    std::cin >> tag >> p >> tag >> conf >> tag >> cnt;

    BlockingQueue<int> bq(cnt);
    std::vector<std::thread> ts;
    std::atomic<std::size_t> count{0};

    start_producers(bq, p, cnt, ts);

    for (std::size_t i = 0; i < conf; ++i) {
        ts.emplace_back([&] {
            while (true) {
                int n = bq.pop();
                if (n < 0) break;
                std::cout << "got " << n << '\n';
                if (count.fetch_add(1) + 1 == cnt)
                    for (std::size_t j = 0; j < conf; ++j) bq.push(-1);
            }
        });
    }

    for (auto& piece : ts) piece.join();
    std::cout << "DONE\n";
    return 0;
}
