#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
#include <vector>
// borrowed from roommate's homework

template <typename Payload>
class ThreadPipe {
public:
    explicit ThreadPipe(std::size_t maxSlots) : maxSlots_(maxSlots) {}

    void push(Payload val) {
        std::unique_lock<std::mutex> guard(syncMu_);
        prodCv_.wait(guard, [this] { return slots_.size() < maxSlots_; });
        slots_.push(std::move(val));
        consCv_.notify_one();
    }

    Payload pop() {
        std::unique_lock<std::mutex> guard(syncMu_);
        consCv_.wait(guard, [this] { return !slots_.empty(); });
        Payload head = std::move(slots_.front());
        slots_.pop();
        prodCv_.notify_one();
        return head;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> guard(syncMu_);
        return slots_.size();
    }

private:
    std::queue<Payload> slots_;
    std::size_t maxSlots_;
    mutable std::mutex syncMu_;
    std::condition_variable prodCv_;
    std::condition_variable consCv_;
};

struct SimParams {
    std::size_t prodCnt, consCnt, itemCnt;
};

SimParams parse_input() {
    SimParams p{};
    std::string skip;
    std::cin >> skip >> p.prodCnt >> skip >> p.consCnt >> skip >> p.itemCnt;
    return p;
}

int main() {
    SimParams cfg = parse_input();
    ThreadPipe<int> pipe(cfg.itemCnt + 2);
    std::vector<std::thread> threadList;
    std::mutex countMu;
    std::size_t recvCount = 0;

    int seq = 0;
    for (std::size_t prod = 0; prod < cfg.prodCnt; ++prod) {
        std::size_t portion = cfg.itemCnt / cfg.prodCnt + (prod < cfg.itemCnt % cfg.prodCnt ? 1 : 0);
        int baseId = seq;
        seq += static_cast<int>(portion);
        threadList.emplace_back([&, portion, baseId]() {
            for (std::size_t idx = portion; idx-- > 0;)
                pipe.push(baseId + static_cast<int>(idx));
        });
    }

    for (std::size_t cons = 0; cons < cfg.consCnt; ++cons) {
        threadList.emplace_back([&]() {
            while (true) {
                int got = pipe.pop();
                if (got == -1) return;
                std::cout << "got " << got << '\n';
                bool allDone = false;
                {
                    std::lock_guard<std::mutex> g(countMu);
                    ++recvCount;
                    allDone = (recvCount == cfg.itemCnt);
                }
                if (allDone) {
                    for (std::size_t stop = cfg.consCnt; stop-- > 0;)
                        pipe.push(-1);
                }
            }
        });
    }

    for (auto& th : threadList) th.join();
    std::cout << "DONE\n";
    return 0;
}
