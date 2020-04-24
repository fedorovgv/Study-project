/ * 

Функции для бинарной сериализации и десериализации примитивных типов,
строк, а также контейнеров std::vector и std::map, шаблонные параметры
которых - типы такого же вида.

Сначала записывается число типа size_t - размер контейнера,
а затем уже сами элементы. Для std::map записывается элементы
как пары, состоящие из ключа и значения. Функции Deserialize
восстанавливают исходный объект по сериализованному представлению.

*/


#include <map>
#include <iostream>
#include <string>
#include <vector>

template <typename T>
void Serialize(T pod, std::ostream& out) {
    out.write(reinterpret_cast<char*>(&pod), sizeof(T));
}
void Serialize(const std::string& str, std::ostream& out) {
    size_t size = str.length();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    for (size_t it = 0; it < size; ++it) {
        out.write(reinterpret_cast<const char*>(&str[it]), sizeof(char));
    }
}
template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out);
template <typename T>
void Serialize(const std::vector<T>& data, std::ostream& out) {
    size_t size = data.size();
    out.write(reinterpret_cast<char*>(&size), sizeof(size_t));
    for (auto now : data) {
        Serialize(now, out);
    }
}
template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out) {
    size_t size = data.size();
    out.write(reinterpret_cast<char*>(&size), sizeof(size_t));
    for (auto now : data) {
        Serialize(now.first, out);
        Serialize(now.second, out);
    }
}

template <typename T>
void Deserialize(std::istream& in, T& pod) {
    in.read(reinterpret_cast<char*>(&pod), sizeof(T));
}
void Deserialize(std::istream& in, std::string& str) {
    size_t size;
    in.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    str.resize(size);
    for (size_t it = 0; it < size; ++it) {
        in.read(reinterpret_cast<char*>(&str[it]), sizeof(char));
    }
}
template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data);
template <typename T>
void Deserialize(std::istream& in, std::vector<T>& data) {
    size_t size;
    in.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    data.resize(size);
    for (size_t it = 0; it < size; ++it) {
        Deserialize(in, data[it]);
    }
}
template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data) {
    size_t size;
    in.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    for (size_t it = 0; it < size; ++it) {
        T1 key;
        T2 val;
        Deserialize(in, key);
        Deserialize(in, val);
        data[key] = val;
    }
}
