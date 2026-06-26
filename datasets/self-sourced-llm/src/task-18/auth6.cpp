#include <iostream>
#include <memory>
#include <string>

class Parser;

class State {
public:
    virtual ~State() = default;
    virtual std::unique_ptr<State> handle(Parser& ctx, const std::string& line) = 0;
};

class Parser {
public:
    std::unique_ptr<State> state;
    int info_n = 0, warn_n = 0, error_n = 0, bad_n = 0;
};

class HeaderState : public State {
public:
    std::unique_ptr<State> handle(Parser& ctx, const std::string& line) override;
};

class BodyState : public State {
public:
    std::unique_ptr<State> handle(Parser& ctx, const std::string& line) override;
};

class DoneState : public State {
public:
    std::unique_ptr<State> handle(Parser&, const std::string&) override {
        return std::unique_ptr<State>(new DoneState());
    }
};

namespace logparse {

bool level_of(const std::string& line, std::string& out) {
    if (line.size() < 3 || line.front() != '[') return false;
    std::size_t idx = line.find(']');
    if (idx == std::string::npos) return false;
    out = line.substr(1, idx - 1);
    return out == "INFO" || out == "WARN" || out == "ERROR";
}

void bump(Parser& ctx, const std::string& lvl) {
    if (lvl == "INFO") ++ctx.info_n;
    else if (lvl == "WARN") ++ctx.warn_n;
    else ++ctx.error_n;
}

}  // namespace logparse

std::unique_ptr<State> HeaderState::handle(Parser& ctx, const std::string& line) {
    if (line == "END") return std::make_unique<DoneState>();
    std::string lvl;
    if (logparse::level_of(line, lvl)) {
        logparse::bump(ctx, lvl);
        return std::make_unique<BodyState>();
    }
    ++ctx.bad_n;
    return std::make_unique<HeaderState>();
}

std::unique_ptr<State> BodyState::handle(Parser& ctx, const std::string& line) {
    if (line == "END") return std::make_unique<DoneState>();
    std::string lvl;
    if (logparse::level_of(line, lvl)) {
        logparse::bump(ctx, lvl);
        return std::make_unique<BodyState>();
    }
    ++ctx.bad_n;
    return std::make_unique<BodyState>();
}

int main() {
    Parser p;
    p.state = std::make_unique<HeaderState>();
    std::string s;
    while (std::getline(std::cin, s))
        p.state = p.state->handle(p, s);

    std::cout << "INFO: " << p.info_n << '\n';
    std::cout << "WARN: " << p.warn_n << '\n';
    std::cout << "ERROR: " << p.error_n << '\n';
    std::cout << "REJECTED: " << p.bad_n << '\n';
    return 0;
}
