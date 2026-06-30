#include <utility>
#include <iostream>
#include <string>
// adapted from classmate submission

class StringBuilder
{
    std::string heap_;

public:
    StringBuilder() = default;

    StringBuilder(StringBuilder&& donor) noexcept : heap_(std::exchange(donor.heap_, {})) {}

    StringBuilder& operator=(StringBuilder&& donor) noexcept
    {
        if (this != &donor) heap_ = std::exchange(donor.heap_, {});
        return *this;
    }

    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& piece)
    {
        heap_.append(piece);
        return *this;
    }

    StringBuilder& append(std::string&& piece)
    {
        heap_.append(std::move(piece));
        return *this;
    }

    StringBuilder& append_line(const std::string& row)
    {
        append(row);
        heap_.push_back('\n');
        return *this;
    }

    const std::string& str() const { return heap_; }

    void clear() { heap_.clear(); }
};

static StringBuilder load_part(std::istream& in)
{
    std::string title_line;
    std::getline(in, title_line);
    if (!title_line.empty() && title_line.front() == ' ')
        title_line.erase(0, 1);

    StringBuilder segment;
    segment.append("# ").append(title_line).append_line("");

    std::string content;
    while (std::getline(in, content))
    {
        if (content.empty()) break;
        segment.append_line(content);
    }
    segment.append_line("");
    return segment;
}

int main()
{
    StringBuilder manuscript;
    std::string verb;

    while (std::cin >> verb)
    {
        if (verb == "END") break;
        if (verb == "SECTION")
        {
            StringBuilder sec = load_part(std::cin);
            manuscript.append(std::move(sec.str()));
        }
    }

    std::cout << manuscript.str();
    return 0;
}
