#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_CORE_GEOMETRY_H_
#define PBRT_SRC_CORE_GEOMETRY_H_

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
        DCHECK(!v.HasNaNs());
        x = v.x;
        y = v.y;
    }
    bool HasNaNs() const { return isNaN(x) || isNaN(y);}
    Vector2<T> &operator=(const Vector2<T> &v) {
        DCHECK(!v.HasNaNs());
        x = v.x;
        y = v.y;
        return *this;
    }
    Vector2<T> operator+(const Vector2<T> &v) const {
        DCHECK(!v.HasNaNs());
        return Vector2<T>(x + v.x, y + v.y);
    }
    Vector2<T> &operator+=(const Vector2<T> &v) {
        DCHECK(!v.HasNaNs());
        x += v.x;
        y += v.y;
        return *this;
    }
    Vector2<T> operator-(const Vector2<T> &v) const {
        DCHECK(!v.HasNaNs());
        return Vector2<T>(x - v.x, y - v.y);
    }
    Vector2<T> &operator-=(const Vector2<T> &v) {
        DCHECK(!v.HasNaNs());
        x -= v.x;
        y -= v.y;
        return *this;
    }
    bool operator==(const Vector2<T> &v) const { return x == v.x && y == v.y; }
    bool operator!=(const Vector2<T> &v) const { return x != v.x || y != v.y; }
    template <typename U>
    Vector2<T> operator*(U u) const {
        DCHECK(!isNaN(u));
        return Vector2<T>(x * u, y * u);
    }
    template <typename U>
    Vector2<T> &operator*=(U u) {
        DCHECK(!isNaN(u));
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

template <typename T>
class Vector3 {
public:
    Vector3(): x(0), y(0), z(0){}
    Vector3(T x, T y, T z): x(x), y(y), z(z){}
    Vector3(const Vector3<T> &v) {
        DCHECK(!v.HasNaNs());
        x = v.x;
        y = v.y;
        z = v.z;
    }
    bool HasNaNs() const { return isNaN(x) || isNaN(y) || isNaN(z); }
    Vector3<T> &operator=(const Vector3<T> &v) {
        DCHECK(!v.HasNaNs());
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }
    Vector3<T> operator+(const Vector3<T> &v) const {
        DCHECK(!v.HasNaNs());
        return Vector3<T>(x + v.x, y + v.y, z + v.z);
    }
    Vector3<T> &operator+=(const Vector3<T> &v) {
        DCHECK(!v.HasNaNs());
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Vector3<T> operator-(const Vector3<T> &v) const {
        DCHECK(!v.HasNaNs());
        return Vector3<T>(x - v.x, y - v.y, z - v.z);
    }
    Vector3<T> &operator-=(const Vector3<T> &v) {
        DCHECK(!v.HasNaNs());
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    bool operator==(const Vector3<T> &v) const { return x == v.x && y == v.y && z == v.z; }
    bool operator!=(const Vector3<T> &v) const { return x != v.x || y != v.y || z != v.z; }
    template <typename U>
    Vector3<T> operator*(U u) const {
        DCHECK(!isNaN(u));
        return Vector3<T>(x * u, y * u, z * u);
    }
    template <typename U>
    Vector3<T> &operator*=(U u) {
        DCHECK(!isNaN(u));
        x *= u;
        y *= u;
        z *= u;
        return *this;
    }
    template <typename U>
    Vector3<T> operator/(U u) const {
        DCHECK_NE(u, 0);
        Float inv = (Float)1 / u;
        return Vector3<T>(x * inv, y * inv, z * inv);
    }
    template <typename U>
    Vector3<T> &operator/=(U u) {
        DCHECK_NE(u, 0);
        Float inv = (Float)1 / u;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }
    Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }
    T operator[](int i) const {
        DCHECK(i >= 0 && i <= 2);
        if (i == 0) return x;
        else if (i == 1) return y;
        return z;
    }
    T &operator[](int i) {
        DCHECK(i >= 0 && i <= 2);
        if (i == 0) return x;
        else if (i == 1) return y;
        return z;
    }
    Float LengthSquared() const { return x * x + y * y + z * z; }
    Float Length() const { return std::sqrt(LengthSquared()); }

    T x, y, z;
};

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Vector3<T> &v) {
    os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const Vector3<Float> &v) {
    os << StringPrintf("[%f, %f, %f]", v.x, v.y, v.z);
    return os;
}

typedef Vector2<int> Vector2i;
typedef Vector2<Float> Vector2f;
typedef Vector3<int> Vector3i;
typedef Vector3<Float> Vector3f;

} // namespace pbrt

#endif // PBRT_SRC_CORE_GEOMETRIC_H_