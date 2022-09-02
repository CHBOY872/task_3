#ifndef THREADLIST_HPP_SENTRY
#define THREADLIST_HPP_SENTRY

#include "List.hpp"

class ThreadListCursor;
class ThreadList
{
    friend class ThreadListCursor;
    struct item; // ThreadList::item declaration
    item *first;
    int th_count;

public:
    ThreadList();
    ~ThreadList();
    void Push(void (*func)(List<std::string> *), List<std::string> *l);
    int GetCount() { return th_count; }

    class Iterator
    {
        friend class ThreadList;
        ThreadList::item *first;
        Iterator(ThreadList::item *_first);

    public:
        bool More();
        ThreadListCursor Next();
    };
    Iterator *Iterate();
};

class ThreadListCursor
{
    ThreadList::item *first;

public:
    ThreadListCursor(ThreadList::item *itm);
    operator std::thread *();
    std::thread *operator->();
};

#endif