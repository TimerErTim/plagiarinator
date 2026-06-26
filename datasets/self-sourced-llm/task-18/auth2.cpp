#include <iostream>
#include <memory>
#include <string>

class Parser;

struct State {
    virtual ~State() = default;
    virtual std::unique_ptr<State> handle(Parser& ctx, const std::string& line) = 0;
};

class Parser {
public:
    std::unique_ptr<State> state;
    int info_count = 0;
    int warn_count = 0;
    int error_count = 0;
    int rejected = 0;
};

struct HeaderState : State {
    std::unique_ptr<State> handle(Parser& ctx, const std::string& line) override;
};

struct BodyState : State {
    std::unique_ptr<State> handle(Parser& ctx, const std::string& line) override;
};

struct DoneState : State {
    std::unique_ptr<State> handle(Parser&, const std::string&) override {
        return std::make_unique<DoneState>();
    }
};

static bool extract_level(const std::string& line, std::string& out) {
    if (line.empty() || line.front() != '[') return false;
    std::size_t close = line.find(']');
    if (close == std::string::npos || close <= 1) return false;
    out = line.substr(1, close - 1);
    return out == "INFO" || out == "WARN" || out == "ERROR";
}

static void record(Parser& ctx, const std::string& lvl) {
    if (lvl == "INFO") ++ctx.info_count;
    else if (lvl == "WARN") ++ctx.warn_count;
    else ++ctx.error_count;
}

std::unique_ptr<State> HeaderState::handle(Parser& ctx, const std::string& line) {
    if (line == "END") return std::make_unique<DoneState>();
    std::string lvl;
    if (extract_level(line, lvl)) {
        record(ctx, lvl);
        return std::make_unique<BodyState>();
    }
    ++ctx.rejected;
    return std::make_unique<HeaderState>();
}

std::unique_ptr<State> BodyState::handle(Parser& ctx, const std::string& line) {
    if (line == "END") return std::make_unique<DoneState>();
    std::string lvl;
    if (extract_level(line, lvl)) {
        record(ctx, lvl);
        return std::make_unique<BodyState>();
    }
    ++ctx.rejected;
    return std::make_unique<BodyState>();
}

int main() {
    Parser p;
    p.state = std::make_unique<HeaderState>();
    std::string line;
    while (std::getline(std::cin, line))
        p.state = p.state->handle(p, line);

    std::cout << "INFO: " << p.info_count << '\n';
    std::cout << "WARN: " << p.warn_count << '\n';
    std::cout << "ERROR: " << p.error_count << '\n';
    std::cout << "REJECTED: " << p.rejected << '\n';
    return 0;
}
