#include <iostream>
#include <memory>
#include <string>
// copied from stackoverflow-ish snippet

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
    int n_info = 0, n_warn = 0, n_err = 0, n_skip = 0;

    void note(const std::string& tag)
    {
        if (tag == "INFO") ++n_info;
        else if (tag == "WARN") ++n_warn;
        else if (tag == "ERROR") ++n_err;
    }
};

class HeaderState : public State
{
    std::unique_ptr<State> handle(Parser& ctx, const std::string& row) override;
};

class BodyState : public State
{
    std::unique_ptr<State> handle(Parser& ctx, const std::string& row) override;
};

class DoneState : public State
{
    std::unique_ptr<State> handle(Parser&, const std::string&) override
    {
        return std::unique_ptr<State>(new DoneState());
    }
};

namespace
{

bool scan_row(const std::string& row, std::string& tag)
{
    std::size_t rb = row.find(']');
    if (row.size() < 4 || row[0] != '[' || rb == std::string::npos) return false;
    tag = row.substr(1, rb - 1);
    return tag == "INFO" || tag == "WARN" || tag == "ERROR";
}

std::unique_ptr<State> body_step(Parser& ctx, const std::string& row)
{
    if (row == "END") return std::make_unique<DoneState>();
    std::string tag;
    if (scan_row(row, tag))
    {
        ctx.note(tag);
        return std::make_unique<BodyState>();
    }
    ++ctx.n_skip;
    return std::make_unique<BodyState>();
}

}  // namespace

std::unique_ptr<State> HeaderState::handle(Parser& ctx, const std::string& row)
{
    if (row == "END") return std::make_unique<DoneState>();
    std::string tag;
    if (scan_row(row, tag))
    {
        ctx.note(tag);
        return std::make_unique<BodyState>();
    }
    ++ctx.n_skip;
    return std::make_unique<HeaderState>();
}

std::unique_ptr<State> BodyState::handle(Parser& ctx, const std::string& row)
{
    return body_step(ctx, row);
}

int main()
{
    Parser engine;
    engine.state.reset(new HeaderState());
    std::string row;
    while (std::getline(std::cin, row))
        engine.state = engine.state->handle(engine, row);

    std::cout << "INFO: " << engine.n_info << std::endl;
    std::cout << "WARN: " << engine.n_warn << std::endl;
    std::cout << "ERROR: " << engine.n_err << std::endl;
    std::cout << "REJECTED: " << engine.n_skip << std::endl;
    return 0;
}
