/* 

My implementation of shared_ptr

*/


#include <algorithm>
#include <memory>
#include <utility>
#include <iostream>

template <typename T>
class SharedPtr {
private:
  T * ptr;
  size_t * count;
  void increase()  {
    if (ptr != nullptr) {
      if (count == nullptr)
        count = new size_t(0);
      ++(*count);
    }
  }

  void decrease() noexcept {
    if (ptr == nullptr)
        return;
    if (*count == 1) {
        delete count;
        delete ptr;
        return;
    }
    --(*count);
  }

public:
  SharedPtr() : ptr(nullptr), count(nullptr) {}

  SharedPtr(T* ptr) : ptr(ptr), count(nullptr) {
    increase();
  }

  SharedPtr(const SharedPtr& other) : ptr(other.ptr), count(other.count) {
    increase();
  }
  SharedPtr(SharedPtr&& other) : ptr(other.ptr), count(other.count) {
    other.ptr = nullptr;
    other.count = nullptr;
  }

  SharedPtr& operator= (const SharedPtr& other) {
    if (ptr != other.ptr) {
      decrease();
      ptr = other.ptr;
      count = other.count;
      increase();
    }
    return *this;
  }

  SharedPtr& operator= (SharedPtr&& other) {
    if (ptr != other.ptr) {
      decrease();
      ptr = other.ptr;
      count = other.count;
      other.ptr = nullptr;
      other.count = nullptr;
    }
    return *this;
  }

  T& operator* () {
    return *ptr;
  }

  const T& operator* () const {
    return *ptr;
  }

  const T * operator -> () const {
    return ptr;
  }

  void reset(T * ptr_) {
    if (ptr != ptr_) {
      decrease();
      ptr = ptr_;
      count = nullptr;
      increase();
    }
  }

  void swap(SharedPtr& other) noexcept {
    std::swap(ptr, other.ptr);
    std::swap(count, other.count);
  }

  T * get() const {
    return ptr;
  }

  explicit operator bool() const noexcept {
    return ptr != nullptr;
  }

  ~SharedPtr() {
    decrease();
  }
};
