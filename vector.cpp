/* implementation of std::vector<T> */

#include <algorithm>
#include <memory>
#include <utility>
#include <iostream>
#include <cstddef>
#include <new>


template <typename T>
class Data {
public:
    T * data = nullptr;
    size_t cp = 0;
    static T * Allocate(size_t sz_) {
        return static_cast<T*>(operator new(sz_ * sizeof(T)));
    }
    static void Deallocate(T* arr) {
        operator delete(arr);
    }
    Data() = default;
    Data(size_t n) {
        data = Allocate(n);
        cp = n;
    }
    Data(const Data& other) = delete;
    Data(Data&& other) {
        Swap(data, other);
    }
    Data& operator= (const Data& other) = delete;
    Data& operator= (Data&& other) {
        Swap(other);
        return *this;
    }
    ~Data() {
        Deallocate(data);
    }
    T* operator+ (size_t it) {
        return data + it;
    }
    const T* operator+ (size_t it) const {
        return data + it;
    }
    T& operator[] (size_t index) {
        return data[index];
    }
    const T& operator[] (size_t index) const {
        return data[index];
    }
    void swap(Data& other) {
        std::swap(data, other.data);
        std::swap(cp, other.cp);
    }
};

template <typename T>
class Vector {
private:
    Data<T> data;
    size_t sz = 0;
    static void Construct(void * arr) {
        new (arr) T();
    }
    static void Construct(void * arr, const T& elem) {
        new (arr) T(elem);
    }
    static void Construct(void * arr, const T&& elem) {
        new (arr) T(std::move(elem));
    }
    static void Destroy(T * arr) {
        arr->~T();
    }

public:
    Vector() = default;
    Vector(size_t n): data(n) {
        std::uninitialized_value_construct_n(data.data, n);
        sz = n;
    }
    Vector(const Vector& other): data(other.sz) {
        std::uninitialized_copy_n(other.data.data, other.sz, data.data);
        sz = other.sz;
    }
    Vector(Vector&& other) {
        swap(other);
    }
    ~Vector() {
        std::destroy_n(data.data, sz);
    }
    Vector& operator= (const Vector& other) {
        if (other.sz > data.cp) {
            Vector tmp(other);
            swap(tmp);
        } else {
            for (size_t it = 0; it < sz && it < other.sz; ++it) {
                data[it] = other[it];
            }
            if (sz < other.sz) {
                std::uninitialized_copy_n(other.data.data, other.sz - sz, data.data + sz);
            } else if (other.sz < sz) {
                std::destroy_n(data.data + other.sz, sz - other.sz);
            }
        }
        sz = other.sz;
        return *this;
    }
    Vector& operator= (Vector&& other) {
        swap(other);
        return *this;
    }
    void swap(Vector& other) {
        data.swap(other.data);
        std::swap(sz, other.sz);
    }
    size_t size() const {
        return sz;
    }
    size_t capacity() const {
        return data.cp;
    }
    T& operator[] (size_t index) {
        return data[index];
    }
    const T& operator[] (size_t index) const {
        return data[index];
    }
    void reserve(size_t sz_) {
        if (sz_ > data.cp) {
            Data<T> new_data(sz_);
            std::uninitialized_move_n(data.data, sz, new_data.data);
            std::destroy_n(data.data, sz);
            data.swap(new_data);
        }
    }
    void resize(size_t n) {
        reserve(n);
        if (sz < n) {
            std::uninitialized_value_construct_n(data + sz, n - sz);
        } else if (sz > n) {
            std::destroy_n(data + n, sz - n);
        }
        sz = n;
    }
    void push_back(const T& elem) {
        if (sz == data.cp) {
            if (sz == 0) {
                reserve(1);
            } else {
                reserve(2 * sz);
            }
        }
        new (data + sz) T(elem);
        sz++;
    }
    void push_back(T&& elem) {
        if (sz == data.cp) {
            if (sz == 0) {
                reserve(1);
            } else {
                reserve(2 * sz);
            }
        }
        new (data + sz) T(std::move(elem));
        sz++;
    }
    void pop_back() {
        std::destroy_at(data + sz - 1);
        sz--;
    }
    void clear() {
        std::destroy_n(data.data, sz);
        sz = 0;
    }
    T* begin() const {
        return data.data;
    }
    T* end() const {
        return data.data + sz;
    }
};
