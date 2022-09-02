#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <dirent.h>
#include <string.h>
#include "ThreadList.hpp"

struct ThreadList::item // ThreadList::item description
{
    std::thread *thr;
    item *next;
    ~item();
};

//////////////////////////////////

ThreadList::item::~item()
{
    delete thr;
}
ThreadList::ThreadList() : first(0), th_count(0) {}
ThreadList::~ThreadList()
{
    while (first)
    {
        item *tmp = first;
        first = first->next;
        delete tmp;
    }
}
void ThreadList::Push(void (*func)(List<std::string> *), List<std::string> *_l)
{
    item *tmp = new item;
    tmp->thr = new std::thread(func, _l);
    tmp->next = first;
    first = tmp;
    th_count++;
}
ThreadList::Iterator *ThreadList::Iterate()
{
    return new ThreadList::Iterator(first);
}

//////////////////////////////////

ThreadListCursor::ThreadListCursor(ThreadList::item *itm)
    : first(itm) {}
ThreadListCursor::operator std::thread *() { return first->thr; }
std::thread *ThreadListCursor::operator->() { return first->thr; }

//////////////////////////////////

ThreadList::Iterator::
    Iterator(ThreadList::item *_first) : first(_first) {}
bool ThreadList::Iterator::More()
{
    return first;
}
ThreadListCursor ThreadList::Iterator::Next()
{
    ThreadListCursor tmp(first);
    first = first->next;
    return tmp;
}