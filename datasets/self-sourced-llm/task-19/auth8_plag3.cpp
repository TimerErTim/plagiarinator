#include <utility>
#include <iostream>
#include <string>
// adapted from classmate submission

class StringBuilder
{
    std::string store_;

public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& donor) noexcept : store_(std::move(donor.store_)) {}
    StringBuilder& operator=(StringBuilder&& donor) noexcept
    {
        store_ = std::move(donor.store_);
        donor.store_.clear();
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s)
    {
        store_.append(s);
        return *this;
    }

    StringBuilder& append(std::string&& s)
    {
        store_.append(std::move(s));
        return *this;
    }

    StringBuilder& append_line(const std::string& s)
    {
        store_.append(s);
        store_.append(1, '\n');
        return *this;
    }

    const std::string& str() const { return store_; }
    void clear() { store_.clear(); }
};

int main()
{
    StringBuilder manuscript;
    std::string opcode;

    while (std::cin >> opcode)
    {
        if (opcode == "END") break;
        if (opcode != "SECTION") continue;

        std::string hdr;
        std::getline(std::cin, hdr);
        while (!hdr.empty() && hdr.front() == ' ') hdr.erase(0, 1);

        std::string draft;
        draft += "# ";
        draft += hdr;
        draft += "\n";

        std::string row;
        while (std::getline(std::cin, row))
        {
            if (row.empty()) break;
            draft += row;
            draft += '\n';
        }
        draft += '\n';

        StringBuilder tmp;
        tmp.append(std::move(draft));
        manuscript.append(std::move(tmp.str()));
    }

    std::cout << manuscript.str();
    return 0;
}
