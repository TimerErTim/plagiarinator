#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>
// borrowed from roommate's homework

template <typename ItemTy>
class WorkQueue {
    std::queue<ItemTy> backlog_;
    std::size_t maxDepth_;
    mutable std::mutex gate_;
    std::condition_variable hasSpace_, hasItem_;

public:
    WorkQueue(std::size_t maxDepth) : maxDepth_(maxDepth) {}

    void push(ItemTy val) {
        std::unique_lock<std::mutex> lk(gate_);
        hasSpace_.wait(lk, [&] { return backlog_.size() < maxDepth_; });
        backlog_.push(std::move(val));
        hasItem_.notify_one();
    }

    ItemTy pop() {
        std::unique_lock<std::mutex> lk(gate_);
        hasItem_.wait(lk, [&] { return !backlog_.empty(); });
        ItemTy x = std::move(backlog_.front());
        backlog_.pop();
        hasSpace_.notify_one();
        return x;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lk(gate_);
        return backlog_.size();
    }
};

static void boot_producers(WorkQueue<int>& wq, std::size_t prodCnt, std::size_t itemCnt,
                           std::vector<std::thread>& pool) {
    std::size_t share = itemCnt / prodCnt, extra = itemCnt % prodCnt, pos = 0;
    for (std::size_t idx = 0; idx < prodCnt; ++idx) {
        std::size_t batch = share + (idx < extra);
        std::size_t origin = pos;
        pos += batch;
        pool.emplace_back([&, batch, origin] {
            for (std::size_t step = batch; step-- > 0;)
                wq.push(static_cast<int>(origin + step));
        });
    }
}

int main() {
    std::size_t prodCnt, consCnt, itemCnt;
    std::string lbl;
    std::cin >> lbl >> prodCnt >> lbl >> consCnt >> lbl >> itemCnt;

    WorkQueue<int> wq(itemCnt);
    std::vector<std::thread> pool;
    std::atomic<std::size_t> done{0};

    boot_producers(wq, prodCnt, itemCnt, pool);

    for (std::size_t i = 0; i < consCnt; ++i) {
        pool.emplace_back([&] {
            while (true) {
                int v = wq.pop();
                if (v < 0) break;
                std::cout << "got " << v << '\n';
                if (done.fetch_add(1) + 1 == itemCnt)
                    for (std::size_t j = consCnt; j-- > 0;)
                        wq.push(-1);
            }
        });
    }

    for (auto& t : pool) t.join();
    std::cout << "DONE\n";
    return 0;
}
