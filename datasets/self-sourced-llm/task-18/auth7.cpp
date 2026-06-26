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
    int info = 0, warn = 0, error = 0, rejected = 0;

    void on_level(const std::string& lv) {
        if (lv == "INFO") ++info;
        else if (lv == "WARN") ++warn;
        else if (lv == "ERROR") ++error;
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
        return std::make_unique<DoneState>();
    }
};

static bool parse_tag(const std::string& ln, std::string& tag) {
    std::size_t r = ln.find(']');
    if (ln.empty() || ln[0] != '[' || r == std::string::npos) return false;
    tag = ln.substr(1, r - 1);
    return tag == "INFO" || tag == "WARN" || tag == "ERROR";
}

static std::unique_ptr<State> route(Parser& ctx, const std::string& line, bool body) {
    if (line == "END") return std::unique_ptr<State>(new DoneState());
    std::string tag;
    if (parse_tag(line, tag)) {
        ctx.on_level(tag);
        return std::unique_ptr<State>(new BodyState());
    }
    ++ctx.rejected;
    if (body) return std::unique_ptr<State>(new BodyState());
    return std::unique_ptr<State>(new HeaderState());
}

std::unique_ptr<State> HeaderState::handle(Parser& ctx, const std::string& line) {
    return route(ctx, line, false);
}

std::unique_ptr<State> BodyState::handle(Parser& ctx, const std::string& line) {
    return route(ctx, line, true);
}

int main() {
    Parser parser;
    parser.state = std::make_unique<HeaderState>();
    std::string line;
    while (std::getline(std::cin, line))
        parser.state = parser.state->handle(parser, line);

    std::cout << "INFO: " << parser.info << '\n';
    std::cout << "WARN: " << parser.warn << '\n';
    std::cout << "ERROR: " << parser.error << '\n';
    std::cout << "REJECTED: " << parser.rejected << '\n';
    return 0;
}
