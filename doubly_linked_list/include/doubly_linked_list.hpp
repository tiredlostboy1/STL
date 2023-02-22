#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H
#include <iostream>

template <class T> 
class List {

  struct Node {
    Node *prev;
    T value;
    Node *next;
  };

  std::size_t size_{};
  Node *head_ = nullptr;
  Node *tail_ = nullptr;

public:
  class Iterator;
  class ConstIterator;

  List(std::size_t count = 0);

  List(const List &other);
  List(List &&other);

  List &operator=(const List &other);
  List &operator=(List &&other);

  ~List();

  std::size_t size();
  bool empty();

  ConstIterator begin() const;

  ConstIterator end() const;

  Iterator begin();

  Iterator end();

  T operator[](std::size_t index) const;
  T &operator[](std::size_t index);

  void push_back(const T &x);
  void push_front(const T &x);
  T pop_back();
  T pop_front();

  void insert(Iterator it, T value);
  T erase(Iterator it);

  class Iterator {
      friend List;
      Node* ptr;

  public:
      using iterator_category = std::bidirectional_iterator_tag;
      using value_type = T;
      using difference_type = std::ptrdiff_t;
      using pointer = value_type*;
      using reference = value_type&;

      Iterator(Node* p) : ptr{ p } {}

      // Инкремент. Движение к следующему элементу. ++it
      Iterator& operator++() {
          if (ptr == nullptr) {
              throw std::runtime_error{ "" };
          }
          ptr = ptr->next;
          return *this;
      }

      // Декремент. Движение к предыдущему элементу. --it
      Iterator& operator--() {
          if (ptr == nullptr) {
              throw std::runtime_error{ "" };
          }
          ptr = ptr->prev;
          return *this;
      }

      // Проверка на неравенство двух итераторов. Обычно используется it !=
      // container.end().
      bool operator!=(const Iterator& other) { return ptr != other.ptr; }

      bool operator==(const Iterator& other) { return ptr == other.ptr; }

      // разыменование (как с указателями): *it = 42; или std::cout << *it;
      T& operator*() { return ptr->value; }
      Iterator end();
  };

  class ConstIterator {
      Iterator it;

  public:
      using iterator_category = typename Iterator::iterator_category;
      using value_type = typename Iterator::value_type;
      using difference_type = typename Iterator::difference_type;
      using pointer = typename Iterator::pointer;
      using reference = typename Iterator::reference;

      ConstIterator(Node* p) : it{ p } {}

      ConstIterator& operator++() {
          ++it;
          return *this;
      }

      ConstIterator& operator--() {
          --it;
          return *this;
      }

      bool operator!=(const ConstIterator& other) { return it != other.it; }

      bool operator==(const ConstIterator& other) { return it == other.it; }

      // единственное отличие от Iterator, что возвращает константную ссылку
      const T& operator*() { return *it; }
  };
};

#endif