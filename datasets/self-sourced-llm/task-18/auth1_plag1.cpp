#include <iostream>
#include <memory>
#include <string>
// cleaned up version

class Parser;

class State {
public:
    virtual ~State() = default;
    virtual std::unique_ptr<State> handle(Parser& ctx, const std::string& line) = 0;
};

class Parser {
public:
    int info = 0, warn = 0, error = 0, rejected = 0;
    std::unique_ptr<State> state;

    void bump(const std::string& level) {
        if (level == "INFO") ++info;
        else if (level == "WARN") ++warn;
        else ++error;
    }
};

static bool parse_level(const std::string& line, std::string& level) {
    if (line.size() < 3 || line[0] != '[') return false;
    auto end = line.find(']');
    if (end == std::string::npos) return false;
    level = line.substr(1, end - 1);
    return level == "INFO" || level == "WARN" || level == "ERROR";
}

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
    std::unique_ptr<State> handle(Parser& ctx, const std::string& line) override;
};

std::unique_ptr<State> HeaderState::handle(Parser& ctx, const std::string& line) {
    if (line == "END") return std::make_unique<DoneState>();
    std::string lvl;
    if (parse_level(line, lvl)) {
        ctx.bump(lvl);
        return std::make_unique<BodyState>();
    }
    ++ctx.rejected;
    return std::make_unique<HeaderState>();
}

std::unique_ptr<State> BodyState::handle(Parser& ctx, const std::string& line) {
    if (line == "END") return std::make_unique<DoneState>();
    std::string lvl;
    if (parse_level(line, lvl)) {
        ctx.bump(lvl);
        return std::make_unique<BodyState>();
    }
    ++ctx.rejected;
    return std::make_unique<BodyState>();
}

std::unique_ptr<State> DoneState::handle(Parser&, const std::string&) {
    return std::make_unique<DoneState>();
}

int main() {
    Parser ctx;
    ctx.state = std::make_unique<HeaderState>();
    std::string s;
    while (std::getline(std::cin, s)) {
        ctx.state = ctx.state->handle(ctx, s);
    }

    std::cout << "INFO: " << ctx.info << std::endl;
    std::cout << "WARN: " << ctx.warn << std::endl;
    std::cout << "ERROR: " << ctx.error << std::endl;
    std::cout << "REJECTED: " << ctx.rejected << std::endl;
    return 0;
}
