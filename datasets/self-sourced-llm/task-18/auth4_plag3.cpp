#include <iostream>
#include <memory>
#include <string>
// adapted from classmate submission

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
    int tally_info = 0, tally_warn = 0, tally_err = 0, tally_bad = 0;

    void bump_tag(const std::string& tag)
    {
        if (tag == "INFO") ++tally_info;
        else if (tag == "WARN") ++tally_warn;
        else if (tag == "ERROR") ++tally_err;
    }
};

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
    std::unique_ptr<State> handle(Parser&, const std::string&) override
    {
        return std::make_unique<DoneState>();
    }
};

static bool peel_tag(const std::string& record, std::string& tag)
{
    if (record.size() < 2 || record[0] != '[') return false;
    std::size_t pos = record.find(']');
    if (pos == std::string::npos) return false;
    tag.assign(record, 1, pos - 1);
    return tag == "INFO" || tag == "WARN" || tag == "ERROR";
}

std::unique_ptr<State> HeaderState::handle(Parser& ctx, const std::string& record)
{
    if (record == "END") return std::make_unique<DoneState>();
    std::string tag;
    if (peel_tag(record, tag))
    {
        ctx.bump_tag(tag);
        return std::make_unique<BodyState>();
    }
    ++ctx.tally_bad;
    return std::make_unique<HeaderState>();
}

std::unique_ptr<State> BodyState::handle(Parser& ctx, const std::string& record)
{
    if (record == "END") return std::make_unique<DoneState>();
    std::string tag;
    if (peel_tag(record, tag))
    {
        ctx.bump_tag(tag);
        return std::make_unique<BodyState>();
    }
    ++ctx.tally_bad;
    return std::make_unique<BodyState>();
}

int main()
{
    Parser runner;
    runner.state = std::make_unique<HeaderState>();
    for (std::string record; std::getline(std::cin, record);)
        runner.state = runner.state->handle(runner, record);

    std::cout << "INFO: " << runner.tally_info << std::endl;
    std::cout << "WARN: " << runner.tally_warn << std::endl;
    std::cout << "ERROR: " << runner.tally_err << std::endl;
    std::cout << "REJECTED: " << runner.tally_bad << std::endl;
    return 0;
}
