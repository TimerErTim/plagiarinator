#include <sstream>
#include <string>
#include <iostream>
#include <vector>
// borrowed from roommate's homework

template <typename T>
class LinkedList
{
    struct Node
    {
        T payload;
        Node* nxt;
        explicit Node(const T& v) : payload(v), nxt(nullptr) {}
    };

    Node* origin;
    std::size_t tally;

    Node* lastLink() const
    {
        if (!origin) return nullptr;
        Node* p = origin;
        while (p->nxt) p = p->nxt;
        return p;
    }

public:
    LinkedList() : origin(nullptr), tally(0) {}

    ~LinkedList()
    {
        Node* step = origin;
        while (step)
        {
            Node* follow = step->nxt;
            delete step;
            step = follow;
        }
    }

    void push_back(const T& item)
    {
        Node* made = new Node(item);
        if (!origin) origin = made;
        else lastLink()->nxt = made;
        ++tally;
    }

    std::size_t size() const { return tally; }

    class Iterator
    {
        Node* cursor;
    public:
        Iterator(Node* n, bool) : cursor(n) {}
        const T& operator*() const { return cursor->payload; }
        Iterator& operator++() { cursor = cursor->nxt; return *this; }
        bool operator!=(const Iterator& rhs) const { return cursor != rhs.cursor; }
    };

    Iterator begin() const { return Iterator(origin, true); }
    Iterator end() const { return Iterator(nullptr, true); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& leftSeq, const LinkedList<T>& rightSeq)
{
    LinkedList<T> mergedOut;
    auto ix = leftSeq.begin(), iy = rightSeq.begin();
    while (ix != leftSeq.end() && iy != rightSeq.end())
    {
        if (*ix <= *iy)
        {
            mergedOut.push_back(*ix);
            ++ix;
        }
        else
        {
            mergedOut.push_back(*iy);
            ++iy;
        }
    }
    for (; ix != leftSeq.end(); ++ix) mergedOut.push_back(*ix);
    for (; iy != rightSeq.end(); ++iy) mergedOut.push_back(*iy);
    return mergedOut;
}

LinkedList<int> loadList(std::istream& in)
{
    LinkedList<int> bucket;
    std::string record;
    std::getline(in, record);
    std::istringstream engine(record);
    std::vector<int> buf;
    int num;
    while (engine >> num) buf.push_back(num);
    auto it = buf.begin();
    for (std::size_t k = buf.size(); k-- > 0; ++it)
        bucket.push_back(*it);
    return bucket;
}

int main()
{
    LinkedList<int> leftSeq = loadList(std::cin);
    LinkedList<int> rightSeq = loadList(std::cin);
    LinkedList<int> mergedOut = merge_sorted(leftSeq, rightSeq);
    bool gap = false;
    for (auto it = mergedOut.begin(); it != mergedOut.end(); ++it)
    {
        if (gap) std::cout << ' ';
        std::cout << *it;
        gap = true;
    }
    std::cout << std::endl;
    return 0;
}
