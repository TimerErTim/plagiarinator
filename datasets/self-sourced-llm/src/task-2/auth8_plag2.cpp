#include <iostream>
#include <sstream>
#include <string>
#include <utility>

template <typename T>
class LinkedList
{
    struct Node
    {
        T val;
        Node* next;
    };

    Node* headPtr;
    std::size_t itemCount;

    static void destroyChain(Node* n)
    {
        while (n)
        {
            Node* save = n->next;
            delete n;
            n = save;
        }
    }

public:
    LinkedList() : headPtr(nullptr), itemCount(0) {}
    ~LinkedList() { destroyChain(headPtr); }

    void push_back(const T& value)
    {
        Node* node = new Node{value, nullptr};
        if (!headPtr)
            headPtr = node;
        else
        {
            Node* tail = headPtr;
            while (tail->next) tail = tail->next;
            tail->next = node;
        }
        itemCount++;
    }

    std::size_t size() const { return itemCount; }

    class Iterator
    {
        Node* current;
    public:
        Iterator(Node* n) : current(n) {}
        T& operator*() { return current->val; }
        const T& operator*() const { return current->val; }
        Iterator& operator++() { current = current->next; return *this; }
        bool operator!=(const Iterator& rhs) const { return current != rhs.current; }
    };

    Iterator begin() { return Iterator(headPtr); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(headPtr); }
    Iterator end() const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T> merge_sorted(const LinkedList<T>& lhs, const LinkedList<T>& rhs)
{
    LinkedList<T> output;
    auto leftIt = lhs.begin();
    auto rightIt = rhs.begin();

    while (leftIt != lhs.end() && rightIt != rhs.end())
    {
        if (*leftIt <= *rightIt)
        {
            output.push_back(*leftIt);
            ++leftIt;
        }
        else
        {
            output.push_back(*rightIt);
            ++rightIt;
        }
    }
    for (; leftIt != lhs.end(); ++leftIt)
        output.push_back(*leftIt);
    for (; rightIt != rhs.end(); ++rightIt)
        output.push_back(*rightIt);
    return output;
}

int main()
{
    LinkedList<int> listOne, listTwo;
    for (int pass = 0; pass < 2; ++pass)
    {
        std::string text;
        std::getline(std::cin, text);
        std::istringstream stream(text);
        int number;
        LinkedList<int>& target = pass ? listTwo : listOne;
        while (stream >> number)
            target.push_back(number);
    }

    LinkedList<int> merged = merge_sorted(listOne, listTwo);

    bool first = true;
    for (auto it = merged.begin(); it != merged.end(); ++it)
    {
        if (!first) std::cout << ' ';
        std::cout << *it;
        first = false;
    }
    std::cout << '\n';
    return 0;
}
