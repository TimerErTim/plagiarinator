#include <string>
#include <iostream>
#include <sstream>
#include <vector>
// borrowed from roommate's homework

template <typename T>
class LinkedList
{
public:
    class Iterator;

private:
    struct Node
    {
        T datum;
        Node* fwd;
    };
    Node* apex;
    std::size_t tally;

public:
    LinkedList() : apex(nullptr), tally(0) {}

    ~LinkedList()
    {
        while (apex)
        {
            Node* t = apex;
            apex = apex->fwd;
            delete t;
        }
    }

    void push_back(const T& v)
    {
        Node* vert = new Node{v, nullptr};
        if (!apex) apex = vert;
        else
        {
            Node* p = apex;
            while (p->fwd) p = p->fwd;
            p->fwd = vert;
        }
        ++tally;
    }

    std::size_t size() const { return tally; }

    class Iterator
    {
        Node* cur;
        friend class LinkedList;
        Iterator(Node* p) : cur(p) {}
    public:
        T& operator*() { return cur->datum; }
        const T& operator*() const { return cur->datum; }
        Iterator& operator++() { cur = cur->fwd; return *this; }
        bool operator!=(const Iterator& o) const { return cur != o.cur; }
    };

    Iterator begin() { return Iterator(apex); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(apex); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& lhs, const LinkedList<T>& rhs)
{
    LinkedList<T> merged;
    auto li = lhs.begin(), ri = rhs.begin();
    while (li != lhs.end() && ri != rhs.end())
    {
        if (*li <= *ri) { merged.push_back(*li); ++li; }
        else { merged.push_back(*ri); ++ri; }
    }
    while (li != lhs.end()) { merged.push_back(*li); ++li; }
    while (ri != rhs.end()) { merged.push_back(*ri); ++ri; }
    return merged;
}

static void fillFromLine(LinkedList<int>& list, const std::string& input)
{
    std::istringstream ss(input);
    std::vector<int> nums;
    int v;
    while (ss >> v) nums.push_back(v);
    auto it = nums.begin();
    for (std::size_t k = nums.size(); k-- > 0; ++it)
        list.push_back(*it);
}

int main()
{
    std::string rowA, rowB;
    std::getline(std::cin, rowA);
    std::getline(std::cin, rowB);

    LinkedList<int> listA, listB;
    fillFromLine(listA, rowA);
    fillFromLine(listB, rowB);

    LinkedList<int> answer = merge_sorted(listA, listB);
    bool lead = true;
    for (auto it = answer.begin(); it != answer.end(); ++it)
    {
        if (!lead) std::cout << ' ';
        std::cout << *it;
        lead = false;
    }
    std::cout << std::endl;
    return 0;
}
