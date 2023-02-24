#include <cassert>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

template <class Key, class Value> class Map {
private:
  using key_type = Key;
  using mapped_type = Value;
  using ValueType = std::pair<const Key, const Value>;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = ValueType &;
  using const_reference = const ValueType &;

  struct Node {
    Node *left{nullptr};
    Node *parent{nullptr};
    Node *right{nullptr};
    std::pair<Key, Value> data{};

    Node(Node *left, Node *parent, Node *right, ValueType data)
        : left{left}, parent{parent}, right{right}, data{data} {}
  };
  Node *root{nullptr};

  void push(const Key &key, const Value &value) {
    if (!root) {
      root = new Node{nullptr, nullptr, nullptr, {key, value}};
      // std::cout<<root<<std::endl;
      return;
    }
    Node *current = root;
    while (current) {
      if (current->data.first > key) {
        if (!current->left) {
          current->left = new Node{nullptr, current, nullptr, {key, value}};
          break;
        } else {
          current = current->left;
        }
      } else {
        if (!current->right) {
          current->right = new Node{nullptr, current, nullptr, {key, value}};
          break;
        } else {
          current = current->right;
        }
      }
    }
  }

public:
  class Iterator;
  class ConstIterator;
  // Создает пустой словарь
  Map() : root{nullptr} {};

  // Создает новый словарь, являющийся глубокой копией other [O(n)]
  //  Map<std::string, int> map;
  //  map["something"] = 69;
  //  map["anything"] = 199;
  //  Map<std::string, int> copied{map};
  //  copied["something"] == map["something"] == 69
  Map(const Map &other) { CopyTree(&root, other.root); }

  // Move конструктор
  Map(Map &&other) { std::swap(root, other.root); }

  // Перезаписывает текущий словарь словарем other
  Map &operator=(const Map &other) {
    Map tmp{other};
    std::swap(root, tmp.root);
    return *this;
  }

  // Присвоивание перемещением
  Map &operator=(Map &&other) {
    Map tmp{std::move(other)};
    std::swap(root, tmp.root);
    return *this;
  }

  // Очищает память словаря
  ~Map() { clear(); }

  // Возвращает итератор на первый элемент
  Iterator begin() {
    if (root == nullptr) {
      return Iterator{nullptr};
    }
    auto current = root;
    while (current && current->left) {
      current = current->left;
    }
    return Iterator{current};
  }

  // Возвращает const итератор на первый элемент
  ConstIterator begin() const {
    if (root == nullptr) {
      return ConstIterator{nullptr};
    }
    auto current = root;
    while (current && current->left) {
      current = current->left;
    }
    return ConstIterator{current};
  }

  // Возвращает итератор обозначающий конец контейнера
  Iterator end() { return Iterator{nullptr}; }

  // Возвращает const итератор обозначающий конец контейнера
  ConstIterator end() const { return ConstIterator{nullptr}; }

  // Возвращает размер словаря (сколько есть узлов)
  std::size_t size() const { return std::distance(begin(), end()); }

  void CopyTree(Node **current, Node *other) {
    // if (other->right) {
    //   current->right = new Node{nullptr, current, nullptr, other->data}; 
    //   CopyTree(current->right, other->right);
    // }
    // if (other->left) {
    //   current->left = new Node{nullptr, current, nullptr, other->data};
    //   CopyTree(current->left, other->left);
    // } 

    if(other) { *current = new Node{nullptr, nullptr, nullptr, other->data}; }
    if(other->right){
      // current->right = new Node{nullptr, current, nulltr, other->right->data}; 
      CopyTree(&((*current)->right), other->right);
    }
    if(other->left) { CopyTree(&((*current)->left), other->left); }
  }

  Iterator find(const Key &key) {
    auto current = root;
    while (current) {
      if (key == current->data.first) {
        return Iterator{current};
      } else if (key > current->data.first) {
        current = current->right;
      } else if (key < current->data.first) {
        current = current->left;
      } else {
        break;
      }
    }
    return end();
  }

  ConstIterator find(const Key &key) const {
    auto current = root;
    while (current) {
      if (key == current->data.first) {
        return ConstIterator{current};
      }
      if (key > current->data.first) {
        current = current->right;
      }
      if (key < current->data.first) {
        current = current->left;
      } else {
        break;
      }
    }
    return end();
  }

  // Проверяет есть ли элемент с таким ключом в контейнере
  bool contains(const Key &key) const { return find(key) != end(); }

  // Возвращает элемент по ключу. Если в словаре нет элемента с таким ключом, то
  // бросает исключение std::out_of_range
  const Value &operator[](const Key &key) const {
    if (find(key) != end()) {
      return (*find(key)).second;
    }
    throw std::out_of_range("no such key");
  }

  // Возвращает ссылку на элемент по ключу (позволяет менять элемент). Если в
  // словаре нет элемента с таким ключем, то создает его и устанавливает
  // дефолтное значение, после чего возвращает на него ссылку. map["something"]
  // = 75;
  Value &operator[](const Key &key) {
    if (find(key) == end()) {
      //   std::cout<<"push"<<std::endl;
      push(key, {});
    }
    // std::cout<<root<<std::endl;
    return (*find(key)).second;
  }

  // Удаляет элемент по ключу и возвращает значение удаленного элемента
  // Map<int, std::string> c =
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
    if (!root) {
      return false;
    }
    Node *current = root;
    while (current) {
      if (key == current->data.first) {
        // no child nodes
        if (!current->left && !current->right) {
          delete current;
          root = nullptr;
          break;
        }
        // has 1 left node
        if (current->left && !current->right) {
          // has parent
          if (current->parent) {
            if (current->data.first < current->parent->data.first) {
              current->parent->left = current->left;
              current->right->parent = current->parent;
              delete current;
              break;
            } else {
              current->parent->right = current->left;
              current->left->parent = current->parent;
              delete current;
              break;
            } // doesn't have parent
          } else {
            root = current->left;
            return true;
          }
        }// the same for right
        if (current->right && !current->left){
        if (current->parent) {
          if (current->data.first > current->parent->data.first) {
            current->parent->right = current->right;
            current->right->parent = current->parent;
            delete current;
            break;
          } else {
            current->parent->left = current->right;
            current->right->parent = current->parent;
            delete current;
            break;
          } // doesn't have parent
        } else {
          root = current->right;
          return true;
        } }// ends here

        // the case where its needed to find successor
        if (current->left && current->right) {
          Node *successor = current;
          successor = successor->right;
          while (successor->left) {
            successor = successor->left;
          }

          std::swap(current->data, successor->data);

          if (successor->right) {
            successor->parent->left = successor->right;
            successor->right->parent = successor->parent;
          }
        }

        delete current;
        return true;
      } else if (key > current->data.first) { // x on the right side
        current = root->right;
      } else { // x on the left side
        current = root->left;
      }
    }
    return false;
  }

  // Меняет текуший контейнер с контейнером other
  void swap(Map &other) { std::swap(root, other.root); }

  // Возвращает итератор на первый элемент который не меньше чем переданный
  // ключ. [O(h)]
  Iterator lower_bound(const Key &key) {
    if (!root) {
      return end();
    }
    auto current = root;

    while (current) {
      if (current->right && key > current->data.first) {
        current = root->right;
      } else if (current->left && key < current->data.first) {
        current = root->left;
      } else {
        break;
      }
    }
    return current;
  }

  void clear(Node *node) {
    if (node == nullptr) {
      return;
    }
    clear(node->left);
    clear(node->right);
    delete node;
  }
  // Очищает контейнер [O(n)]
  // Map<int, std::string> c =
  //   {
  //       {1, "one" }, {2, "two" }, {3, "three"},
  //       {4, "four"}, {5, "five"}, {6, "six"  }
  //   };
  // c.clear;
  // c.size() == 0 //true;
  void clear() {
    clear(root);
    root = nullptr;
  }

  class Iterator {
  private:
    Node *node;

  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Key;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;

    Iterator(Node *node) : node{node} {}
    // Инкремент. Движение к следующему элементу.
    Iterator &operator++() {

      // one to the right child then down to the left bottom
      if (node->right) {
        node = node->right;
        while (node && node->left) {
          node = node->left;
        }
      }
      // up to parent which was not iterated
      else {
        while (node->parent && node->data.first > node->parent->data.first) {
          node = node->parent;
        }
        node = node->parent;
      }
      return *this;
    }

    // Декремент. Движение к предыдущему элементу.
    Iterator &operator--() {
      // one to the left parent then to the right
      if (node->left) {
        node = node->left;
        while (node && node->right) {
          node = node->right;
        }
      } else {
        while (node->data.first < node->parent.data.first) {
          node = node->parent;
        }
        node = node->parent;
      }
      return *this;
    }

    bool operator==(const Iterator &other) { return node == other.node; }
    // Проверка на неравенство двух итераторов.
    bool operator!=(const Iterator &other) { return node != other.node; }

    // разыменование (как с указателями): *it = 42; или std::cout << *it;
    std::pair<Key, Value> &operator*() { return node->data; }
    const std::pair<Key, Value> *operator->() { return &(node->data); }
  };
  class ConstIterator {
  private:
    Node *node;

  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Key;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;

    ConstIterator(Node *node) : node{node} {}

    ConstIterator &operator++() {
     
      // one to the right child then down to the left bottom
      if (node->right) {
        node = node->right;
        while (node && node->left) {
          node = node->left;
        }
      }
      // up to parent which was not iterated
      else {
        while (node->parent && node->data.first > node->parent->data.first) {
          node = node->parent;
        }
        node = node->parent;
      }
      return *this;
    }

    ConstIterator &operator--() {
      // one to the left parent then to the right
      if (node->left) {
        node = node->left;
        while (node && node->right) {
          node = node->right;
        }
      } else {
        while (node->data.first < node->parent.data.first) {
          node = node->parent;
        }
        node = node->parent;
      }
      return *this;
    }

    bool operator!=(const ConstIterator &other) { return node != other.node; }

    bool operator==(const ConstIterator &other) { return node == other.node; }

    // единственное отличие от Iterator, что возвращает константную ссылку
    const std::pair<Key, Value> &operator*() { return node->data; }

    const std::pair<Key, Value> *operator->() { return &(node->data); }
  };
};
