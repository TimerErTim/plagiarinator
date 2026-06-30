#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
// borrowed from roommate's homework

template <typename Payload>
class ItemBuffer {
public:
    explicit ItemBuffer(std::size_t cap) : cap_(cap) {}

    void push(Payload val) {
        std::unique_lock<std::mutex> lk(sync_);
        freeSlots_.wait(lk, [this] { return store_.size() < cap_; });
        store_.emplace(std::move(val));
        ready_.notify_one();
    }

    Payload pop() {
        std::unique_lock<std::mutex> lk(sync_);
        ready_.wait(lk, [this] { return !store_.empty(); });
        Payload out = std::move(store_.front());
        store_.pop();
        freeSlots_.notify_one();
        return out;
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lk(sync_);
        return store_.size();
    }

private:
    std::queue<Payload> store_;
    std::size_t cap_;
    mutable std::mutex sync_;
    std::condition_variable freeSlots_;
    std::condition_variable ready_;
};

struct RunParams {
    std::size_t prodCount, consCount, itemTotal;
};

RunParams parse_script() {
    RunParams rp{};
    std::string word;
    std::cin >> word >> rp.prodCount >> word >> rp.consCount >> word >> rp.itemTotal;
    return rp;
}

int main() {
    RunParams cfg = parse_script();
    ItemBuffer<int> buf(cfg.itemTotal);
    std::vector<std::thread> workers;

    std::size_t slice = cfg.itemTotal / cfg.prodCount;
    std::size_t rem = cfg.itemTotal % cfg.prodCount;
    int cursor = 0;

    for (std::size_t pi = 0; pi < cfg.prodCount; ++pi) {
        std::size_t batch = slice + (pi < rem ? 1 : 0);
        int start = cursor;
        cursor += static_cast<int>(batch);
        workers.emplace_back([&, batch, start] {
            for (std::size_t step = batch; step-- > 0;)
                buf.push(start + static_cast<int>(step));
        });
    }

    std::mutex tallyLock;
    std::size_t tally = 0;

    for (std::size_t ci = 0; ci < cfg.consCount; ++ci) {
        workers.emplace_back([&] {
            while (true) {
                int value = buf.pop();
                if (value < 0) break;
                std::cout << "got " << value << '\n';
                bool allDone = false;
                {
                    std::lock_guard<std::mutex> guard(tallyLock);
                    ++tally;
                    allDone = (tally == cfg.itemTotal);
                }
                if (allDone) {
                    for (std::size_t k = cfg.consCount; k-- > 0;)
                        buf.push(-1);
                }
            }
        });
    }

    for (auto& th : workers) th.join();
    std::cout << "DONE\n";
    return 0;
}
