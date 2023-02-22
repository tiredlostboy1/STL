#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <list>

template <class T>
class Queue
{
    std::list<T> data_;

public:
    // Создает пустую очередь
    Queue();

    // Создает новую очередь, являющююся глубокой копией очереди other [O(n)]
    Queue(const Queue &other);

    // Перезаписывает текущую очередь очередью other
    Queue &operator=(const Queue &other);

    // Создает новую очередь перемещая существующую
    Queue(Queue &&other);

    // Перезаписывает текущую очередь очередью other
    Queue &operator=(Queue &&other);

    // Очищает память очереди
    ~Queue();

    // Возвращает размер очереди (сколько памяти уже занято)
    std::size_t size() const;

    // Проверяет является ли контейнер пустым
    bool empty() const;

    // Получает ссылку на первый элемент очереди
    T &front();

    // Получает ссылку на последний элемент очереди
    T &back();

    const T &front() const;
    const T &back() const;

    // Добавляет элемент в конец очереди.
    void push(T &&x);

    // Добавляет элемент в конец очереди.
    void push(const T &x);

    // Удаляет элемент из начала очереди. Возвращает удаленный элемент.
    T pop();

    // Меняет содержимое с другой очередью. q1.swap(q2);
    void swap(Queue &other);
};

#endif