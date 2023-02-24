#pragma once
#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include <cassert>
#include <cstddef>
#include <list>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

template <class Key, class Value> 
class UnorderedMap {
private:
  using ValueType = std::pair<Key, Value>;
  std::vector<std::list<ValueType>> data{};
  std::size_t s = 0;

  void set(const Key &key, Value &value) {
    std::size_t hashed = std::hash<Key>{}(key);
    data[hashed % data.size()].push_front({key, value});
    ++s;
  }

public:
  class Iterator;
  class ConstIterator;
  // Создает пустой словарь
  UnorderedMap() : data{8} {}

  // Создает новый UnorderedMap, являющийся глубокой копией other [O(n)]
  // UnorderedMap<std::string, int>  map;
  // map["something"] = 69;
  // map["anything"] = 199;
  // UnorderedMap<std::string, int> copied{map};
  // copied["something"] == map["something"] == 69
  UnorderedMap(const UnorderedMap &other) = default;

  // Конструктор перемещения
  UnorderedMap(UnorderedMap &&other) {
    std::swap(data, other.data);
    std::swap(s, other.s);
  }

  // Перезаписывает текущий словарь словарем other
  UnorderedMap &operator=(const UnorderedMap &other) {
    UnorderedMap tmp{other};
    std::swap(data, tmp.data);
    std::swap(s, tmp.s);
    return *this;
  }

  // Присваивание перемещением
  UnorderedMap &operator=(UnorderedMap &&other) {
    UnorderedMap tmp{std::move(other)};
    std::swap(data, tmp.data);
    std::swap(s, tmp.s);
    return *this;
  }

  ~UnorderedMap() = default;

  // Возвращает итератор на первый элемент
  Iterator begin() { return Iterator{data.begin(), --data.end()}; }

  // Возвращает константный итератор на первый элемент
  ConstIterator begin() const {
    return ConstIterator{Iterator{data.begin(), --data.end()}};
  }

  // Возвращает итератор обозначающий конец контейнера (за последним элементом)
  Iterator end() { return Iterator{data.end()-1}; }

  // Возвращает константный итератор, обозначающий конец контейнера
  ConstIterator end() const { return ConstIterator{Iterator{data.end() - 1}}; }

  // Возвращает размер UnorderedMap (сколько элементов добавлено)
  std::size_t size() const {
    return s;
  }

  // Проверяет является ли UnorderedMap пустым
  bool empty() const { return !s; }

  // Возвращает элемент по ключу. Если отсутсвует, выбрасывает исключение
  const Value &operator[](Key key) const {
    size_t hashed = std::hash<Key>{}(key);
    for (const auto& pair : data[hashed % data.size()]) {
      if (key == pair.first) {
        return pair.second;
      }
    }
    throw std::out_of_range{"No such a key here"};
  }

  // Возвращает ссылку на элемент по Key (позволяет менять элемент). Если
  // элемент с таким ключом отсутствует, создает его и инициализирует дефолтным
  // значением map["something"] = 75;
  Value &operator[](Key key) {
    size_t hashed = std::hash<Key>{}(key);
     for (auto& pair : data[hashed % data.size()]) {
      if (key == pair.first) {
        return pair.second;
      }
    }
    insert(key, Value{});
    return (*find(key)).second;
  }

  // Проверяет есть ли в контейнере элемент с таким Key
  bool contains(const Key &key) const {
    size_t idx = std::hash<Key>{}(key);
    for (const auto& it : data[idx % data.size()]) {
      if(it.first == key) {
        return true;
      }
    }
    return false;
  }
  // Возвращяет Итератор на элемент который ищем, если нет такого элемента
  // возвращает итератор на последный элемент
  Iterator find(const Key &key) {
   size_t idx = std::hash<Key>{}(key);
    for(auto it = data[idx % data.size()].begin(); it != data[idx % data.size()].end(); ++it) {
      if(it->first == key) {
        return Iterator(it);
      }
    }
    return end();
  }

  // Добавляет новый элемент с ключем и значением, если нет уже существуюшего
  // элемента с таким же ключом Map<int, std::string> c =
  //   {
  //       {1, "one" }, {2, "two" }, {3, "three"},
  //       {4, "four"}, {5, "five"}, {6, "six"  }
  //   };
  // c.(5,"something");
  // возвращает false, потому что элмемент с key = 5 уже существует․
  bool insert(const Key &k, const Value &v) {
    if (!contains(k)) {
      std::size_t hashed = std::hash<Key>{}(k);
      data[hashed % data.size()].push_back({k, v});
      ++s;
      return true;
    }
    return false;
  }

  // Удаляет элемент по ключу и возвращает результат операции
  // UnorderedMap<int, std::string> c =
  //   {
  //       {1, "one" }, {2, "two" }, {3, "three"},
  //       {4, "four"}, {5, "five"}, {6, "six"  }
  //   };
  // c.erase(4) == true
  // c =
  //   {
  //       {1, "one" }, {2, "two" }, {3, "three"},
  //             {5, "five"}, {6,"six"  }
  //   }; результат после erase
  bool erase(const Key &key) {
    size_t idx = std::hash<Key>{}(key);
    auto &collisions = data[idx % data.size()];
    for (auto it = collisions.begin(); it != collisions.end(); ++it) {
      if (key == it->first) {
        collisions.erase(it);
        --s;
        return true;
      }
    }
    return false;
  }

  class Iterator {
  private:
    typename std::vector<std::list<std::pair<Key, Value>>>::iterator it{};
    typename std::vector<std::list<std::pair<Key, Value>>>::iterator list_it_start{};
    typename std::vector<std::list<std::pair<Key, Value>>>::iterator list_it_end{};
    typename std::list<std::pair<Key, Value>>::iterator index{};

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::pair<Key, Value>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;

    Iterator(typename std::vector<std::list<std::pair<Key, Value>>>::iterator list_it_start,
              typename std::vector<std::list<std::pair<Key, Value>>>::iterator list_it_end)
        : it{list_it_start}, list_it_start{list_it_start},
          list_it_end{list_it_end}, index{(*list_it_start).begin()} {
      while (index == (*it).end()) {
        if (it == list_it_end) {
          return;
        }
        ++it;
        index = it.begin();
      }
    }

    Iterator(typename std::vector<std::list<std::pair<Key, Value>>>::iterator list_it_end)
        : it{list_it_end}, list_it_start{list_it_end}, list_it_end{list_it_end},
          index((*list_it_end).end()) {}

    Iterator(typename std::list<std::pair<Key, Value>>::iterator it) : index{it} {}

    Iterator &operator++() {
      ++index;
      while (index == (*it).end()) {
        if (it != list_it_end) {
          ++it;
          index = it.begin();
        }
      }
      return *this;
    }

    bool operator!=(const Iterator &other) { return index != other.index; }
    bool operator==(const Iterator &other) { return index == other.index; }

    // Разыменовывает указатель: std::cout << it->second;
    std::pair<Key, Value> &operator->() { return &(*index); }

    // Возвращает значение итератора: *it = {"travel", 42};
   std::pair<Key, Value> &operator*() { return *index; }
  };
  class ConstIterator {
  private:
    Iterator it;

  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::pair<Key, Value>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;

    ConstIterator(Iterator it) : it{it} {}

    ConstIterator &operator++() {
      ++it;
      return *this;
    }

    bool operator!=(const ConstIterator &other) { return it != other.it; }
    bool operator==(const ConstIterator &other) { return it == other.it; }

    const std::pair<Key, Value> &operator*() { return *it; }
  };
};

#endif