#include <iostream>
#include <string>
#include <utility>

class StringBuilder
{
    std::string data;

public:
    StringBuilder() = default;

    StringBuilder(StringBuilder&& other) noexcept : data(std::move(other.data)) {}

    StringBuilder& operator=(StringBuilder&& other) noexcept
    {
        if (this != &other) data = std::move(other.data);
        return *this;
    }

    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s)
    {
        data += s;
        return *this;
    }

    StringBuilder& append(std::string&& s)
    {
        data += std::move(s);
        return *this;
    }

    StringBuilder& append_line(const std::string& s)
    {
        data += s;
        data += '\n';
        return *this;
    }

    const std::string& str() const { return data; }

    void clear() { data.clear(); }
};

static void trim_leading(std::string& s)
{
    s.erase(0, s.find_first_not_of(' '));
}

static StringBuilder make_section(std::istream& in)
{
    std::string title;
    std::getline(in, title);
    trim_leading(title);

    StringBuilder section;
    section.append("# ").append(title).append_line("");

    std::string line;
    while (std::getline(in, line))
    {
        if (line.empty()) break;
        section.append_line(line);
    }
    section.append_line("");
    return section;
}

int main()
{
    StringBuilder doc;
    std::string token;

    while (std::cin >> token)
    {
        if (token == "END") break;
        if (token != "SECTION") continue;

        StringBuilder section = make_section(std::cin);
        doc.append(std::move(section.str()));
    }

    std::cout << doc.str();
    return 0;
}
