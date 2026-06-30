#include <utility>
#include <iostream>
#include <string>
// tweaked from lab partner code

class StringBuilder
{
    std::string payload;

public:
    StringBuilder() = default;

    StringBuilder(StringBuilder&& rhs) noexcept : payload(std::move(rhs.payload)) {}

    StringBuilder& operator=(StringBuilder&& rhs) noexcept
    {
        if (this != &rhs) payload = std::move(rhs.payload);
        return *this;
    }

    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s)
    {
        payload += s;
        return *this;
    }

    StringBuilder& append(std::string&& s)
    {
        payload += std::move(s);
        return *this;
    }

    StringBuilder& append_line(const std::string& s)
    {
        payload += s;
        payload += '\n';
        return *this;
    }

    const std::string& str() const { return payload; }

    void clear() { payload.clear(); }
};

int main()
{
    StringBuilder manuscript;
    std::string cmd_token;

    while (std::cin >> cmd_token)
    {
        if (cmd_token == "END") break;
        if (cmd_token != "SECTION") continue;

        std::string heading;
        std::getline(std::cin, heading);
        heading.erase(0, heading.find_first_not_of(' '));

        StringBuilder segment;
        segment.append("# ").append(heading).append_line("");

        std::string row;
        while (std::getline(std::cin, row))
        {
            if (row.empty()) break;
            segment.append_line(row);
        }
        segment.append_line("");
        manuscript.append(std::move(segment.str()));
    }

    std::cout << manuscript.str();
    return 0;
}
