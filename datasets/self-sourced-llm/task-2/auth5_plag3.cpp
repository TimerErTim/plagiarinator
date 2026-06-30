#include <iostream>
#include <sstream>
#include <string>
// borrowed from roommate's homework

template <typename T>
class LinkedList
{
    struct Node { T payload; Node* link; };
    Node* chainHead;
    std::size_t tally;

    void attachTail(Node* fresh)
    {
        if (!chainHead) { chainHead = fresh; return; }
        Node* rover = chainHead;
        while (rover->link) rover = rover->link;
        rover->link = fresh;
    }

public:
    LinkedList() : chainHead(nullptr), tally(0) {}
    ~LinkedList()
    {
        Node* cur = chainHead;
        while (cur) { Node* gone = cur; cur = cur->link; delete gone; }
    }

    void push_back(const T& val)
    {
        attachTail(new Node{val, nullptr});
        ++tally;
    }

    std::size_t size() const { return tally; }

    class Iterator
    {
        Node* spot;
    public:
        Iterator(Node* node) : spot(node) {}
        T& operator*() { return spot->payload; }
        const T& operator*() const { return spot->payload; }
        Iterator& operator++() { spot = spot->link; return *this; }
        bool operator!=(const Iterator& o) { return spot != o.spot; }
    };

    Iterator begin() { return Iterator(chainHead); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(chainHead); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& leftSide, const LinkedList<T>& rightSide)
{
    LinkedList<T> fused;
    auto leftPtr = leftSide.begin(), rightPtr = rightSide.begin();
    while (leftPtr != leftSide.end() && rightPtr != rightSide.end())
    {
        if (*leftPtr <= *rightPtr) { fused.push_back(*leftPtr); ++leftPtr; }
        else { fused.push_back(*rightPtr); ++rightPtr; }
    }
    auto flushRest = [&](auto it, auto end) {
        std::size_t n = 0;
        for (auto t = it; t != end; ++t) ++n;
        for (; n-- > 0; ++it) fused.push_back(*it);
    };
    flushRest(leftPtr, leftSide.end());
    flushRest(rightPtr, rightSide.end());
    return fused;
}

int main()
{
    LinkedList<int> firstList, secondList;
    std::string rawLine;
    std::getline(std::cin, rawLine);
    { std::istringstream in(rawLine); int token; while (in >> token) firstList.push_back(token); }
    std::getline(std::cin, rawLine);
    { std::istringstream in(rawLine); int token; while (in >> token) secondList.push_back(token); }

    LinkedList<int> mergedOut = merge_sorted(firstList, secondList);
    bool gap = false;
    for (auto cursor = mergedOut.begin(); cursor != mergedOut.end(); ++cursor)
    {
        if (gap) std::cout << ' ';
        std::cout << *cursor;
        gap = true;
    }
    std::cout << std::endl;
    return 0;
}
