#include "vector.hpp"

// class Iterator{};
// Создает вектор размера size заполненный дефолтными значениями типа T
template <class T>
Vector<T>::Vector(std::size_t size)
    : size_{size}, capacity_{2 * size + 1}, data_{new T[capacity_]{}} {}

// Создает новый вектор, являющийся глубокой копией вектора other
template <class T>
Vector<T>::Vector(const Vector &other)
    : size_{other.size_}, capacity_{other.capacity_},
      data_{new T[other.capacity_]{}}
{
    for (std::size_t i = 0; i < other.size_; i++)
    {
        data_[i] = other.data_[i];
    }
}

// Перезаписывает текущий вектор вектором other
// Vector v1{5}, v2{10};
// v1 = v2;
// std::cout << v1.size() == 10 << std::endl // True
template <class T>
Vector<T> &Vector<T>::operator=(const Vector<T> &other)
{
    Vector tmp{other};
    std::swap(data_, tmp.data_);
    std::swap(size_, tmp.size_);
    std::swap(capacity_, tmp.capacity_);
    return *this;
}

template <class T>
Vector<T>::Vector(Vector &&other)
    : size_{other.size}, capacity_{other.capacity}, data_{other.data_}
{
    other.data_ = nullptr;
    other.size_ = other.capacity_ = 0;
}

template <class T>
Vector<T> &Vector<T>::operator=(Vector<T> &&other)
{
    Vector tmp{std::move(other)};
    std::swap(data_, tmp.data_);
    std::swap(size_, tmp.size_);
    std::swap(capacity_, tmp.capacity_);
    return *this;
}

// Очищает память вектора
template <class T>
Vector<T>::~Vector() { delete[] data_; }

// Возвращает размер вектора (сколько памяти уже занято)
template <class T>
std::size_t Vector<T>::size() const { return size_; }

template <class T>
const T* Vector<T>::data() const { return data_; }

// Проверяет является ли контейнер пустым
template <class T>
bool Vector<T>::empty() const { return size_ == 0; }

// Возвращает размер выделенной памяти
template <class T>
std::size_t Vector<T>::capacity() const { return capacity_; }

// Возвращает копию элемента по индексу
template <class T>
T Vector<T>::operator[](std::size_t index) const
{
    if (index >= size_)
    {
        throw std::out_of_range("error");
    }
    return data_[index];
}

// Возвращает ссылку на элемент по индексу (позволяет менять элемент, типа
// v[5] = 42;)
template <class T>
T &Vector<T>::operator[](std::size_t index)
{
    if (index >= size_)
    {
        throw std::out_of_range("error");
    }
    return data_[index];
}

// Перевыделяет память:
// * выделяет новый кусок памяти размером равным new_capacityacity,
// * копирует столько старых элементов, сколько влезает,
// * удаляет старый кусок памяти.
template <class T>
void Vector<T>::resize(std::size_t new_capacity)
{
    // важное замечание - resize в std::vector работает не так
    T *data_resized = new T[new_capacity]{};

    if (size_ >= new_capacity)
    {
        size_ = new_capacity;
    }
    for (std::size_t i = 0; i < size_; i++)
    {
        data_resized[i] = data_[i];
    }

    capacity_ = new_capacity;
    delete[] data_;
    data_ = data_resized;
}

// Добавляет элемент в конец вектора. Если нужно перевыделяет память
template <class T>
void Vector<T>::push_back(const T &x)
{
    if (size_ == capacity_)
    {
        resize(2 * capacity_);
    }
    data_[size_++] = x;
}

// Удаляет последний элемент вектора.
template <class T>
T Vector<T>::pop_back()
{
    if (size_ == 0)
    {
        throw std::out_of_range("error");
    }
    // if (size_ == capacity_ / 4) {
    //   resize(capacity_ / 2);
    // }
    return data_[--size_];
}

// Очищает вектор (выделенная память остает выделенной)
template <class T>
void Vector<T>::clear() { size_ = 0; }

// Вставляет новый элемент value на место pos.
// [1, 2, 3].insert(1, 42) -> [1, 42, 2, 3]
template <class T>
void Vector<T>::insert(size_t pos, T value)
{
    push_back(value);
    for (std::size_t i = size_ - 1; i > pos; i--)
    {
        std::swap(data_[i], data_[i - 1]);
    }
}

// Удаляет элемент с идексом pos. Возвращает удаленный элемент.
// [1, 2, 3].erase(1) -> [1, 3] (return 2)
template <class T>
T Vector<T>::erase(size_t pos)
{
    for (std::size_t i = pos; i < size_ - 1; i++)
    {
        std::swap(data_[i], data_[i + 1]);
    }
    return pop_back();
}
