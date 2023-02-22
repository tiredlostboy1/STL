#include "stack.hpp"

template <class T>
Stack<T>::Stack() = default;

template <class T>
Stack<T>::Stack(const Stack &other) = default;

template <class T>
Stack<T> &Stack<T>::operator=(const Stack &other) = default;

template <class T>
Stack<T>::~Stack() = default;

template <class T>
std::size_t Stack<T>::size()
{
  return data.size();
}

template <class T>
bool Stack<T>::empty()
{
  return data.empty();
}

template <class T>
void Stack<T>::push_back(const T &x)
{
  data.push_back(x);
}

template <class T>
T &Stack<T>::top()
{
  if (data.empty())
  {
    throw std::out_of_range{"empty container"};
  }
  return data.back();
}

template <class T>
T Stack<T>::pop()
{
  T tmp = top();
  data.pop_back();
  return tmp;
}

template <class T>
void Stack<T>::swap(Stack &other)
{
  data.swap(other.data);
}
