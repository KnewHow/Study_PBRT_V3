#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_CORE_GEOMETRIC_H_
#define PBRT_SRC_CORE_GEOMETRIC_H_

#include "pbrt.h"


#include "stringprint.h"

namespace pbrt {

template <typename T>
inline bool isNaN(const T t) {
    return std::isnan(t);
}

inline bool isNaN(int t) {
    return false;
}

template<typename T>
class Vector2 {
public:
    Vector2():x(0),y(0){}
    Vector2(T x, T y): x(x), y(y){}
    Vector2(const Vector2<T> &v) {
        CHECK(!v.HasNaNs());
        x = v.x;
        y = v.y;
    }
    bool HasNaNs() const { return isNaN(x) || isNaN(y);}
    Vector2<T> &operator=(const Vector2<T> &v) {
        CHECK(!v.HasNaNs());
        x = v.x;
        y = v.y;
        return *this;
    }
    Vector2<T> operator+(const Vector2<T> &v) const {
        CHECK(!v.HasNaNs());
        return Vector2<T>(x + v.x, y + v.y);
    }
    Vector2<T> &operator+=(const Vector2<T> &v) {
        CHECK(!v.HasNaNs());
        x += v.x;
        y += v.y;
        return *this;
    }
    Vector2<T> operator-(const Vector2<T> &v) const {
        CHECK(!v.HasNaNs());
        return Vector2<T>(x - v.x, y - v.y);
    }
    Vector2<T> &operator-=(const Vector2<T> &v) {
        CHECK(!v.HasNaNs());
        x -= v.x;
        y -= v.y;
        return *this;
    }
    bool operator==(const Vector2<T> &v) const { return x == v.x && y == v.y; }
    bool operator!=(const Vector2<T> &v) const { return x != v.x || y != v.y; }
    template <typename U>
    Vector2<T> operator*(U u) const {
        return Vector2<T>(x * u, y * u);
    }
    template <typename U>
    Vector2<T> &operator*=(U u) {
        x *= u;
        y *= u;
        return *this;
    }
    template <typename U>
    Vector2<T> operator/(U u) const {
        CHECK_NE(u, 0);
        Float inv = (Float)1 / u;
        return Vector2<T>(x * inv, y * inv);
    }
    template <typename U>
    Vector2<T> &operator/=(U u) {
        CHECK_NE(u, 0);
        Float inv = (Float)1 / u;
        x *= inv;
        y *= inv;
        return *this;
    }
    Vector2<T> operator-() const { return Vector2<T>(-x, -y); }
    T operator[](int i) const {
        DCHECK(i >= 0 && i <= 1);
        if(i == 0)
            return x;
        return y;
    }
    T &operator[](int i) {
        DCHECK(i >= 0 && i <= 1);
        if(i == 0)
            return x;
        return y;
    }
    Float LengthSquared() const { return x * x + y * y; }
    Float Length() const { return std::sqrt(LengthSquared()); }

    T x, y;
};

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Vector2<T> &v) {
    os << "[" << v.x << ", " << v.y <<"]";
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const Vector2<Float> &v) {
    os << StringPrintf("[%f, %f]", v.x, v.y);
    return os;
}

typedef Vector2<int> Vector2i;
typedef Vector2<Float> Vector2f;

} // namespace pbrt

#endif // PBRT_SRC_CORE_GEOMETRIC_H_