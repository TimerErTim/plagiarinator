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
    int info = 0, warn = 0, err = 0, rejected = 0;

    void accept(const std::string& level) {
        if (level == "INFO") ++info;
        else if (level == "WARN") ++warn;
        else if (level == "ERROR") ++err;
    }
};

class HeaderState : public State {
    std::unique_ptr<State> handle(Parser& ctx, const std::string& line) override;
};

class BodyState : public State {
    std::unique_ptr<State> handle(Parser& ctx, const std::string& line) override;
};

class DoneState : public State {
    std::unique_ptr<State> handle(Parser&, const std::string&) override {
        return std::unique_ptr<State>(new DoneState());
    }
};

namespace {

bool valid_log(const std::string& line, std::string& level) {
    std::size_t rb = line.find(']');
    if (line.size() < 4 || line[0] != '[' || rb == std::string::npos) return false;
    level = line.substr(1, rb - 1);
    return level == "INFO" || level == "WARN" || level == "ERROR";
}

std::unique_ptr<State> process_body(Parser& ctx, const std::string& line) {
    if (line == "END") return std::make_unique<DoneState>();
    std::string lvl;
    if (valid_log(line, lvl)) {
        ctx.accept(lvl);
        return std::make_unique<BodyState>();
    }
    ++ctx.rejected;
    return std::make_unique<BodyState>();
}

}  // namespace

std::unique_ptr<State> HeaderState::handle(Parser& ctx, const std::string& line) {
    if (line == "END") return std::make_unique<DoneState>();
    std::string lvl;
    if (valid_log(line, lvl)) {
        ctx.accept(lvl);
        return std::make_unique<BodyState>();
    }
    ++ctx.rejected;
    return std::make_unique<HeaderState>();
}

std::unique_ptr<State> BodyState::handle(Parser& ctx, const std::string& line) {
    return process_body(ctx, line);
}

int main() {
    Parser parser;
    parser.state.reset(new HeaderState());
    std::string input;
    while (std::getline(std::cin, input))
        parser.state = parser.state->handle(parser, input);

    std::cout << "INFO: " << parser.info << '\n';
    std::cout << "WARN: " << parser.warn << '\n';
    std::cout << "ERROR: " << parser.err << '\n';
    std::cout << "REJECTED: " << parser.rejected << '\n';
    return 0;
}
