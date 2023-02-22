
#include "doubly_linked_list.hpp"

  // Создает список размера count заполненный дефолтными значениями типа T
template <class T>
List<T>::List(std::size_t count) {
    while (count) {
      push_back(T{});
      --count;
    }
  }

  // Создает новый список, являющийся глубокой копией списка other [O(n)]
  template <class T>
List<T>::List(const List &other) {
    for (const auto &x : other) {
      push_back(x);
    }
  }

template <class T>
List<T>::List(List &&other) {
    std::swap(size_, other.size_);
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
  }

  // Перезаписывает текущий список списком other
  // List l1{5}, l2{10};
  // l1 = l2;
  // std::cout << l1.size() == 10 << std::endl // True
  template <class T>
 inline List<T> &List<T>::operator=(const List<T> &other){
    List tmp{other}; // использование copy-конструктора
    std::swap(size_, tmp.size_);
    std::swap(head_, tmp.head_);
    std::swap(tail_, tmp.tail_);

    return *this;
    // tmp.~List() очистит то, что было в текущем списке раньше.
  }

template <class T>
List<T> &List<T>::operator=(List<T> &&other) {
    List tmp{std::move(other)}; // использование copy-конструктора
    std::swap(size_, tmp.size_);
    std::swap(head_, tmp.head_);
    std::swap(tail_, tmp.tail_);

    return *this;
  }

  // Очищает память списка [O(n)]
  template <class T>
List<T>::~List() {
    while (size_) {
      pop_front();
    }
  }

  // Возвращает размер списка (сколько памяти уже занято)
  template <class T>
  std::size_t List<T>::size() { return size_; }

  // Проверяет является ли контейнер пустым
  template <class T>
  bool List<T>::empty() { return size_; }

  // Возвращает итератор на первый элемент
template <class T>
typename List<T>::ConstIterator List<T>::begin() const { return ConstIterator{head_}; }

  // Возвращает итератор обозначающий конец контейнера
  template <class T>
typename List<T>::ConstIterator List<T>::end() const { return ConstIterator{nullptr}; }

template <class T>
typename List<T>::Iterator List<T>::begin() { return Iterator{head_}; }

  // Возвращает итератор обозначающий конец контейнера
  template <class T>
  typename List<T>::Iterator List<T>::end() { return Iterator{nullptr}; }

  // Возвращает копию элемента по индексу
  template <class T>
  T List<T>::operator[](std::size_t index) const {
    for (auto &x : *this) {
      if (index == 0) {
        return x;
      }
      --index;
    }
    throw std::out_of_range{""};
  }

  // Возвращает ссылку на элемент по индексу (позволяет менять элемент, типа
  // v[5] = 42;)
  template <class T>
  T &List<T>::operator[](size_t index){
    for (auto &x : *this) {
      if (index == 0) {
        return x;
      }
      --index;
    }
    throw std::out_of_range{""};
  }

  // Добавляет элемент в конец списока.
  template <class T>
  void List<T>::push_back(const T &x) {

    Node *tmp = new Node{nullptr, x, nullptr};
    if (size_ == 0) {
      head_ = tmp;
    } else {
      tmp->prev = tail_;
      tail_->next = tmp;
    }
    tail_ = tmp;
    ++size_;
  }

  // Добавляет элемент в начало списока.
  template <class T>
  void List<T>::push_front(const T &x) {

    Node *tmp = new Node{nullptr, x, nullptr};
    if (size_ == 0) {
      tail_ = tmp;
    } else {
      tmp->next = head_;
      head_->prev = tmp;
    }
    head_ = tmp;
    ++size_;
  }

  // Удаляет последний элемент списка.
  template <class T>
  T List<T>::pop_back() {
    if (size_ == 0)
      throw std::runtime_error("you can't pop");

    T poped_value = tail_->value;
    Node *point = tail_;

    if (size_ == 1) {
      head_ = nullptr;
      tail_ = nullptr;
    } else {
      tail_ = tail_->prev;
      tail_->next = nullptr;
    }
    delete point;
    --size_;
    return poped_value;
  }

  // Удаляет первый элемент списока.
  template <class T>
  T List<T>::pop_front() {
    if (size_ == 0)
      throw std::runtime_error("you can't pop");

    T poped_value = head_->value;
    Node *point = head_;

    if (size_ == 1) {
      head_ = nullptr;
      tail_ = nullptr;
    } else {
      head_ = head_->next;
      head_->prev = nullptr;
    }
    delete point;
    --size_;
    return poped_value;
  }

  // Вставляет новый элемент value перед элементом, на который указывает it.
  //     it
  //     v
  // [1, 2, 3].insert(it, 42) -> [1, 42, 2, 3]
  template <class T>
  void List<T>::insert(List<T>::Iterator it, T value) {
    if (it.ptr == head_) {
      push_front(value);
      return;
    } else if (it.ptr == nullptr) {
      push_back(value);
      return;
    }
    Node *tmp = new Node{it.ptr->prev, value, it.ptr};
    it.ptr->prev->next = tmp;
    it.ptr->prev = tmp;
    ++size_;
  }

  //     it
  //     v
  // [1, 2, 3].erase(1) -> [1, 3] (return 2)
  template <class T>
  T List<T>::erase(Iterator it) {

    if (it.ptr == head_) {
      return pop_front();
    } else if (it.ptr == tail_) {
      return pop_back();
    } else if (it.ptr == nullptr) {
      throw std::out_of_range("cant be erased");
    }

    T res = it.ptr->value;
    it.ptr->next->prev = it.ptr->prev;
    it.ptr->prev->next = it.ptr->next;

    delete it.ptr;
    --size_;
    return res;
  }
