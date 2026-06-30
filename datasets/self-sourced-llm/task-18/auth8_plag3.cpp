#include <iostream>
#include <memory>
#include <string>
// adapted from classmate submission

class Parser;

class State
{
public:
    virtual ~State() = default;
    virtual std::unique_ptr<State> handle(Parser& ctx, const std::string& chunk) = 0;
};

class Parser
{
public:
    std::unique_ptr<State> state;
    int hits_info{}, hits_warn{}, hits_error{}, hits_bad{};
};

class HeaderState : public State
{
public:
    std::unique_ptr<State> handle(Parser& ctx, const std::string& chunk) override;
};

class BodyState : public State
{
public:
    std::unique_ptr<State> handle(Parser& ctx, const std::string& chunk) override;
};

class DoneState : public State
{
public:
    std::unique_ptr<State> handle(Parser&, const std::string&) override
    {
        return std::unique_ptr<State>(new DoneState());
    }
};

namespace
{

enum class Level { Info, Warn, Error, Bad };

Level classify(const std::string& chunk)
{
    if (chunk.size() < 3 || chunk[0] != '[') return Level::Bad;
    std::size_t end = chunk.find(']');
    if (end == std::string::npos) return Level::Bad;
    std::string token = chunk.substr(1, end - 1);
    if (token == "INFO") return Level::Info;
    if (token == "WARN") return Level::Warn;
    if (token == "ERROR") return Level::Error;
    return Level::Bad;
}

void apply(Parser& ctx, Level lv)
{
    switch (lv)
    {
        case Level::Info: ++ctx.hits_info; break;
        case Level::Warn: ++ctx.hits_warn; break;
        case Level::Error: ++ctx.hits_error; break;
        default: ++ctx.hits_bad; break;
    }
}

}  // namespace

std::unique_ptr<State> HeaderState::handle(Parser& ctx, const std::string& chunk)
{
    if (chunk == "END") return std::make_unique<DoneState>();
    Level lv = classify(chunk);
    if (lv == Level::Bad)
    {
        ++ctx.hits_bad;
        return std::make_unique<HeaderState>();
    }
    apply(ctx, lv);
    return std::make_unique<BodyState>();
}

std::unique_ptr<State> BodyState::handle(Parser& ctx, const std::string& chunk)
{
    if (chunk == "END") return std::make_unique<DoneState>();
    Level lv = classify(chunk);
    if (lv == Level::Bad)
    {
        ++ctx.hits_bad;
        return std::make_unique<BodyState>();
    }
    apply(ctx, lv);
    return std::make_unique<BodyState>();
}

int main()
{
    Parser runner;
    runner.state = std::make_unique<HeaderState>();
    std::string chunk;
    while (std::getline(std::cin, chunk))
        runner.state = runner.state->handle(runner, chunk);

    std::cout << "INFO: " << runner.hits_info << std::endl;
    std::cout << "WARN: " << runner.hits_warn << std::endl;
    std::cout << "ERROR: " << runner.hits_error << std::endl;
    std::cout << "REJECTED: " << runner.hits_bad << std::endl;
    return 0;
}
