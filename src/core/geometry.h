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
    explicit Vector2(const Point2<T> &p);
    explicit Vector2(const Point3<T> &p);
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

template <typename T>
class Point2 {
public:
    Point2(): x(0), y(0){}
    Point2(T x, T y): x(x), y(y){}
    explicit Point2(const Point3<T>& p): x(p.x), y(p.y) { DCHECK(!HasNaNs()); }
    Point2(const Point2<T> &p) {
        DCHECK(!p.HasNaNs());
        x = p.x;
        y = p.y;
    }
    bool HasNaNs() const { return isNaN(x) || isNaN(y); }
    template <typename U>
    explicit Point2(const Point2<U> &p) {
        x = (T)p.x;
        y = (T)p.y;
        DCHECK(!HasNaNs());
    }
    template <typename U>
    explicit Point2(const Vector2<U> &v) {
        x = (T)v.x;
        y = (T)v.y;
        DCHECK(!HasNaNs());
    }
    template <typename U>
    explicit operator Vector2<U>() const {
        return Vector2<U>(x, y);
    }
    Point2<T> &operator=(const Point2<T> &p) {
        DCHECK(!p.HasNaNs());
        x = p.x;
        y = p.y;
        return *this;
    }
    Point2<T> operator+(const Point2<T> &p) const {
        DCHECK(!p.HasNaNs());
        return Point2<T>(x + p.x, y + p.y);
    }
    Point2<T> operator+(const Vector2<T> &v) const {
        DCHECK(!v.HasNaNs());
        return Point2<T>(x + v.x, y + v.y);
    }
    Point2<T> &operator+=(const Point2<T> &p) {
        DCHECK(!p.HasNaNs());
        x += p.x;
        y += p.y;
        return *this;
    }
    Point2<T> &operator+=(const Vector2<T> &v) {
        DCHECK(!v.HasNaNs());
        x += v.x;
        y += v.y;
        return *this;
    }
    Vector2<T> operator-(const Point2<T> &p) const {
        DCHECK(!p.HasNaNs());
        return Vector2<T>(x - p.x, y - p.y);
    }
    Point2<T> operator-(const Vector2<T> &v) const {
        DCHECK(!v.HasNaNs());
        return Point2<T>(x - v.x, y - v.y);
    }
    Point2<T> &operator-=(const Vector2<T> &v) {
        DCHECK(!v.HasNaNs());
        x -= v.x;
        y -= v.y;
        return *this;
    }
    Point2<T> operator-() const { return Point2<T>(-x, -y); }
    template <typename U>
    Point2<T> operator*(U u) const {
        DCHECK(!isNaN(u));
        return Point2<T>(x * u, y * u);
    }
    template <typename U>
    Point2<T> operator*=(U u) {
        DCHECK(!isNaN(u));
        x *= u;
        y *= u;
        return *this;
    }
    template <typename U>
    Point2<T> operator/(U u) const {
        DCHECK(!isNaN(u));
        Float inv = (Float)1 / u;
        return Point2<T>(x * inv, y * inv);
    }
    template <typename U>
    Point2<T> operator/=(U u) {
        DCHECK(!isNaN(u));
        Float inv = (Float)1 / u;
        x *= inv;
        y *= inv;
        return *this;
    }
    T operator[](int i) const {
        DCHECK(i >= 0 && i <= 1);
        if(i == 0) return x;
        return y;
    }
    T &operator[](int i) {
        DCHECK(i >= 0 && i <= 1);
        if(i == 0) return x;
        return y;
    }
    bool operator==(const Point2<T> &p) const { return x == p.x && y == p.y; }
    bool operator!=(const Point2<T> &p) const { return x != p.x || y != p.y; }

    T x, y;
};

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Point2<T> &p) {
    os << "[" << p.x << ", " << p.y << "]";
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const Point2<Float> &p) {
    os << StringPrintf("[%f, %f]", p.x, p.y);
    return os;
}

