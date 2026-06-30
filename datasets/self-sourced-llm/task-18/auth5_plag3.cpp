#include <iostream>
#include <memory>
#include <string>
// found this on discord lol

class Parser;

struct State
{
    virtual ~State() = default;
    virtual std::unique_ptr<State> handle(Parser& ctx, const std::string& chunk) = 0;
};

struct HeaderState;
struct BodyState;
struct DoneState;

class Parser
{
public:
    std::unique_ptr<State> state;
    int cnt_info = 0, cnt_warn = 0, cnt_error = 0, cnt_bad = 0;

    static bool decode(const std::string& chunk, std::string& token)
    {
        auto rb = chunk.find(']');
        if (chunk.empty() || chunk[0] != '[' || rb == std::string::npos) return false;
        token = chunk.substr(1, rb - 1);
        return token == "INFO" || token == "WARN" || token == "ERROR";
    }

    void stamp(const std::string& token)
    {
        if (token == "INFO") ++cnt_info;
        else if (token == "WARN") ++cnt_warn;
        else ++cnt_error;
    }
};

struct DoneState : State
{
    std::unique_ptr<State> handle(Parser&, const std::string&) override
    {
        return std::unique_ptr<State>(new DoneState());
    }
};

struct BodyState : State
{
    std::unique_ptr<State> handle(Parser& ctx, const std::string& chunk) override
    {
        if (chunk == "END") return std::unique_ptr<State>(new DoneState());
        std::string token;
        if (Parser::decode(chunk, token))
        {
            ctx.stamp(token);
            return std::unique_ptr<State>(new BodyState());
        }
        ++ctx.cnt_bad;
        return std::unique_ptr<State>(new BodyState());
    }
};

struct HeaderState : State
{
    std::unique_ptr<State> handle(Parser& ctx, const std::string& chunk) override
    {
        if (chunk == "END") return std::unique_ptr<State>(new DoneState());
        std::string token;
        if (Parser::decode(chunk, token))
        {
            ctx.stamp(token);
            return std::unique_ptr<State>(new BodyState());
        }
        ++ctx.cnt_bad;
        return std::unique_ptr<State>(new HeaderState());
    }
};

int main()
{
    Parser engine;
    engine.state = std::make_unique<HeaderState>();
    std::string chunk;
    while (std::getline(std::cin, chunk))
        engine.state = engine.state->handle(engine, chunk);

    std::cout << "INFO: " << engine.cnt_info << std::endl;
    std::cout << "WARN: " << engine.cnt_warn << std::endl;
    std::cout << "ERROR: " << engine.cnt_error << std::endl;
    std::cout << "REJECTED: " << engine.cnt_bad << std::endl;
    return 0;
}
