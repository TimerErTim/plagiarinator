#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

template <typename... Args>
void log_line(std::ostream& out, Args&&... args)
{
    ((out << std::forward<Args>(args) << " "), ...);
    out << '\n';
}

class Config
{
    std::unordered_map<std::string, std::string> entries_;

public:
    void read(std::istream& in)
    {
        std::string line;
        while (std::getline(in, line) && !line.empty())
        {
            auto pos = line.find('=');
            if (pos != std::string::npos)
                entries_[line.substr(0, pos)] = line.substr(pos + 1);
        }
    }

    bool has(const std::string& key) const
    {
        return entries_.find(key) != entries_.end();
    }

    const std::string& get(const std::string& key) const
    {
        return entries_.at(key);
    }
};

static bool looks_int(const std::string& s)
{
    if (s.empty()) return false;
    std::size_t i = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    if (i >= s.size()) return false;
    for (; i < s.size(); ++i)
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    return true;
}

struct ParsedArg
{
    bool as_int;
    int num;
    std::string text;
};

static ParsedArg decode_token(const std::string& t)
{
    if (looks_int(t)) return ParsedArg{true, std::stoi(t), {}};
    return ParsedArg{false, 0, t};
}

static void write_log_args(std::ostream& out, const std::vector<ParsedArg>& args)
{
    if (args.size() == 1)
    {
        if (args[0].as_int) log_line(out, args[0].num);
        else log_line(out, args[0].text);
    }
    else if (args.size() == 2)
    {
        if (args[0].as_int && args[1].as_int) log_line(out, args[0].num, args[1].num);
        else if (args[0].as_int) log_line(out, args[0].num, args[1].text);
        else if (args[1].as_int) log_line(out, args[0].text, args[1].num);
        else log_line(out, args[0].text, args[1].text);
    }
    else if (args.size() == 3)
    {
        const ParsedArg& a0 = args[0], &a1 = args[1], &a2 = args[2];
        if (a1.as_int)
        {
            if (a0.as_int && a2.as_int) log_line(out, a0.num, a1.num, a2.num);
            else if (a0.as_int) log_line(out, a0.num, a1.num, a2.text);
            else if (a2.as_int) log_line(out, a0.text, a1.num, a2.num);
            else log_line(out, a0.text, a1.num, a2.text);
        }
        else
        {
            log_line(out, a0.as_int ? std::to_string(a0.num) : a0.text, a1.text,
                     a2.as_int ? std::to_string(a2.num) : a2.text);
        }
    }
    else if (args.empty())
    {
        out << '\n';
    }
    else
    {
        for (std::size_t j = args.size(); j-- > 0;)
        {
            if (j + 1 < args.size()) out << ' ';
            if (args[j].as_int) out << args[j].num;
            else out << args[j].text;
        }
        out << '\n';
    }
}

static void putback_word(std::istream& in, const std::string& w)
{
    for (auto it = w.rbegin(); it != w.rend(); ++it)
        in.putback(*it);
}

int main()
{
    Config cfg;
    cfg.read(std::cin);

    std::string cmd;
    while (std::cin >> cmd && cmd != "END")
    {
        if (cmd == "GET")
        {
            std::string key;
            std::cin >> key;
            std::cout << (cfg.has(key) ? cfg.get(key) : std::string("MISSING")) << '\n';
        }
        else if (cmd == "LOG")
        {
            std::vector<std::string> tokens;
            std::string tok;
            while (std::cin >> tok)
            {
                if (tok == "GET" || tok == "LOG" || tok == "END")
                {
                    putback_word(std::cin, tok);
                    break;
                }
                tokens.push_back(tok);
            }
            std::vector<ParsedArg> parsed;
            for (const auto& t : tokens) parsed.push_back(decode_token(t));
            write_log_args(std::cout, parsed);
        }
    }
    return 0;
}
