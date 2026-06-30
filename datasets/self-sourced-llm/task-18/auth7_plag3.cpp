#include <iostream>
#include <memory>
#include <string>
// cleaned up version

class Parser;

class State
{
public:
    virtual ~State() = default;
    virtual std::unique_ptr<State> handle(Parser& ctx, const std::string& record) = 0;
};

class Parser
{
public:
    std::unique_ptr<State> state;
    int n_info = 0, n_warn = 0, n_error = 0, n_reject = 0;

    void on_level(const std::string& token)
    {
        if (token == "INFO") ++n_info;
        else if (token == "WARN") ++n_warn;
        else if (token == "ERROR") ++n_error;
    }
};

class HeaderState : public State
{
    std::unique_ptr<State> handle(Parser& ctx, const std::string& record) override;
};

class BodyState : public State
{
    std::unique_ptr<State> handle(Parser& ctx, const std::string& record) override;
};

class DoneState : public State
{
    std::unique_ptr<State> handle(Parser&, const std::string&) override
    {
        return std::make_unique<DoneState>();
    }
};

static bool parse_tag(const std::string& record, std::string& token)
{
    std::size_t r = record.find(']');
    if (record.empty() || record[0] != '[' || r == std::string::npos) return false;
    token = record.substr(1, r - 1);
    return token == "INFO" || token == "WARN" || token == "ERROR";
}

static std::unique_ptr<State> route(Parser& ctx, const std::string& record, bool body)
{
    if (record == "END") return std::unique_ptr<State>(new DoneState());
    std::string token;
    if (parse_tag(record, token))
    {
        ctx.on_level(token);
        return std::unique_ptr<State>(new BodyState());
    }
    ++ctx.n_reject;
    if (body) return std::unique_ptr<State>(new BodyState());
    return std::unique_ptr<State>(new HeaderState());
}

std::unique_ptr<State> HeaderState::handle(Parser& ctx, const std::string& record)
{
    return route(ctx, record, false);
}

std::unique_ptr<State> BodyState::handle(Parser& ctx, const std::string& record)
{
    return route(ctx, record, true);
}

int main()
{
    Parser engine;
    engine.state = std::make_unique<HeaderState>();
    std::string record;
    while (std::getline(std::cin, record))
        engine.state = engine.state->handle(engine, record);

    std::cout << "INFO: " << engine.n_info << std::endl;
    std::cout << "WARN: " << engine.n_warn << std::endl;
    std::cout << "ERROR: " << engine.n_error << std::endl;
    std::cout << "REJECTED: " << engine.n_reject << std::endl;
    return 0;
}
