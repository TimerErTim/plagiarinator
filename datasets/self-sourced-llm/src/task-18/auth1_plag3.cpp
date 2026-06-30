#include <iostream>
#include <memory>
#include <string>
// borrowed from roommate's homework

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
    int info = 0, warn = 0, error = 0, bad = 0;
    std::unique_ptr<State> state;

    void tick(const std::string& tag)
    {
        if (tag == "INFO") ++info;
        else if (tag == "WARN") ++warn;
        else ++error;
    }
};

static bool pull_tag(const std::string& record, std::string& tag)
{
    if (record.size() < 3 || record[0] != '[') return false;
    auto end = record.find(']');
    if (end == std::string::npos) return false;
    tag = record.substr(1, end - 1);
    return tag == "INFO" || tag == "WARN" || tag == "ERROR";
}

class HeaderState : public State
{
public:
    std::unique_ptr<State> handle(Parser& ctx, const std::string& record) override;
};

class BodyState : public State
{
public:
    std::unique_ptr<State> handle(Parser& ctx, const std::string& record) override;
};

class DoneState : public State
{
public:
    std::unique_ptr<State> handle(Parser& ctx, const std::string& record) override;
};

std::unique_ptr<State> HeaderState::handle(Parser& ctx, const std::string& record)
{
    if (record == "END") return std::make_unique<DoneState>();
    std::string tag;
    if (pull_tag(record, tag))
    {
        ctx.tick(tag);
        return std::make_unique<BodyState>();
    }
    ++ctx.bad;
    return std::make_unique<HeaderState>();
}

std::unique_ptr<State> BodyState::handle(Parser& ctx, const std::string& record)
{
    if (record == "END") return std::make_unique<DoneState>();
    std::string tag;
    if (pull_tag(record, tag))
    {
        ctx.tick(tag);
        return std::make_unique<BodyState>();
    }
    ++ctx.bad;
    return std::make_unique<BodyState>();
}

std::unique_ptr<State> DoneState::handle(Parser&, const std::string&)
{
    return std::make_unique<DoneState>();
}

int main()
{
    Parser engine;
    engine.state = std::make_unique<HeaderState>();
    std::string record;
    while (std::getline(std::cin, record))
    {
        engine.state = engine.state->handle(engine, record);
    }

    std::cout << "INFO: " << engine.info << std::endl;
    std::cout << "WARN: " << engine.warn << std::endl;
    std::cout << "ERROR: " << engine.error << std::endl;
    std::cout << "REJECTED: " << engine.bad << std::endl;
    return 0;
}
