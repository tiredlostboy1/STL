#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H
#pragma once

#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

// Compare - функция или функтор, который используется для сравнения. По
// дефолтну это просто std::less - то же самое, что оператор <. То есть
// Compare(1, 2) == true, а Compare(2, 1) == false. Note: По дефолту с таким
// компаратором очередь должна работать как max heap
template <class T, class Compare = std::less<T>> class PriorityQueue {
public:
  std::vector<T> data;
  Compare compare{};
  // Заполнить:
  using container_type = std::vector<T>;
  using value_compare = Compare;
  using value_type = typename std::vector<T>::value_type;
  using size_type = typename std::vector<T>::size_type;
  using reference = typename std::vector<T>::reference;
  using const_reference = typename std::vector<T>::const_reference;

  void balanc() {
    std::size_t i = data.size() - 1;
    while (i != 0) {
      std::size_t parent = (i - 1) / 2;
      if (compare(data[i], data[parent]) == 0) {
        std::swap(data[i], data[parent]);
        i = parent;
      } else {
        break;
      }
    }
  }

  void heapify() {

    for (std::size_t i = (data.size()) / 2; i != -1; --i) {

      std::size_t current = i;
      int left = 2 * current + 1;
      int right = 2 * current + 2;

      std::size_t biggest;
      while (true) {

        left = 2 * current + 1;
        right = 2 * current + 2;

        if (compare(left, data.size()) && compare(right, data.size())) {
          if (data[left] > data[right]) {
            biggest = left;
          } else {
            biggest = right;
          }
        } else if (compare(left, data.size()) && !compare(right, data.size())) {
          biggest = left;
        } else {
          break;
        }

        if (!compare(data[current], data[biggest])) {
          break;
        }
        std::swap(data[current], data[biggest]);
        current = biggest;
      }
    }
  }

public:
  // Создает пустую очередь с приоритетом
  PriorityQueue() = default;

  // Создает новую очередь с приоритетом, являющююся глубокой копией очереди
  // other [O(n)]
  PriorityQueue(const PriorityQueue &other) = default;

  PriorityQueue(const std::vector<T> vec) : data(vec) { heapify(); }

  // Перезаписывает текущую очередь с приоритетом очередью other
  PriorityQueue &operator=(const PriorityQueue &other) = default;

  // Создает новую очередь с приоритетом перемещая существующую [O(1)]
  PriorityQueue(PriorityQueue &&other) = default;

  // Присваивает текущую очередь с приоритетом очередью other
  PriorityQueue &operator=(PriorityQueue &&other) = default;

  // Очищает память очереди с приоритетом
  ~PriorityQueue() = default;

  // Получает ссылку на верхний элемент очереди с приоритетом
  const_reference top() const { return data.front(); }

  // Проверяет является ли контейнер пустым
  bool empty() const { return data.empty(); }

  // Возвращает размер очереди с приоритетом (сколько памяти уже занято)
  size_type size() const { return data.size(); }

  // Добавляет элемент в очередь с приоритетом
  void push(const value_type &value) {
    data.push_back(value);
    balanc();
  }

  // Добавляет элемент в очередь с приоритетом
  void push(const value_type &&value) {
    data.push_back(std::move(value));
    balanc();
  }

  template <class... Args> void emplace(Args &&...value) {
    data.emplace_back(std::forward<Args>(value)...);
    balanc();
  }

  // Удаляет элемент из начала очереди с приоритетом. Возвращает удаленный
  // элемент.
  T pop() {
    std::size_t current = 0;
    int left = 2 * current + 1;
    int right = 2 * current + 2;
    std::size_t last = data[0];
    std::size_t biggest;
    std::swap(data[0], data[data.size() - 1]);
    data.pop_back();
    while (true) {

      left = 2 * current + 1;
      right = 2 * current + 2;

      if (compare(left, data.size()) && compare(right, data.size())) {
        if (data[left] > data[right]) {
          biggest = left;
        } else {
          biggest = right;
        }
      } else if (compare(left, data.size()) && !compare(right, data.size())) {
        biggest = left;
      } else {
        break;
      }

      if (!compare(data[current], data[biggest])) {
        break;
      }
      std::swap(data[current], data[biggest]);
      current = biggest;
    }
    return last;
  }

  // Меняет содержимое с другой очередью с приоритетом. q1.swap(q2);
  void swap(PriorityQueue &other) { std::swap(data, other.data); }
};

#endif