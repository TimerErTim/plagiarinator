#include <iostream>
#include <string>
#include <utility>

class StringBuilder
{
    std::string storage;

public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& from) noexcept : storage(std::move(from.storage)) {}
    StringBuilder& operator=(StringBuilder&& from) noexcept
    {
        storage = std::move(from.storage);
        from.storage.clear();
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& text)
    {
        storage += text;
        return *this;
    }

    StringBuilder& append(std::string&& text)
    {
        storage += std::move(text);
        return *this;
    }

    StringBuilder& append_line(const std::string& text)
    {
        storage.append(text).push_back('\n');
        return *this;
    }

    const std::string& str() const { return storage; }
    void clear() { storage.clear(); }
};

static void strip_leading(std::string& title)
{
    if (title.size() && title[0] == ' ') title = title.substr(1);
}

static std::string build_section_body(std::istream& in, const std::string& title)
{
    std::string body;
    body.reserve(128);
    body += "# ";
    body += title;
    body += "\n";

    std::string row;
    while (std::getline(in, row))
    {
        if (row.empty()) break;
        body += row;
        body += '\n';
    }
    body += '\n';
    return body;
}

int main()
{
    StringBuilder master;
    std::string tag;

    while (std::cin >> tag)
    {
        if (tag == "END") break;
        if (tag != "SECTION") continue;

        std::string title;
        std::getline(std::cin, title);
        strip_leading(title);

        master.append(std::move(build_section_body(std::cin, title)));
    }

    std::cout << master.str();
    return 0;
}
