#include <string>
#include <utility>
#include <sstream>
#include <iostream>

class StringBuilder {
public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& o) noexcept : chunks_(std::move(o.chunks_)) {}
    StringBuilder& operator=(StringBuilder&& o) noexcept {
        chunks_ = std::move(o.chunks_);
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) {
        chunks_ += s;
        return *this;
    }

    StringBuilder& append(std::string&& s) {
        chunks_ += std::move(s);
        return *this;
    }

    StringBuilder& append_line(const std::string& s) {
        chunks_.append(s);
        chunks_.emplace_back('\n');
        return *this;
    }

    const std::string& str() const { return chunks_; }
    void clear() { chunks_.clear(); }

private:
    std::string chunks_;
};

int main() {
    StringBuilder document;

    for (std::string cmd; std::cin >> cmd;) {
        if (cmd == "END") break;
        if (cmd != "SECTION") continue;

        std::string heading;
        std::getline(std::cin, heading);
        if (!heading.empty() && heading[0] == ' ') heading.erase(0, 1);

        std::ostringstream block;
        block << "# " << heading << std::endl;

        std::string entry;
        while (std::getline(std::cin, entry)) {
            if (entry.empty()) break;
            block << entry << std::endl;
        }
        block << std::endl;

        document.append(std::move(block.str()));
    }

    std::cout << document.str();
    return 0;
}
