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
        Node* succ;
        Node(const T& v) : payload(v), succ(nullptr) {}
    };
    Node* front_;
    std::size_t tally_;

public:
    LinkedList() : front_(nullptr), tally_(0) {}
    ~LinkedList()
    {
        while (front_)
        {
            Node* spare = front_;
            front_ = front_->succ;
            delete spare;
        }
    }

    void push_back(const T& v)
    {
        Node* fresh = new Node(v);
        if (!front_) front_ = fresh;
        else
        {
            Node* trail = front_;
            while (trail->succ) trail = trail->succ;
            trail->succ = fresh;
        }
        ++tally_;
    }

    std::size_t size() const { return tally_; }

    class Iterator
    {
        Node* spot_;
    public:
        explicit Iterator(Node* n) : spot_(n) {}
        T& operator*() const { return spot_->payload; }
        Iterator& operator++() { spot_ = spot_->succ; return *this; }
        bool operator!=(const Iterator& o) const { return spot_ != o.spot_; }
    };

    Iterator begin() const { return Iterator(front_); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& seqA, const LinkedList<T>& seqB)
{
    LinkedList<T> combo;
    auto pa = seqA.begin(), pb = seqB.begin();
    auto za = seqA.end(), zb = seqB.end();
    while (pa != za && pb != zb)
    {
        if (*pa <= *pb) { combo.push_back(*pa); ++pa; }
        else { combo.push_back(*pb); ++pb; }
    }
    while (pa != za) { combo.push_back(*pa); ++pa; }
    while (pb != zb) { combo.push_back(*pb); ++pb; }
    return combo;
}

static LinkedList<int> readLine(std::istream& in)
{
    LinkedList<int> bucket;
    std::string raw;
    if (!std::getline(in, raw)) return bucket;
    std::istringstream ss(raw);
    std::vector<int> buf;
    int x;
    while (ss >> x) buf.push_back(x);
    auto it = buf.begin();
    for (std::size_t k = buf.size(); k-- > 0; ++it)
        bucket.push_back(*it);
    return bucket;
}

int main()
{
    LinkedList<int> streamA = readLine(std::cin);
    LinkedList<int> streamB = readLine(std::cin);
    LinkedList<int> answer = merge_sorted(streamA, streamB);
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
