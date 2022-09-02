#ifndef LIST_HPP_SENTRY
#define LIST_HPP_SENTRY

template <class T>
class ListCursor;

template <class T>
class List
{
    friend class ListCursor<T>;
    struct item
    {
        T *val;
        item *next;
    };
    item *first;

public:
    List() : first(0) {}
    ~List();

    void Push(T *val);
    void Pop();

    T &Get();

    class Iterator
    {
        friend class List<T>;
        List<T>::item *first;
        Iterator(List<T>::item *_first) : first(_first) {}

    public:
        bool More();
        ListCursor<T> Next();
    };

    Iterator *Iterate() { return new Iterator(first); }
};

template <class T>
class ListCursor
{
    typename List<T>::item *data;

public:
    ListCursor(typename List<T>::item *_data) : data(_data) {}

    operator T *() { return data->val; }
    T *operator->() { return data->val; }
};

template <class T>
bool List<T>::Iterator::More()
{
    return first;
}

template <class T>
ListCursor<T> List<T>::Iterator::Next()
{
    ListCursor<T> tmp(first);
    first = first->next;
    return tmp;
}

template <class T>
List<T>::~List()
{
    while (first)
    {
        item *tmp = first;
        first = first->next;
        delete tmp->val;
        delete tmp;
    }
}

template <class T>
void List<T>::Push(T *val)
{
    item *tmp = new item;
    tmp->val = new T(*val);
    tmp->next = first;
    first = tmp;
}

template <class T>
void List<T>::Pop()
{
    item *tmp = first;
    first = first->next;
    delete tmp;
}

template <class T>
T &List<T>::Get()
{
    return first->val;
}

#endif