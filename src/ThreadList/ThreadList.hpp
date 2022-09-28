#ifndef THREADLIST_HPP_SENTRY
#define THREADLIST_HPP_SENTRY

#include "../List/List.hpp"

template <>
class List<std::thread>
{
    friend class ListCursor<std::thread>;
    struct item
    {
        std::thread *val;
        item *next;
        ~item();
    };
    item *first;
    int count;

public:
    List();
    ~List();

    void Push(void (*func)(List<std::string> *), List<std::string> *l);
    void Pop();

    int GetCount() { return count; }

    class Iterator
    {
        friend class List<std::thread>;
        List<std::thread>::item *first;
        Iterator(List<std::thread>::item *_first);

    public:
        bool More();
        ListCursor<std::thread> Next();
    };

    Iterator *Iterate();
};

template <>
class ListCursor<std::thread>
{
    List<std::thread>::item *first;

public:
    ListCursor(List<std::thread>::item *itm);
    operator std::thread *();
    std::thread *operator->();
};

#endif