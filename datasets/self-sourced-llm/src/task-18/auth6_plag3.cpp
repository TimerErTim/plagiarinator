#include <iostream>
#include <memory>
#include <string>
// quick submit before deadline

class Parser;

class State
{
public:
    virtual ~State() = default;
    virtual std::unique_ptr<State> handle(Parser& ctx, const std::string& row) = 0;
};

class Parser
{
public:
    std::unique_ptr<State> state;
    int hits_info = 0, hits_warn = 0, hits_error = 0, hits_bad = 0;
};

class HeaderState : public State
{
public:
    std::unique_ptr<State> handle(Parser& ctx, const std::string& row) override;
};

class BodyState : public State
{
public:
    std::unique_ptr<State> handle(Parser& ctx, const std::string& row) override;
};

class DoneState : public State
{
public:
    std::unique_ptr<State> handle(Parser&, const std::string&) override
    {
        return std::unique_ptr<State>(new DoneState());
    }
};

namespace logparse
{

bool level_of(const std::string& row, std::string& token)
{
    if (row.size() < 3 || row.front() != '[') return false;
    std::size_t idx = row.find(']');
    if (idx == std::string::npos) return false;
    token = row.substr(1, idx - 1);
    return token == "INFO" || token == "WARN" || token == "ERROR";
}

void bump(Parser& ctx, const std::string& token)
{
    if (token == "INFO") ++ctx.hits_info;
    else if (token == "WARN") ++ctx.hits_warn;
    else ++ctx.hits_error;
}

}  // namespace logparse

std::unique_ptr<State> HeaderState::handle(Parser& ctx, const std::string& row)
{
    if (row == "END") return std::make_unique<DoneState>();
    std::string token;
    if (logparse::level_of(row, token))
    {
        logparse::bump(ctx, token);
        return std::make_unique<BodyState>();
    }
    ++ctx.hits_bad;
    return std::make_unique<HeaderState>();
}

std::unique_ptr<State> BodyState::handle(Parser& ctx, const std::string& row)
{
    if (row == "END") return std::make_unique<DoneState>();
    std::string token;
    if (logparse::level_of(row, token))
    {
        logparse::bump(ctx, token);
        return std::make_unique<BodyState>();
    }
    ++ctx.hits_bad;
    return std::make_unique<BodyState>();
}

int main()
{
    Parser machine;
    machine.state = std::make_unique<HeaderState>();
    std::string row;
    while (std::getline(std::cin, row))
        machine.state = machine.state->handle(machine, row);

    std::cout << "INFO: " << machine.hits_info << std::endl;
    std::cout << "WARN: " << machine.hits_warn << std::endl;
    std::cout << "ERROR: " << machine.hits_error << std::endl;
    std::cout << "REJECTED: " << machine.hits_bad << std::endl;
    return 0;
}
