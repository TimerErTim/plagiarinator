#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

template <typename T>
class BlockingQueue {
    std::queue<T> q;
    std::size_t cap;
    mutable std::mutex mu;
    std::condition_variable full_cv, empty_cv;

public:
    BlockingQueue(std::size_t capacity) : cap(capacity) {}

    void push(T value)
    {
        std::unique_lock<std::mutex> lk(mu);
        full_cv.wait(lk, [&] { return q.size() < cap; });
        q.push(std::move(value));
        empty_cv.notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lk(mu);
        empty_cv.wait(lk, [&] { return !q.empty(); });
        T x = std::move(q.front());
        q.pop();
        full_cv.notify_one();
        return x;
    }

    std::size_t size() const
    {
        std::lock_guard<std::mutex> lk(mu);
        return q.size();
    }
};

struct SimInput {
    std::size_t p, c, n;
};

SimInput read_sim()
{
    SimInput in{};
    std::string tag;
    std::size_t p_tmp = 0, c_tmp = 0, n_tmp = 0;
    std::cin >> tag >> p_tmp >> tag >> c_tmp >> tag >> n_tmp;
    in.n = n_tmp;
    in.c = c_tmp;
    in.p = p_tmp;
    return in;
}

void add_producers(BlockingQueue<int>& bq, std::size_t p, std::size_t n, std::vector<std::thread>& out)
{
    std::size_t each = n / p, rem = n % p, cur = 0;
    std::size_t i = 0;
    while (i < p)
    {
        std::size_t k = each + (i < rem);
        std::size_t start = cur;
        cur += k;
        out.emplace_back([&, k, start] {
            std::size_t j = 0;
            while (j < k)
            {
                bq.push(static_cast<int>(start + j));
                ++j;
            }
        });
        ++i;
    }
}

int main()
{
    SimInput cfg = read_sim();
    BlockingQueue<int> bq(cfg.n);
    std::vector<std::thread> ts;
    std::atomic<std::size_t> count{0};

    add_producers(bq, cfg.p, cfg.n, ts);

    for (std::size_t i = 0; i < cfg.c; ++i)
    {
        ts.emplace_back([&] {
            while (true)
            {
                int v = bq.pop();
                if (v < 0) break;
                std::cout << "got " << v << '\n';
                if (count.fetch_add(1) + 1 == cfg.n)
                    for (std::size_t j = 0; j < cfg.c; ++j)
                        bq.push(-1);
            }
        });
    }

    for (auto& t : ts) t.join();
    std::cout << "DONE\n";
    return 0;
}
