#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <stdexcept>
#include <utility>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
private:
    std::list<std::pair<const KeyType, ValueType>> list;
    std::vector<std::list<typename std::list< std::pair<const KeyType,
    ValueType>>::iterator>> hash_table;
    size_t hash_table_capacity;
    size_t size_;
    Hash hash;
    
public:
    using iterator = typename
    std::list<std::pair<const KeyType, ValueType>>::iterator;
    using const_iterator = typename
    std::list<std::pair<const KeyType, ValueType>>::const_iterator;

    void clear() {
        list.clear();
        hash_table.clear();
        size_ = 0;
        hash_table_capacity = 42;
        hash_table.resize(42);
    }

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return (size() == 0);
    }

    size_t mod_hash(const KeyType& key) const {
        return hash(key) % hash_table_capacity;
    }

    void make_large(const size_t& capacity) {
        std::list<std::pair<const KeyType, ValueType>> pmet;
        for (const auto& cur : list) {
            pmet.insert(pmet.end(), cur);
        }
        list.clear();
        hash_table.clear();
        hash_table.resize(capacity);
        for (const auto& cur : pmet) {
            insert(cur);
        }
    }

    void insert(const std::pair<const KeyType, ValueType>& pair) {
        if (size() >= hash_table_capacity) {
            hash_table_capacity *= 2;
            size_ = 0;
            make_large(hash_table_capacity);
        }
        if (find(pair.first) == list.end()) {
            list.push_back(pair);
            hash_table[mod_hash(pair.first)].push_back(--list.end());
            size_++;
        }
    }

    iterator find(const KeyType& key) {
        for (auto begin = hash_table[mod_hash(key)].begin();
            begin != hash_table[mod_hash(key)].end();
            ++begin) {
            if ((*begin)->first == key)
                return *begin;
        }
        return list.end();
    }

    const_iterator find(const KeyType& key) const {
        for (auto begin = hash_table[mod_hash(key)].begin();
             begin != hash_table[mod_hash(key)].end();
             ++begin) {
            if ((*begin)->first == key)
                return *begin;
        }
        return list.cend();
    }

    HashMap(Hash hasher = Hash()): hash(hasher), size_(0), hash_table_capacity(42) {
        hash_table.resize(42);
    }

    template<class iter>
    HashMap(iter begin, iter end, Hash hasher = Hash()): hash(hasher), hash_table_capacity(42) {
        size_ = 0;
        hash_table.resize(42);
        for (; begin != end; ++begin) {
            insert(*begin);
        }
    }

    HashMap(const std::initializer_list<std::pair<const KeyType,
            ValueType>>& initializer_list_, Hash hasher = Hash()):
            hash(hasher), hash_table_capacity(42) {
        size_ = 0;
        hash_table.resize(42);
        for (auto begin = initializer_list_.begin();
             begin != initializer_list_.end();
             ++begin) {
            insert(*begin);
        }
    }

    HashMap& operator= (const HashMap& second) {
        auto for_copy = second.list;
        list.clear();
        hash_table.clear();
        make_large(second.hash_table_capacity);
        for (const auto& cur : for_copy) {
            insert(cur);
        }
        return *this;
    }

    void erase(const KeyType& key) {
        for (auto begin = hash_table[mod_hash(key)].begin();
             begin != hash_table[mod_hash(key)].end();
             ++begin) {
            if ((*begin)->first == key) {
                size_--;
                hash_table[mod_hash(key)].erase(begin);
                list.erase(*begin);
                return;
            }
        }
    }

    ValueType& operator[] (const KeyType& key) {
        for (const auto& cur : hash_table[mod_hash(key)]) {
            if (cur->first == key) {
                return cur->second;
            }
        }
        insert({key, ValueType()});
        return (--list.end())->second;
    }

    const ValueType& at(KeyType key) const {
        for (const auto& cur : hash_table[mod_hash(key)]) {
            if (cur->first == key) {
                return cur->second;
            }
        }
        throw std::out_of_range("");
    }

    Hash hash_function() const {
        return hash;
    }

    iterator begin() {
        return list.begin();
    }

    const_iterator begin() const {
        return list.cbegin();
    }

    iterator end() {
        return list.end();
    }

    const_iterator end() const {
        return list.cend();
    }
};
