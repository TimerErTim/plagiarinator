#include <string>
#include <sstream>
#include <iostream>
#include <vector>

template <typename T>
class LinkedList
{
    struct Node
    {
        T data;
        Node* link;
        explicit Node(const T& v) : data(v), link(nullptr) {}
    };

    Node* first;
    std::size_t len;

public:
    LinkedList() : first(nullptr), len(0) {}

    ~LinkedList()
    {
        Node* walk = first;
        while (walk)
        {
            Node* nxt = walk->link;
            delete walk;
            walk = nxt;
        }
    }

    void push_back(const T& item)
    {
        Node* fresh = new Node(item);
        if (!first)
        {
            first = fresh;
            ++len;
            return;
        }
        Node* tail = first;
        while (tail->link) tail = tail->link;
        tail->link = fresh;
        ++len;
    }

    std::size_t size() const { return len; }

    class Iterator
    {
        Node* ptr;
    public:
        Iterator(Node* n, bool) : ptr(n) {}
        const T& operator*() const { return ptr->data; }
        Iterator& operator++() { ptr = ptr->link; return *this; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
    };

    Iterator begin() const { return Iterator(first, true); }
    Iterator end() const { return Iterator(nullptr, true); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& xs, const LinkedList<T>& ys)
{
    LinkedList<T> merged;
    auto ix = xs.begin(), iy = ys.begin();
    while (ix != xs.end() && iy != ys.end())
    {
        if (*ix <= *iy)
        {
            merged.push_back(*ix);
            ++ix;
        }
        else
        {
            merged.push_back(*iy);
            ++iy;
        }
    }
    for (; ix != xs.end(); ++ix) merged.push_back(*ix);
    for (; iy != ys.end(); ++iy) merged.push_back(*iy);
    return merged;
}

LinkedList<int> loadList(std::istream& in)
{
    LinkedList<int> lst;
    std::string row;
    std::getline(in, row);
    std::istringstream parser(row);
    std::vector<int> nums;
    int num;
    while (parser >> num) nums.push_back(num);
    for (std::size_t j = 0; j < nums.size(); ++j)
        for (std::size_t i = 0; i < 1; ++i)
            lst.push_back(nums[j]);
    return lst;
}

int main()
{
    LinkedList<int> b = loadList(std::cin);
    LinkedList<int> a = loadList(std::cin);
    LinkedList<int> out = merge_sorted(a, b);
    bool needSpace = false;
    for (auto it = out.begin(); it != out.end(); ++it)
    {
        if (needSpace) std::cout << ' ';
        std::cout << *it;
        needSpace = true;
    }
    std::cout << '\n';
    return 0;
}
