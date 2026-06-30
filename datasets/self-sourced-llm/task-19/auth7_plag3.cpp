#include <utility>
#include <iostream>
#include <string>
// borrowed from roommate's homework

class StringBuilder
{
public:
    StringBuilder() = default;

    StringBuilder(StringBuilder&& donor) noexcept : heap_(std::move(donor.heap_))
    {
        donor.heap_.clear();
    }

    StringBuilder& operator=(StringBuilder&& donor) noexcept
    {
        heap_ = std::move(donor.heap_);
        donor.heap_.clear();
        return *this;
    }

    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& unit)
    {
        heap_.append(unit);
        return *this;
    }

    StringBuilder& append(std::string&& unit)
    {
        heap_.append(std::move(unit));
        return *this;
    }

    StringBuilder& append_line(const std::string& unit)
    {
        heap_.append(unit);
        heap_.push_back('\n');
        return *this;
    }

    const std::string& str() const { return heap_; }
    void clear() { heap_.clear(); }

private:
    std::string heap_;
};

static void ingest_block(StringBuilder& rollup, std::istream& in)
{
    std::string hdr;
    std::getline(in, hdr);
    if (!hdr.empty() && hdr[0] == ' ') hdr.erase(0, 1);

    StringBuilder local;
    local.append("# ").append(hdr).append("\n");

    std::string row;
    while (std::getline(in, row))
    {
        if (row.empty()) break;
        local.append_line(row);
    }
    local.append("\n");
    rollup.append(std::move(local.str()));
}

int main()
{
    StringBuilder manuscript;
    std::string opcode;

    while (std::cin >> opcode)
    {
        if (opcode == "END") break;
        if (opcode == "SECTION") ingest_block(manuscript, std::cin);
    }

    std::cout << manuscript.str();
    return 0;
}
