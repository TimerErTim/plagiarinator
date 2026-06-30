#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
#include <vector>
// borrowed from roommate's homework

template <typename Unit>
class LineQueue {
    const std::size_t limit_;
    std::queue<Unit> pipe_;
    mutable std::mutex gate_;
    std::condition_variable prodSig_;
    std::condition_variable consSig_;

public:
    explicit LineQueue(std::size_t limit) : limit_(limit) {}

    void push(Unit val) {
        std::unique_lock<std::mutex> hold(gate_);
        prodSig_.wait(hold, [this] { return pipe_.size() < limit_; });
        pipe_.emplace(std::move(val));
        consSig_.notify_one();
    }

    Unit pop() {
        std::unique_lock<std::mutex> hold(gate_);
        consSig_.wait(hold, [this] { return !pipe_.empty(); });
        Unit picked = std::move(pipe_.front());
        pipe_.pop();
        prodSig_.notify_one();
        return picked;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> hold(gate_);
        return pipe_.size();
    }
};

int main() {
    std::size_t prodCnt = 0, consCnt = 0, itemCnt = 0;
    std::string line;
    std::getline(std::cin, line);
    {
        std::istringstream a(line);
        std::string t;
        a >> t >> prodCnt;
    }
    std::getline(std::cin, line);
    {
        std::istringstream b(line);
        std::string t;
        b >> t >> consCnt;
    }
    std::getline(std::cin, line);
    {
        std::istringstream c(line);
        std::string t;
        c >> t >> itemCnt;
    }

    LineQueue<int> lane(itemCnt);
    std::vector<std::thread> team;
    std::size_t recv = 0;
    std::mutex recvMu;

    int idBase = 0;
    for (std::size_t p = 0; p < prodCnt; ++p) {
        std::size_t share = itemCnt / prodCnt + (p < itemCnt % prodCnt ? 1 : 0);
        int base = idBase;
        idBase += static_cast<int>(share);
        team.emplace_back([&, share, base] {
            for (std::size_t step = share; step-- > 0;)
                lane.push(base + static_cast<int>(step));
        });
    }

    for (std::size_t c = 0; c < consCnt; ++c) {
        team.emplace_back([&] {
            while (true) {
                int v = lane.pop();
                if (v < 0) break;
                std::cout << "got " << v << '\n';
                bool all = false;
                {
                    std::lock_guard<std::mutex> g(recvMu);
                    ++recv;
                    all = (recv == itemCnt);
                }
                if (all) {
                    for (std::size_t k = consCnt; k-- > 0;)
                        lane.push(-1);
                }
            }
        });
    }

    for (auto& th : team) th.join();
    std::cout << "DONE\n";
    return 0;
}
