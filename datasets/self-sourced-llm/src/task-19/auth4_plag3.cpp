#include <utility>
#include <iostream>
#include <string>
// quick submit before deadline

class StringBuilder
{
public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& rhs) noexcept;
    StringBuilder& operator=(StringBuilder&& rhs) noexcept;
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s);
    StringBuilder& append(std::string&& s);
    StringBuilder& append_line(const std::string& s);
    const std::string& str() const { return bytes_; }
    void clear() { bytes_.clear(); }

private:
    std::string bytes_;
};

StringBuilder::StringBuilder(StringBuilder&& rhs) noexcept : bytes_(std::move(rhs.bytes_))
{
    rhs.bytes_.clear();
}

StringBuilder& StringBuilder::operator=(StringBuilder&& rhs) noexcept
{
    bytes_ = std::move(rhs.bytes_);
    rhs.bytes_.clear();
    return *this;
}

StringBuilder& StringBuilder::append(const std::string& s)
{
    bytes_ += s;
    return *this;
}

StringBuilder& StringBuilder::append(std::string&& s)
{
    bytes_ += std::move(s);
    return *this;
}

StringBuilder& StringBuilder::append_line(const std::string& s)
{
    bytes_ += s;
    bytes_ += '\n';
    return *this;
}

int main()
{
    StringBuilder output;
    std::string cmd_token;

    while (std::cin >> cmd_token)
    {
        if (cmd_token == "END") break;
        if (cmd_token != "SECTION") continue;

        std::string caption;
        std::getline(std::cin, caption);
        while (!caption.empty() && caption[0] == ' ') caption.erase(0, 1);

        StringBuilder chunk;
        chunk.append("# ").append(caption).append("\n");

        std::string row;
        while (std::getline(std::cin, row) && !row.empty())
            chunk.append_line(row);
        chunk.append("\n");

        output.append(std::move(chunk.str()));
    }

    std::cout << output.str();
    return 0;
}
