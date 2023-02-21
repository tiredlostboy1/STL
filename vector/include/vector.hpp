#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

template <class T>
class Vector {
private:
  std::size_t size_;
  std::size_t capacity_;
  T *data_;

public:

    Vector(std::size_t size = 0);

    Vector(const Vector &other);
    Vector &operator=(const Vector &other);
    Vector(Vector &&other);
    Vector &operator=(Vector &&other);

    ~Vector();

    std::size_t size() const;
    const T *data() const;
    bool empty() const;
    std::size_t capacity() const;

    T operator[](std::size_t index) const;
    T &operator[](std::size_t index);

    void resize(std::size_t new_capacity);
    void push_back(const T &x);
    T pop_back();
    void clear();
    void insert(size_t pos, T value);
    T erase(size_t pos);
    
};

#endif