#include <iostream>
#include <sstream>
#include <string>
#include <vector>
// borrowed from roommate's homework

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::istringstream;
using std::string;
using size_t = std::size_t;

template <typename T>
class LinkedList
{
    struct Node { T item; Node* link; Node(T v): item(v), link(nullptr) {} };
    Node* origin;
    size_t qty;

public:
    LinkedList(): origin(nullptr), qty(0) {}
    ~LinkedList() { purge(); }

    void purge()
    {
        while (origin) { Node* t = origin; origin = origin->link; delete t; }
        qty = 0;
    }

    void push_back(const T& v)
    {
        Node* vertex = new Node(v);
        if (!origin) origin = vertex;
        else
        {
            Node* p = origin;
            while (p->link) p = p->link;
            p->link = vertex;
        }
        qty++;
    }

    size_t size() const { return qty; }

    class Iterator
    {
        Node* spot;
    public:
        Iterator(Node* n): spot(n) {}
        T& operator*() { return spot->item; }
        const T& operator*() const { return spot->item; }
        Iterator& operator++() { spot = spot->link; return *this; }
        bool operator!=(const Iterator& o) { return spot != o.spot; }
    };

    Iterator begin() { return Iterator(origin); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(origin); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& seqA, const LinkedList<T>& seqB)
{
    LinkedList<T> combo;
    auto i = seqA.begin(), j = seqB.begin();
    while (i != seqA.end() && j != seqB.end())
    {
        if (*i <= *j) { combo.push_back(*i); ++i; }
        else { combo.push_back(*j); ++j; }
    }
    for (; i != seqA.end(); ++i) combo.push_back(*i);
    for (; j != seqB.end(); ++j) combo.push_back(*j);
    return combo;
}

LinkedList<int> parseInts(const string& line)
{
    LinkedList<int> bucket;
    istringstream iss(line);
    std::vector<int> buf;
    int v;
    while (iss >> v) buf.push_back(v);
    auto it = buf.begin();
    for (size_t k = buf.size(); k-- > 0; ++it)
        bucket.push_back(*it);
    return bucket;
}

int main()
{
    string rowA, rowB;
    getline(cin, rowA);
    getline(cin, rowB);
    auto output = merge_sorted(parseInts(rowA), parseInts(rowB));
    bool sep = false;
    for (auto it = output.begin(); it != output.end(); ++it)
    {
        if (sep) cout << ' ';
        cout << *it;
        sep = true;
    }
    cout << endl;
    return 0;
}
