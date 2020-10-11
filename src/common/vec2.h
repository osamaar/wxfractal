#pragma once

#include <cmath>

template <typename T>
class Vec2{
public:
    T x, y;

    Vec2() : x(0), y(0) { }
    Vec2(T x_, T y_) : x(x_), y(y_) { }
    Vec2(const Vec2<T> &other) : x(other.x), y(other.y) { }

    template <typename U>
    Vec2(const Vec2<U> &other) : x(other.x), y(other.y) { }

    template <typename U>
    Vec2<T> add(const Vec2<U> &other) {
        return Vec2(x + other.x, y + other.y);
    }

    template <typename U>
    Vec2<T> dot(const Vec2<U> &other);
    
    template <typename U>
    Vec2<T> mul(U factor) {
        return Vec2(factor*x, factor*y);
    }
    
    template <typename U>
    Vec2<T> div(U factor) {
        return Vec2(x/factor, y/factor);
    }
    
    T length() {
        return std::sqrt(x*x + y*y);
    }
    
    Vec2<T> unit() {
        return this->div(length());
    }
};


// template <typename T>
// Vec2<T> operator+(Vec2<T> a, Vec2<T> b) {
//     return a.add(b);
// }