#include <iostream>
#include <vector>

template <class T>
class Stack {
  std::vector<T> data;
public:
  class Iterator;
  // Создает пустой стэк
  Stack();

  // Создает новый стэк, являющийся глубокой копией списка other [O(n)]
  Stack(const Stack& other);

  // Перезаписывает текущий стэк стэком other
  Stack& operator=(const Stack& other);
  
  // Очищает память стэка
  ~Stack();

  // Возвращает размер стэка (сколько памяти уже занято)
  std::size_t size();

  // Проверяет является ли контейнер пустым
  bool empty();

  // Добавляет элемент в конец стэка.
  void push_back(const T& x);

  // Получает элемент на верху стэка
  T& top();

  // Удаляет последний элемент стэка. Возвращает удаленный элемент.
  T pop();

  // Меняет содержимое с другим стэком. s1.swap(s2);
  void swap(Stack& other);
};
