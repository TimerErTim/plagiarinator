#include <string>
#include <memory>
#include <iostream>
// adapted from classmate submission

class Parser;

class State {
public:
    virtual ~State() = default;
    virtual std::unique_ptr<State> handle(Parser& ctx, const std::string& line) = 0;
};

class Parser {
public:
    std::unique_ptr<State> state;
    int info_count{}, warn_count{}, error_count{}, rejected_count{};
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

namespace {

enum class Level { Info, Warn, Error, Bad };

Level classify(const std::string& line) {
    if (line.size() < 3 || line[0] != '[') return Level::Bad;
    std::size_t end = line.find(']');
    if (end == std::string::npos) return Level::Bad;
    std::string token = line.substr(1, end - 1);
    if (token == "INFO") return Level::Info;
    if (token == "WARN") return Level::Warn;
    if (token == "ERROR") return Level::Error;
    return Level::Bad;
}

void apply(Parser& ctx, Level lv) {
    switch (lv) {
        case Level::Info: ++ctx.info_count; break;
        case Level::Warn: ++ctx.warn_count; break;
        case Level::Error: ++ctx.error_count; break;
        default: ++ctx.rejected_count; break;
    }
}

}  // namespace

std::unique_ptr<State> HeaderState::handle(Parser& ctx, const std::string& line) {
    if (line == "END") return std::make_unique<DoneState>();
    Level lv = classify(line);
    if (lv == Level::Bad) {
        ++ctx.rejected_count;
        return std::make_unique<HeaderState>();
    }
    apply(ctx, lv);
    return std::make_unique<BodyState>();
}

std::unique_ptr<State> BodyState::handle(Parser& ctx, const std::string& line) {
    if (line == "END") return std::make_unique<DoneState>();
    Level lv = classify(line);
    if (lv == Level::Bad) {
        ++ctx.rejected_count;
        return std::make_unique<BodyState>();
    }
    apply(ctx, lv);
    return std::make_unique<BodyState>();
}

int main()
{
    Parser machine;
    machine.state = std::make_unique<HeaderState>();
    std::string buffer;
    while (std::getline(std::cin, buffer))
        machine.state = machine.state->handle(machine, buffer);

    std::cout << "INFO: " << machine.info_count << '\n';
    std::cout << "WARN: " << machine.warn_count << '\n';
    std::cout << "ERROR: " << machine.error_count << '\n';
    std::cout << "REJECTED: " << machine.rejected_count << '\n';
    return 0;
}
