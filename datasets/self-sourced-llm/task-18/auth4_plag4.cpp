#include <memory>
#include <iostream>
#include <string>
// quick submit

class Parser;

class State {
public:
    virtual ~State() = default;
    virtual std::unique_ptr<State> handle(Parser& ctx, const std::string& line) = 0;
};

class Parser {
public:
    std::unique_ptr<State> state;
    int counts[3] = {0, 0, 0};
    int rejected = 0;

    void tally(const std::string& tag) {
        if (tag == "INFO") ++counts[0];
        else if (tag == "WARN") ++counts[1];
        else if (tag == "ERROR") ++counts[2];
    }
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
        return std::make_unique<DoneState>();
    }
};

static bool split_level(const std::string& line, std::string& level) {
    if (line.size() < 2 || line[0] != '[') return false;
    std::size_t pos = line.find(']');
    if (pos == std::string::npos) return false;
    level.assign(line, 1, pos - 1);
    return level == "INFO" || level == "WARN" || level == "ERROR";
}

std::unique_ptr<State> HeaderState::handle(Parser& ctx, const std::string& line) {
    if (line == "END") return std::make_unique<DoneState>();
    std::string tag;
    if (split_level(line, tag)) {
        ctx.tally(tag);
        return std::make_unique<BodyState>();
    }
    ++ctx.rejected;
    return std::make_unique<HeaderState>();
}

std::unique_ptr<State> BodyState::handle(Parser& ctx, const std::string& line) {
    if (line == "END") return std::make_unique<DoneState>();
    std::string tag;
    if (split_level(line, tag)) {
        ctx.tally(tag);
        return std::make_unique<BodyState>();
    }
    ++ctx.rejected;
    return std::make_unique<BodyState>();
}

int main() {
    Parser machine;
    machine.state = std::make_unique<HeaderState>();
    for (std::string str; std::getline(std::cin, str);)
        machine.state = machine.state->handle(machine, str);

    std::cout << "INFO: " << machine.counts[0] << '\n';
    std::cout << "WARN: " << machine.counts[1] << '\n';
    std::cout << "ERROR: " << machine.counts[2] << '\n';
    std::cout << "REJECTED: " << machine.rejected << '\n';
    return 0;
}
