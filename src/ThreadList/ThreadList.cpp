#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <dirent.h>
#include <string.h>
#include "ThreadList.hpp"

//////////////////////////////////

List<std::thread>::item::~item()
{
    delete val;
}

List<std::thread>::List() : first(0), count(0) {}
List<std::thread>::~List()
{
    while (first)
    {
        item *tmp = first;
        first = first->next;
        delete tmp;
    }
}
void List<std::thread>::Push(void (*func)(List<std::string> *),
                             List<std::string> *_l)
{
    item *tmp = new item;
    tmp->val = new std::thread(func, _l);
    tmp->next = first;
    first = tmp;
    count++;
}
List<std::thread>::Iterator *List<std::thread>::Iterate()
{
    return new List<std::thread>::Iterator(first);
}

//////////////////////////////////

ListCursor<std::thread>::ListCursor(List<std::thread>::item *itm)
    : first(itm) {}
ListCursor<std::thread>::operator std::thread *() { return first->val; }
std::thread *ListCursor<std::thread>::operator->() { return first->val; }

//////////////////////////////////

List<std::thread>::Iterator::
    Iterator(List<std::thread>::item *_first) : first(_first) {}
bool List<std::thread>::Iterator::More()
{
    return first;
}
ListCursor<std::thread> List<std::thread>::Iterator::Next()
{
    ListCursor<std::thread> tmp(first);
    first = first->next;
    return tmp;
}