#include <iostream>
#include <memory>
#include <string>

class Parser;

struct State {
    virtual ~State() = default;
    virtual std::unique_ptr<State> handle(Parser& ctx, const std::string& line) = 0;
};

struct HeaderState;
struct BodyState;
struct DoneState;

class Parser {
public:
    std::unique_ptr<State> state;
    int info = 0, warn = 0, error = 0, rejected = 0;

    static bool decode(const std::string& line, std::string& level) {
        auto rb = line.find(']');
        if (line.empty() || line[0] != '[' || rb == std::string::npos) return false;
        level = line.substr(1, rb - 1);
        return level == "INFO" || level == "WARN" || level == "ERROR";
    }

    void mark(const std::string& level) {
        if (level == "INFO") ++info;
        else if (level == "WARN") ++warn;
        else ++error;
    }
};

struct DoneState : State {
    std::unique_ptr<State> handle(Parser&, const std::string&) override {
        return std::unique_ptr<State>(new DoneState());
    }
};

struct BodyState : State {
    std::unique_ptr<State> handle(Parser& ctx, const std::string& line) override {
        if (line == "END") return std::unique_ptr<State>(new DoneState());
        std::string lvl;
        if (Parser::decode(line, lvl)) {
            ctx.mark(lvl);
            return std::unique_ptr<State>(new BodyState());
        }
        ++ctx.rejected;
        return std::unique_ptr<State>(new BodyState());
    }
};

struct HeaderState : State {
    std::unique_ptr<State> handle(Parser& ctx, const std::string& line) override {
        if (line == "END") return std::unique_ptr<State>(new DoneState());
        std::string lvl;
        if (Parser::decode(line, lvl)) {
            ctx.mark(lvl);
            return std::unique_ptr<State>(new BodyState());
        }
        ++ctx.rejected;
        return std::unique_ptr<State>(new HeaderState());
    }
};

int main() {
    Parser ctx;
    ctx.state = std::make_unique<HeaderState>();
    std::string row;
    while (std::getline(std::cin, row))
        ctx.state = ctx.state->handle(ctx, row);

    std::cout << "INFO: " << ctx.info << '\n';
    std::cout << "WARN: " << ctx.warn << '\n';
    std::cout << "ERROR: " << ctx.error << '\n';
    std::cout << "REJECTED: " << ctx.rejected << '\n';
    return 0;
}
