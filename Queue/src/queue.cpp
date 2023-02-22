#include "queue.hpp"

template <class T>
Queue<T>::Queue() = default;

template <class T>
Queue<T>::Queue(const Queue<T> &other) = default;

template <class T>
Queue<T>& Queue<T>::operator=(const Queue<T> &other) = default;

template <class T>
Queue<T>::Queue(Queue<T> &&other) = default;

template <class T>
Queue<T>& Queue<T>::operator=(Queue<T> &&other) = default;

template <class T>
Queue<T>::~Queue() = default;

template <class T>
std::size_t Queue<T>::size() const
{
    return data_.size();
}

template <class T>
bool Queue<T>::empty() const
{
    return data_.empty();
}

template <class T>
T &Queue<T>::front()
{
    return data_.front();
}

template <class T>
T &Queue<T>::back()
{
    return data_.back();
}

template <class T>
const T &Queue<T>::front() const
{
    return data_.front();
}

template <class T>
const T &Queue<T>::back() const
{
    return data_.back();
}

template <class T>
void Queue<T>::push(T &&x)
{
    data_.push_back(std::move(x));
}

template <class T>
void Queue<T>::push(const T &x)
{
    data_.push_back(x);
}
template <class T>
T Queue<T>::pop()
{
    T tmp = front();
    data_.pop_front();
    return tmp;
}
template <class T>
void Queue<T>::swap(Queue &other)
{
    data_.swap(other.a);
}