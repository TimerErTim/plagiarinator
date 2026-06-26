#include <condition_variable>
#include <atomic>
#include <mutex>
#include <iostream>
#include <string>
#include <queue>
#include <thread>
// quick submit

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
        T x = std::move(q.front());
        q.pop();
        full_cv.notify_one();
        return x;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lk(mu);
        return q.size();
    }
};

static void spawn_producers(BlockingQueue<int>& bq, std::size_t p, std::size_t n, std::vector<std::thread>& out) {
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
    std::size_t p, c, n;
    std::string tag;
    std::cin >> tag >> p >> tag >> c >> tag >> n;

    BlockingQueue<int> bq(n);
    std::vector<std::thread> ts;
    std::atomic<std::size_t> count{0};

    spawn_producers(bq, p, n, ts);

    for (std::size_t i = 0; i < c; ++i) {
        ts.emplace_back([&] {
            while (true) {
                int v = bq.pop();
                if (v < 0) break;
                std::cout << "got " << v << '\n';
                if (count.fetch_add(1) + 1 == n)
                    for (std::size_t j = 0; j < c; ++j) bq.push(-1);
            }
        });
    }

    for (auto& t : ts) t.join();
    std::cout << "DONE\n";
    return 0;
}
