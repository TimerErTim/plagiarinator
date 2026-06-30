#include <iostream>
#include <sstream>
#include <string>
// borrowed from roommate's homework

template <typename T>
class LinkedList
{
    struct Node { T payload; Node* link; Node(T d): payload(d), link(0) {} };
    Node* chainFront;
    std::size_t tally;

public:
    LinkedList(): chainFront(0), tally(0) {}
    ~LinkedList()
    {
        Node* cur = chainFront;
        while (cur)
        {
            Node* gone = cur;
            cur = cur->link;
            delete gone;
        }
    }

    void push_back(const T& x)
    {
        Node* nn = new Node(x);
        if (!chainFront) chainFront = nn;
        else
        {
            for (Node* rover = chainFront; ; rover = rover->link)
                if (!rover->link) { rover->link = nn; break; }
        }
        tally++;
    }

    std::size_t size() const { return tally; }

    class Iterator
    {
        Node* spot;
    public:
        Iterator(Node* n): spot(n) {}
        T& operator*() { return spot->payload; }
        const T& operator*() const { return spot->payload; }
        Iterator& operator++() { spot = spot->link; return *this; }
        bool operator!=(const Iterator& x) { return spot != x.spot; }
    };

    Iterator begin() { return Iterator(chainFront); }
    Iterator end() { return Iterator(0); }
    Iterator begin() const { return Iterator(chainFront); }
    Iterator end() const { return Iterator(0); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& srcA, const LinkedList<T>& srcB)
{
    LinkedList<T> merged;
    auto ia = srcA.begin(), ib = srcB.begin();
    while (ia != srcA.end() && ib != srcB.end())
    {
        if (*ia <= *ib) { merged.push_back(*ia); ++ia; }
        else { merged.push_back(*ib); ++ib; }
    }
    auto drain = [&](auto it, auto end) {
        std::size_t k = 0;
        for (auto t = it; t != end; ++t) ++k;
        for (; k-- > 0; ++it) merged.push_back(*it);
    };
    drain(ia, srcA.end());
    drain(ib, srcB.end());
    return merged;
}

int main()
{
    LinkedList<int> rowA, rowB;
    std::string ln;
    std::getline(std::cin, ln);
    { std::istringstream s(ln); int val; while (s >> val) rowA.push_back(val); }
    std::getline(std::cin, ln);
    { std::istringstream s(ln); int val; while (s >> val) rowB.push_back(val); }
    LinkedList<int> finalList = merge_sorted(rowA, rowB);
    bool firstOut = true;
    for (auto p = finalList.begin(); p != finalList.end(); ++p)
    {
        if (!firstOut) std::cout << ' ';
        std::cout << *p;
        firstOut = false;
    }
    std::cout << "\n";
    return 0;
}
