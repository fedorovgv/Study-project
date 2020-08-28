/*

My implementation of optional (en.cppreference.com/w/cpp/utility/optional).

*/

#include <iostream>

struct BadOptionalAccess {
};

template <typename T>
class Optional {
private:
    alignas(T) unsigned char data[sizeof(T)];
    bool defined = false;

public:
    Optional() = default;
    Optional(const T& elem) {
        new (data) T(elem);
        defined = true;
    }
    Optional(T && elem) {
        new (data) T(std::move(elem));
        defined = true;
    }
    Optional(const Optional& other) {
        if (other.defined) {
            new (data) T(*other);
            defined = true;
        }
    }
    Optional& operator=(const Optional& other) {
        if (defined) {
            if (other.defined) {
                *reinterpret_cast<T*>(data) = *reinterpret_cast<const T*>(other.data);
            } else {
                reinterpret_cast<T*>(data)->~T();
                defined = false;
            }
        } else {
            if (other.defined) {
                new (data) T(*other);
                defined = true;
            }
        }
        return *this;
    }
    Optional& operator=(const T& elem) {
        if (defined) {
            *reinterpret_cast<T*>(data) = elem;
        } else {
            new (data) T(elem);
            defined = true;
        }
        return *this;
    }
    Optional& operator=(T&& elem) {
        if (defined) {
            *reinterpret_cast<T*>(data) = std::move(elem);
        } else {
            new (data) T(std::move(elem));
            defined = true;
        }
        return *this;
    }
    bool has_value() const {
        return defined;
    }
    T& operator*() {
        return *reinterpret_cast<T*>(data);
    }
    const T& operator*() const {
        return *reinterpret_cast<const T*>(data);
    }
    T* operator->() {
        return reinterpret_cast<T*>(data);
    }
    const T* operator->() const {
        return reinterpret_cast<const T*>(data);
    }
    T& value() {
        if (defined) {
            return **this;
        } else {
            throw BadOptionalAccess();
        }
    }
    const T& value() const {
        if (defined) {
            return **this;
        } else {
            throw BadOptionalAccess();
        }
    }
   void reset() {
       if (defined) {
          reinterpret_cast<T*>(data)->~T();
       }
       defined = false;
   }

  ~Optional() {
      if (defined) {
          reinterpret_cast<T*>(data)->~T();
      }
  }
};
