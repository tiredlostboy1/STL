#ifndef SMART_POINTERS_H
#define SMART_POINTERS_H
#pragma once

#include <cassert>
#include <iostream>

template <class T> struct CountData {
  T *data{nullptr};
  long *shared_counter{nullptr};
  long *weak_counter{nullptr};
};

template <class T> class SharedPtr {
private:
  CountData<T> *shared{};

public:
  // Создает пустой SharedPtr
  SharedPtr() : shared(nullptr) {}

  // Создает новый объект для конкретного указателя
  SharedPtr(T *ptr) : shared(new CountData<T>{ptr, new long{1}, new long{0}}) {}

  // Создает новый SharedPtr, который делит владение с other
  SharedPtr(const SharedPtr &other) : shared{other.shared} {
    ++(*(shared->shared_counter));
  }

  // Перезаписывает текущий умный указатель с other, при этом делит владение
  SharedPtr &operator=(const SharedPtr &other) {

    SharedPtr ptr{other};
    std::swap(shared, ptr.shared);
    return *this;
  }

  // Перезаписывает в текущий указатель указателем other(r-value)
  SharedPtr(SharedPtr &&other) {
    std::swap(shared, other.shared);
  }

  // Присваивает текущему указателю указатель other
  SharedPtr &operator=(SharedPtr &&other) {

    SharedPtr ptr{std::move(other)};
    std::swap(shared, ptr.shared);
    return *this;
  }
  // Очищает память умного указателя
  ~SharedPtr() {
    if (shared == nullptr) {
      return;
    }
    (*shared->shared_counter)--;
    if ((*shared->shared_counter) == 0) {
      delete shared->data;
      if ((*shared->weak_counter) == 0) {
        delete shared;
      }
    }
  }

  // Возвращает сырой указатель
  T *get() const { return shared->data; }

  //Результат разыменования указателя
  T &operator*() const { return *(shared->data); }

  // Чтобы можно было писать ptr->field
  T *operator->() const { return shared->data; }

  // Возвращает количество SharedPtr, с которыми делит память сохранённый
  // указатель (включая самого себя)
  long use_count() const {
    if (shared == nullptr) {
      return 0;
    }
    return *(shared->shared_counter);
  }

  // Проверяет, не равен ли сохраненный указатель нулю: if (ptr) или if(!ptr)
  operator bool() const { return shared != nullptr; }

  template <class U> friend class WeakPtr;
};

template <class T> class WeakPtr {
private:
  CountData<T> *weak = nullptr;

public:
  // Создает пустой WeakPtr
  WeakPtr() {}

  // Создает новый WeakPtr, который делит владение с other
  WeakPtr(const WeakPtr &other) : weak{other.weak} {
    ++(*(weak->weak_counter));
  }

  // Перезаписывает текущий WeakPtr с other
  WeakPtr &operator=(const WeakPtr &other) {
    WeakPtr ptr{other};
    std::swap(weak, ptr.weak);
    return *this;
  }

  // Перезаписывает текущий указатель указателем other(r-value)
  WeakPtr(WeakPtr &&other) { std::swap(weak, other.weak); }

  // Присваивает текущему указателю указатель  other
  WeakPtr &operator=(WeakPtr &&other) {
    WeakPtr tmp{std::move(other)};
    std::swap(weak, tmp.weak);
    return *this;
  }

  // Создает WeakPtr на SharedPtr (если other пуст, то текущий указатель тоже
  // должен быть пустым)
  WeakPtr(const SharedPtr<T> &other) {
    if (other.shared) {
      weak = other.shared;
      ++(*weak->weak_counter);
    } else {
      WeakPtr tmp{};
      std::swap(weak, tmp.weak);
    }
  }

  // Перезаписывает SharedPtr в WeakPtr (если other пуст, то текущий
  // указатель
  // тоже должен быть пустым)
  WeakPtr &operator=(const SharedPtr<T> &other) {
    if (other.shared) {
      weak = other.shared;
      ++(*weak->weak_counter);
    } else {
      WeakPtr tmp;
      std::swap(weak, tmp.weak);
    }
    return *this;
  }

  // Уничтожает WeakPtr.
  ~WeakPtr() {
    if (weak == nullptr) {
      return;
    }
    (*weak->weak_counter)--;
    if ((weak->shared_counter) == 0 && (weak->weak_counter) == 0) {
      delete weak;
    }
  }

  // Показывет количество SharedPtr, указывающих на этот объект.
  long use_count() const {
    if (weak == nullptr) {
      return 0;
    }
    return *(weak->shared_counter);
  }

  // Показывает, был ли удален управляемый объект .
  bool expired() const { return !use_count(); }

  // Создает новый SharedPtr, который разделяет права собственности на
  // управляемый объект. Если управляемого объекта нет, то возвращаемый
  // SharedPtr также пуст.
  // Важно: доступ к ресурсу осуществляется только через lock
  SharedPtr<T> lock() const {
    if (weak && *(weak->shared_counter) != 0) {
      if (weak->data) {
        SharedPtr<T> other;
        other.shared = weak;
        ++(*weak->shared_counter);
        return other;
      }
    }
    return SharedPtr<T>{};
  }
};

template <typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

#endif