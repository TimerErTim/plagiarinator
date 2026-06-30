#include <iostream>
#include <memory>
#include <string>
// tweaked from lab partner code

class Parser;

struct State
{
    virtual ~State() = default;
    virtual std::unique_ptr<State> handle(Parser& ctx, const std::string& chunk) = 0;
};

class Parser
{
public:
    std::unique_ptr<State> state;
    int n_info = 0;
    int n_warn = 0;
    int n_error = 0;
    int n_bad = 0;
};

struct HeaderState : State
{
    std::unique_ptr<State> handle(Parser& ctx, const std::string& chunk) override;
};

struct BodyState : State
{
    std::unique_ptr<State> handle(Parser& ctx, const std::string& chunk) override;
};

struct DoneState : State
{
    std::unique_ptr<State> handle(Parser&, const std::string&) override
    {
        return std::make_unique<DoneState>();
    }
};

static bool grab_token(const std::string& chunk, std::string& token)
{
    if (chunk.empty() || chunk.front() != '[') return false;
    std::size_t close = chunk.find(']');
    if (close == std::string::npos || close <= 1) return false;
    token = chunk.substr(1, close - 1);
    return token == "INFO" || token == "WARN" || token == "ERROR";
}

static void tally(Parser& ctx, const std::string& token)
{
    if (token == "INFO") ++ctx.n_info;
    else if (token == "WARN") ++ctx.n_warn;
    else ++ctx.n_error;
}

std::unique_ptr<State> HeaderState::handle(Parser& ctx, const std::string& chunk)
{
    if (chunk == "END") return std::make_unique<DoneState>();
    std::string token;
    if (grab_token(chunk, token))
    {
        tally(ctx, token);
        return std::make_unique<BodyState>();
    }
    ++ctx.n_bad;
    return std::make_unique<HeaderState>();
}

std::unique_ptr<State> BodyState::handle(Parser& ctx, const std::string& chunk)
{
    if (chunk == "END") return std::make_unique<DoneState>();
    std::string token;
    if (grab_token(chunk, token))
    {
        tally(ctx, token);
        return std::make_unique<BodyState>();
    }
    ++ctx.n_bad;
    return std::make_unique<BodyState>();
}

int main()
{
    Parser runner;
    runner.state = std::make_unique<HeaderState>();
    std::string chunk;
    while (std::getline(std::cin, chunk))
        runner.state = runner.state->handle(runner, chunk);

    std::cout << "INFO: " << runner.n_info << std::endl;
    std::cout << "WARN: " << runner.n_warn << std::endl;
    std::cout << "ERROR: " << runner.n_error << std::endl;
    std::cout << "REJECTED: " << runner.n_bad << std::endl;
    return 0;
}