template <typename T>
class Point3 {
public:
    Point3(): x(0), y(0), z(0){}
    Point3(T x, T y, T z): x(x), y(y), z(z){ DCHECK(!HasNaNs()); }
    Point3(const Point3<T>& p): x(p.x), y(p.y), z(p.z) { DCHECK(!HasNaNs()); }
    template <typename U>
    explicit Point3(const Point3<U>& p) {
        x = (T)p.x;
        y = (T)p.y;
        z = (T)p.z;
        DCHECK(!HasNaNs());
    }
    bool HasNaNs() const { return isNaN(x) || isNaN(y) || isNaN(z); }
    template <typename U>
    explicit Point3(const Vector3<U> &v) {
        x = (T)v.x;
        y = (T)v.y;
        z = (T)v.z;
        DCHECK(!HasNaNs());
    }
    template <typename U>
    explicit operator Vector3<U>() const {
        return Vector3<U>(x, y, z);
    }
    Point3<T> &operator=(const Point3<T> &p) {
        x = p.x;
        y = p.y;
        z = p.z;
        return *this;
    }
    Point3<T> operator+(const Point3<T> &p) const {
        DCHECK(!p.HasNaNs());
        return Point3<T>(x + p.x, y + p.y, z + p.z);
    }
    Point3<T> operator+(const Vector3<T> &v) const {
        DCHECK(!v.HasNaNs());
        return Point3<T>(x + v.x, y + v.y, z + v.z);
    }
    Point3<T> &operator+=(const Point3<T> &p) {
        DCHECK(!p.HasNaNs());
        x += p.x;
        y += p.y;
        z += p.z;
        return *this;
    }
    Point3<T> &operator+=(const Vector3<T> &v) {
        DCHECK(!v.HasNaNs());
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Vector3<T> operator-(const Point3<T> &p) const {
        DCHECK(!p.HasNaNs());
        return Vector3<T>(x - p.x, y - p.y, z - p.z);
    }
    Point3<T> operator-(const Vector3<T> &v) const {
        DCHECK(!v.HasNaNs());
        return Point3<T>(x - v.x, y - v.y, z - v.z);
    }
    Point3<T> operator-=(const Vector3<T> &v) {
        DCHECK(!v.HasNaNs());
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    Point3<T> operator-() const { return Point3<T>(-x, -y, -z); }
    template <typename U>
    Point3<T> operator*(U u) const {
        DCHECK(!isNaN(u));
        return Point3<T>(x * u, y * u, z *u);
    }
    template <typename U>
    Point3<T> &operator*=(U u) {
        DCHECK(!isNaN(u));
        x *= u;
        y *= u;
        z *= u;
        return *this;
    }
    template <typename U>
    Point3<T> operator/(U u) const {
        DCHECK(!isNaN(u));
        Float inv = (Float)1 / u;
        return Point3<T>(x * inv, y * inv, z * inv);
    }
    template <typename U>
    Point3<T> &operator/=(U u) {
        DCHECK(!isNaN(u));
        Float inv = (Float)1 / u;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }
    T operator[](int i) const {
        DCHECK(i >= 0 && i <= 2);
        if(i == 0) return x;
        else if(i == 1) return y;
        else return z;
    }
    T &operator[](int i) {
        DCHECK(i >= 0 && i <= 2);
        if(i == 0) return x;
        else if(i == 1) return y;
        else return z;
    }
    bool operator==(const Point3<T> &p) const { return x == p.x && y == p.y && z == p.z; }
    bool operator!=(const Point3<T> &p) const { return x != p.x || y != p.z || z != p.z; }
    T x, y, z;
};

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Point3<T> &p) {
    os << "[" << p.x << ", " << p.y << ", " << p.z << "]";
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const Point3<Float> &p) {
    os << StringPrintf("[%f, %f, %f]", p.x, p.y, p.z);
    return os;
}

typedef Point2<int> Point2i;
typedef Point2<Float> Point2f;
typedef Point3<int> Point3i;
typedef Point3<Float> Point3f;

template <typename T>
class Normal3 {
public:
    Normal3(): x(0), y(0), z(0){}
    Normal3(T x, T y, T z): x(x), y(y), z(z){ DCHECK(!HasNaNs()); }
    Normal3(const Normal3<T> &n): x(n.x), y(n.y), z(n.z) { DCHECK(!HasNaNs()); }
    explicit Normal3(const Vector3<T> &v): x(v.x), y(v.y), z(v.z) { DCHECK(!HasNaNs()); }
    bool HasNaNs() const { return isNaN(x) || isNaN(y) || isNaN(z); }
    Normal3<T> &operator=(const Normal3<T> &n) {
        DCHECK(!n.HasNaNs());
        x = n.x;
        y = n.y;
        z = n.z;
        return *this;
    }
    Normal3<T> operator+(const Normal3<T> &n) const {
        DCHECK(!n.HasNaNs());
        return Normal3<T>(x + n.x, y + n.y, z + n.z);
    }
    Normal3<T> &operator+=(const Normal3<T> &n) {
        DCHECK(!n.HasNaNs());
        x += n.x;
        y += n.y;
        z += n.z;
        return *this;
    }
    Normal3<T> operator-(const Normal3<T> &n) const {
        DCHECK(!n.HasNaNs());
        return Normal3<T>(x - n.x, y - n.y, z - n.z);
    }
    Normal3<T> &operator-=(const Normal3<T> &n) {
        DCHECK(!n.HasNaNs());
        x -= n.x;
        y -= n.y;
        z -= n.z;
        return *this;
    }
    template <typename U>
    Normal3<T> operator*(U u) const {
        DCHECK(!isNaN(u));
        return Normal3<T>(x * u, y * u, z * u);
    }
    template <typename U>
    Normal3<T> &operator*=(U u) {
        DCHECK(!isNaN(u));
        x *= u;
        y *= u;
        z *= u;
        return *this;
    }
    template <typename U>
    Normal3<T> operator/(U u) const {
        DCHECK(!isNaN(u));
        Float inv = (Float)1 / u;
        return Normal3<T>(x * inv, y * inv, z * inv);
    }
    template <typename U>
    Normal3<T> &operator/=(U u) {
        DCHECK(!isNaN(u));
        Float inv = (Float)1 / u;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }
    Normal3<T> operator-() const { return Normal3<T>(-x, -y, -z); }
    T operator[](int i) const {
        DCHECK(i >= 0 && i <= 2);
        if(i == 0) return x;
        else if(i == 1) return y;
        else return z;
    }
    T &operator[](int i) {
        DCHECK(i >= 0 && i <= 2);
        if(i == 0) return x;
        else if(i == 1) return y;
        else return z;
    }
    bool operator==(const Normal3<T> &n) const { return x == n.x && y == n.y && z == n.z; }
    bool operator!=(const Normal3<T> &n) const { return x != n.x || y != n.y || z != n.z; }
    Float LengthSquared() const { return x * x + y * y + z * z; }
    Float Length() const { return std::sqrt(LengthSquared()); }
    T x, y, z;
};

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Normal3<T> &n) {
    os << "[" << n.x << ", " << n.y << ", " << n.z << "]";
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const Normal3<Float> &n) {
    os << StringPrintf("[%f, %f, %f]", n.x, n.y, n.z);
    return os;
}

typedef Normal3<Float> Normal3f;

template <typename T>
Vector2<T>::Vector2(const Point2<T> &p): x(p.x), y(p.y) { DCHECK(!HasNaNs()); }

template <typename T>
Vector2<T>::Vector2(const Point3<T> &p): x(p.x), y(p.y) { DCHECK(!HasNaNs()); }

} // namespace pbrt

#endif // PBRT_SRC_CORE_GEOMETRIC_H_