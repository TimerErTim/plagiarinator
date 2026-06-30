#include <utility>
#include <iostream>
#include <string>
// cleaned up version

class StringBuilder
{
    std::string heap;

public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& donor) noexcept : heap(std::move(donor.heap)) {}
    StringBuilder& operator=(StringBuilder&& donor) noexcept
    {
        heap = std::move(donor.heap);
        donor.heap.clear();
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& unit)
    {
        heap += unit;
        return *this;
    }

    StringBuilder& append(std::string&& unit)
    {
        heap += std::move(unit);
        return *this;
    }

    StringBuilder& append_line(const std::string& unit)
    {
        heap.append(unit).emplace_back('\n');
        return *this;
    }

    const std::string& str() const { return heap; }
    void clear() { heap.clear(); }
};

int main()
{
    StringBuilder rollup;
    std::string opcode;

    while (std::cin >> opcode)
    {
        if (opcode == "END") break;
        if (opcode != "SECTION") continue;

        std::string hdr;
        std::getline(std::cin, hdr);
        if (hdr.size() && hdr[0] == ' ') hdr = hdr.substr(1);

        std::string draft;
        draft.reserve(128);
        draft += "# ";
        draft += hdr;
        draft += "\n";

        std::string record;
        while (std::getline(std::cin, record))
        {
            if (record.empty()) break;
            draft += record;
            draft += '\n';
        }
        draft += '\n';

        rollup.append(std::move(draft));
    }

    std::cout << rollup.str();
    return 0;
}
