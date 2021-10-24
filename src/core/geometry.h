#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_CORE_GEOMETRY_H_
#define PBRT_SRC_CORE_GEOMETRY_H_

#include <iterator>

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
    explicit Vector3<T>(const Point3<T> &p);
    explicit Vector3<T>(const Normal3<T> &n);
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
class Bounds2 {
public:
    Bounds2() {
        T minV = std::numeric_limits<T>::lowest();
        T maxV = std::numeric_limits<T>::max();
        pMin = Point2<T>(maxV, maxV);
        pMax = Point2<T>(minV, minV);
    }
    explicit Bounds2(const Point2<T> &p): pMin(p), pMax(p){}
    Bounds2(const Point2<T> &p0, const Point2<T> &p1) {
        pMin = Point2<T>(std::min(p0.x, p1.x), std::min(p0.y, p1.y));
        pMax = Point2<T>(std::max(p0.x, p1.x), std::max(p0.y, p1.y));
    }
    template <typename U>
    explicit operator Bounds2<U>() const {
        return Bounds2<U>((Point2<U>)pMin, (Point2<U>)pMax);
    }
    Vector2<T> Diagonal() const { return pMax - pMin; }
    T Area() const {
        Vector2<T> diag = Diagonal();
        return diag.x * diag.y;
    }
    int MaximumExtent() const {
        Vector2<T> diag = Diagonal();
        if(diag.x > diag.y) return 0;
        else return 1;
    }
    inline const Point2<T> &operator[](int i) const {
        DCHECK(i >= 0 && i <= 1);
        return i == 0 ? pMin : pMax;
    }
    inline Point2<T> &operator[](int i) {
        DCHECK(i >= 0 && i <= 1);
        return i == 0 ? pMin : pMax;
    }
    bool operator==(const Bounds2<T> &b) const { return pMin == b.pMin && pMax == b.pMax; }
    bool operator!=(const Bounds2<T> &b) const { return pMin != b.pMin || pMax != b.pMax; }
    Point2<T> Lerp(const Point2f &p) const {
        return Point2<T>(pbrt::Lerp(p.x, pMin.x, pMax.x), 
                         pbrt::Lerp(p.y, pMin.y, pMax.y));
    }
    Vector2<T> Offset(const Point2<T> &p) const {
        Vector2<T> r = p - pMin;
        if(pMax.x > pMin.y) r.x /= (pMax.x - pMin.x);
        if(pMax.y > pMin.y) r.y /= (pMax.y - pMin.y);
        return r;
    }
    void BoundingSphere(Point2<T> &center, Float &radius) const {
        center = (pMin + pMax) / 2 ;
        radius = Inside(center, *this) ? Distance(center, pMax) : 0;
    }
    friend inline std::ostream &operator<<(std::ostream& os, const Bounds2<T> &b) {
        os << "[" << b.pMin << "-" << b.pMax << "]";
        return os;
    }
    Point2<T> pMin, pMax;
};

template <typename T>
class Bounds3 {
public:
    Bounds3(){
        T min = std::numeric_limits<T>::lowest();
        T max = std::numeric_limits<T>::max();
        pMin = Point3<T>(max, max, max);
        pMax = Point3<T>(min, min, min);
    }
    Bounds3(const Point3<T> &p0, const Point3<T> &p1) {
        pMin = Point3<T>(std::min(p0.x, p1.x), std::min(p0.y, p1.y), std::min(p0.z, p1.z));
        pMax = Point3<T>(std::max(p0.x, p1.x), std::max(p0.y, p1.y), std::max(p0.z, p1.z));
    }
    explicit Bounds3(const Point3<T> &p): pMin(p), pMax(p){}
    template <typename U>
    explicit operator Bounds3<U>() const {
        return Bounds3<U>((Point3<U>)pMin, (Point3<U>)pMax);
    }
    Point3<T> Corner(int corner) const {
        DCHECK(corner >= 0 && corner < 8);
        return Point3<T>((*this)[(corner & 1)].x,
                         (*this)[(corner & 2) ? 1 : 0].y,
                         (*this)[(corner & 4) ? 1 : 0].z);
    }
    Vector3<T> Diagonal() const  { return pMax - pMin; }
    T SurfaceArea() const {
        Vector3<T> diag = Diagonal();
        return 2 * (diag.x * diag.y + diag.x * diag.z + diag.y * diag.z);
    }
    T Volume() const {
        Vector3<T> diag = Diagonal();
        return diag.x * diag.y * diag.z;
    }
    int MaximumExtent() const {
        Vector3<T> diag = Diagonal();
        if(diag.x > diag.y && diag.x > diag.z) 
            return 0;
        else if(diag.y > diag.z) 
            return 1;
        else 
            return 2;
    }
    const Point3<T> &operator[](int i) const {
        DCHECK(i >= 0 && i <= 1);
        return i == 0 ? pMin : pMax; 
    }
    Point3<T> &operator[](int i) {
        DCHECK(i >= 0 && i <= 1);
        return i == 0 ? pMin : pMax; 
    }
    bool operator==(const Bounds3<T> &b) const { return pMin == b.pMin && pMax == b.pMax; }
    bool operator!=(const Bounds3<T> &b) const { return pMin != b.pMin || pMax != b.pMax; }
    Point3<T> Lerp(const Point3<Float> &t) const {
        return Point3<T>(pbrt::Lerp(t.x, pMin.x, pMax.y),
                         pbrt::Lerp(t.y, pMin.y, pMax.y),
                         pbrt::Lerp(t.z, pMin.z, pMax.z));
    }
    Vector3<T> Offset(const Point3<T> &p) const {
        Vector3<T> r = p - pMin;
        if(pMax.x > pMin.x) r.x /= pMax.x - pMin.x;
        if(pMax.y > pMin.y) r.y /= pMax.y - pMin.y;
        if(pMax.z > pMin.z) r.z /= pMax.z - pMin.z;
        return r;
    } 
    void BoundingSphere(Point3<T> &center, Float &radius) const {
        center = (pMin + pMax) / 2;
        radius = Inside(center, *this) ? Distance(*center, pMax) : 0;
    }
    inline bool IntersectP(const Ray &ray, Float &hitt0, Float &hitt1) const;
    inline bool IntersectP(const Ray &ray, const Vector3f &invDir, const int dirIsNeg[3]) const;
    friend std::ostream &operator<<(std::ostream &os, const Bounds3<T> &b) {
        os << "[" << b.pMin << "-" << b.pMax << "]";
        return os;
    }
    Point3<T> pMin, pMax;
};

typedef Bounds2<int> Bounds2i;
typedef Bounds2<Float> Bounds2f;
typedef Bounds3<int> Bounds3i;
typedef Bounds3<Float> Bounds3f;

class Bounds2iIterator: public std::forward_iterator_tag { // iterator for bounds2i, use this, the range of point is pMin to pMax(don't contain)
public:
    Bounds2iIterator(const Bounds2i &b, const Point2i & p): b(b), p(p){}
    Bounds2iIterator operator++() {
        advance();
        return *this;
    }
    Bounds2iIterator operator++(int) {
        Bounds2iIterator old = *this;
        advance();
        return old;
    }
    bool operator==(const Bounds2iIterator &bi) const {
        return b == bi.b && p == bi.p;
    }
    bool operator!=(const Bounds2iIterator &bi) const {
        return b != bi.b || p != bi.p;
    }
    Point2i operator*() const { return p; }
private:
    void advance() {
        ++p.x;
        if(p.x == b.pMax.x) {
            p.x = b.pMin.x;
            ++p.y;
        }
    }
    Point2i p; // current interted point
    const Bounds2i &b; // the bounds need to iterate
};

inline Bounds2iIterator begin(const Bounds2i& b) {
    return Bounds2iIterator(b, b.pMin);
}

inline Bounds2iIterator end(const Bounds2i& b) {
    // Normally, the ending point is at the minimum x value and one past
    // the last valid y value.
    Point2i pEnd(b.pMin.x, b.pMax.y);
    // However, if the bounds are degenerate, override the end point to
    // equal the start point so that any attempt to iterate over the bounds
    // exits out immediately.
    if(b.pMin.x >= b.pMax.x || b.pMin.y >= b.pMax.y)
        pEnd = b.pMin;
    return Bounds2iIterator(b, pEnd);
}

class Ray {
public:
    Ray(): tMax(Infinity){}
    Ray(const Point3f &o, const Vector3f& d, Float tMax = Infinity):o(o), d(d), tMax(tMax){}
    Point3f operator()(Float t) const { return o + d * t; }
    friend std::ostream &operator<<(std::ostream &os, const Ray &ray) {
        os << "Ray: [o=" << ray.o << ", d=" << ray.d << ", tMax=" << ray.tMax << "]";
        return os;
    }
    bool HasNaNs() const { return o.HasNaNs() || d.HasNaNs() || isNaN(tMax); }
    Point3f o; // the origin of the ray
    Vector3f d; // the direction of the ray, it must be normalize
    mutable Float tMax;// a varible to tag the maximum distance the ray can run along with the d
};

template <typename T>
Vector2<T>::Vector2(const Point2<T> &p): x(p.x), y(p.y) { DCHECK(!HasNaNs()); }

template <typename T>
Vector2<T>::Vector2(const Point3<T> &p): x(p.x), y(p.y) { DCHECK(!HasNaNs()); }

template <typename T>
Vector3<T>::Vector3(const Point3<T> &p): x(p.x), y(p.y), z(p.z) { DCHECK(!HasNaNs()); }

template <typename T, typename U>
inline Vector3<T> operator*(U u, const Vector3<T> &v) {
    return v * u;
}

template <typename T>
Vector3<T> Abs(const Vector3<T> &v) {
    return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

template <typename T>
inline T Dot(const Vector3<T> &v0, const Vector3<T> &v1) {
    DCHECK(!v0.HasNaNs() && !v1.HasNaNs());
    return v0.x * v1.x + v0.y * v1.y + v0.z  * v1.z;
}

template <typename T>
inline T AbsDot(const Vector3<T> &v0, const Vector3<T> &v1) {
    DCHECK(!v0.HasNaNs() && !v1.HasNaNs());
    return std::abs(Dot(v0, v1));
}

template <typename T>
inline Vector3<T> Cross(const Vector3<T> & v0, const Vector3<T> &v1) {
    DCHECK(!v0.HasNaNs() && !v1.HasNaNs());
    double v0x = v0.x, v0y = v0.y, v0z = v0.z;
    double v1x = v1.x, v1y = v1.y, v1z = v1.z;
    return Vector3<T>((v0y * v1z) - (v0z * v1y), (v0z * v1x) - (v0x * v1z),
                      (v0x * v1y) - (v0y * v1x));
}

template <typename T>
inline Vector3<T> Cross(const Vector3<T> &v1, const Normal3<T> &v2) {
    DCHECK(!v1.HasNaNs() && !v2.HasNaNs());
    double v1x = v1.x, v1y = v1.y, v1z = v1.z;
    double v2x = v2.x, v2y = v2.y, v2z = v2.z;
    return Vector3<T>((v1y * v2z) - (v1z * v2y), (v1z * v2x) - (v1x * v2z),
                      (v1x * v2y) - (v1y * v2x));
}

template <typename T>
inline Vector3<T> Cross(const Normal3<T> &v1, const Vector3<T> &v2) {
    DCHECK(!v1.HasNaNs() && !v2.HasNaNs());
    double v1x = v1.x, v1y = v1.y, v1z = v1.z;
    double v2x = v2.x, v2y = v2.y, v2z = v2.z;
    return Vector3<T>((v1y * v2z) - (v1z * v2y), (v1z * v2x) - (v1x * v2z),
                      (v1x * v2y) - (v1y * v2x));
}

template <typename T>
inline Vector3<T> Normalize(const Vector3<T> &v) {
    return v / v.Length();
}

template <typename T>
T MinComponent(const Vector3<T> &v) {
    return std::min(v.x, std::min(v.y, v.z));
}

template <typename T>
T MaxComponent(const Vector3<T> &v) {
    return std::max(v.x, std::max(v.y, v.z));
}

template <typename T>
int MaxDimension(const Vector3<T> &v) {
    return (v.x > v.y) ? ((v.x > v.z) ? 0 : 2) : ((v.y > v.z) ? 1 : 2);
}

template <typename T>
Vector3<T> Min(const Vector3<T> &p1, const Vector3<T> &p2) {
    return Vector3<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
                      std::min(p1.z, p2.z));
}

template <typename T>
Vector3<T> Max(const Vector3<T> &p1, const Vector3<T> &p2) {
    return Vector3<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y),
                      std::max(p1.z, p2.z));
}

template <typename T>
Vector3<T> Permute(const Vector3<T> &v, int x, int y, int z) {
    return Vector3<T>(v[x], v[y], v[z]);
}

template <typename T>
inline void CoordinateSystem(const Vector3<T> &v1, Vector3<T> &v2,
                             Vector3<T> &v3) { // build a coordinate system use v1, 
    if (std::abs(v1.x) > std::abs(v1.y))
        v2 = Vector3<T>(-v1.z, 0, v1.x) / std::sqrt(v1.x * v1.x + v1.z * v1.z);
    else
        v2 = Vector3<T>(0, v1.z, -v1.y) / std::sqrt(v1.y * v1.y + v1.z * v1.z);
    v3 = Cross(v1, v2);
}

template <typename T, typename U>
inline Vector2<T> operator*(U f, const Vector2<T> &v) {
    return v * f;
}

template <typename T>
inline Float Dot(const Vector2<T> &v1, const Vector2<T> &v2) {
    DCHECK(!v1.HasNaNs() && !v2.HasNaNs());
    return v1.x * v2.x + v1.y * v2.y;
}

template <typename T>
inline Float AbsDot(const Vector2<T> &v1, const Vector2<T> &v2) {
    DCHECK(!v1.HasNaNs() && !v2.HasNaNs());
    return std::abs(Dot(v1, v2));
}

template <typename T>
inline Vector2<T> Normalize(const Vector2<T> &v) {
    return v / v.Length();
}
template <typename T>
Vector2<T> Abs(const Vector2<T> &v) {
    return Vector2<T>(std::abs(v.x), std::abs(v.y));
}


template <typename T>
inline Float Distance(const Point2<T> &p0, const Point2<T> &p1) {
    return (p0 - p1).Length();
}

template <typename T>
inline Float Distance(const Point3<T> &p0, const Point3<T> &p1) {
    return (p0 - p1).Length();
}

template <typename T, typename U>
inline Point3<T> operator*(U f, const Point3<T> &p) {
    DCHECK(!p.HasNaNs());
    return p * f;
}

template <typename T>
Point3<T> Lerp(Float t, const Point3<T> &p0, const Point3<T> &p1) {
    return (1 - t) * p0 + t * p1;
}

template <typename T>
Point3<T> Min(const Point3<T> &p1, const Point3<T> &p2) {
    return Point3<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
                     std::min(p1.z, p2.z));
}

template <typename T>
Point3<T> Max(const Point3<T> &p1, const Point3<T> &p2) {
    return Point3<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y),
                     std::max(p1.z, p2.z));
}

template <typename T>
Point3<T> Floor(const Point3<T> &p) {
    return Point3<T>(std::floor(p.x), std::floor(p.y), std::floor(p.z));
}

template <typename T>
Point3<T> Ceil(const Point3<T> &p) {
    return Point3<T>(std::ceil(p.x), std::ceil(p.y), std::ceil(p.z));
}

template <typename T>
Point3<T> Abs(const Point3<T> &p) {
    return Point3<T>(std::abs(p.x), std::abs(p.y), std::abs(p.z));
}

template <typename T>
inline Float DistanceSquared(const Point2<T> &p1, const Point2<T> &p2) {
    return (p1 - p2).LengthSquared();
}

template <typename T, typename U>
inline Point2<T> operator*(U f, const Point2<T> &p) {
    DCHECK(!p.HasNaNs());
    return p * f;
}

template <typename T>
Point2<T> Floor(const Point2<T> &p) {
    return Point2<T>(std::floor(p.x), std::floor(p.y));
}

template <typename T>
Point2<T> Ceil(const Point2<T> &p) {
    return Point2<T>(std::ceil(p.x), std::ceil(p.y));
}

template <typename T>
Point2<T> Lerp(Float t, const Point2<T> &v0, const Point2<T> &v1) {
    return (1 - t) * v0 + t * v1;
}

template <typename T>
Point2<T> Min(const Point2<T> &pa, const Point2<T> &pb) {
    return Point2<T>(std::min(pa.x, pb.x), std::min(pa.y, pb.y));
}

template <typename T>
Point2<T> Max(const Point2<T> &pa, const Point2<T> &pb) {
    return Point2<T>(std::max(pa.x, pb.x), std::max(pa.y, pb.y));
}

template <typename T>
Point3<T> Permute(const Point3<T> &p, int x, int y, int z) {
    return Point3<T>(p[x], p[y], p[z]);
}

template <typename T, typename U>
inline Normal3<T> operator*(U f, const Normal3<T> &n) {
    return Normal3<T>(f * n.x, f * n.y, f * n.z);
}

template <typename T>
inline Normal3<T> Normalize(const Normal3<T> &n) {
    return n / n.Length();
}

template <typename T>
inline Vector3<T>::Vector3(const Normal3<T> &n)
    : x(n.x), y(n.y), z(n.z) {
    DCHECK(!n.HasNaNs());
}

template <typename T>
inline T Dot(const Normal3<T> &n1, const Vector3<T> &v2) {
    DCHECK(!n1.HasNaNs() && !v2.HasNaNs());
    return n1.x * v2.x + n1.y * v2.y + n1.z * v2.z;
}

template <typename T>
inline T Dot(const Vector3<T> &v1, const Normal3<T> &n2) {
    DCHECK(!v1.HasNaNs() && !n2.HasNaNs());
    return v1.x * n2.x + v1.y * n2.y + v1.z * n2.z;
}

template <typename T>
inline T Dot(const Normal3<T> &n1, const Normal3<T> &n2) {
    DCHECK(!n1.HasNaNs() && !n2.HasNaNs());
    return n1.x * n2.x + n1.y * n2.y + n1.z * n2.z;
}

template <typename T>
inline T AbsDot(const Normal3<T> &n1, const Vector3<T> &v2) {
    DCHECK(!n1.HasNaNs() && !v2.HasNaNs());
    return std::abs(n1.x * v2.x + n1.y * v2.y + n1.z * v2.z);
}

template <typename T>
inline T AbsDot(const Vector3<T> &v1, const Normal3<T> &n2) {
    DCHECK(!v1.HasNaNs() && !n2.HasNaNs());
    return std::abs(v1.x * n2.x + v1.y * n2.y + v1.z * n2.z);
}

template <typename T>
inline T AbsDot(const Normal3<T> &n1, const Normal3<T> &n2) {
    DCHECK(!n1.HasNaNs() && !n2.HasNaNs());
    return std::abs(n1.x * n2.x + n1.y * n2.y + n1.z * n2.z);
}

template <typename T>
inline Normal3<T> Faceforward(const Normal3<T> &n, const Vector3<T> &v) {
    return (Dot(n, v) < 0.f) ? -n : n;
}

template <typename T>
inline Normal3<T> Faceforward(const Normal3<T> &n, const Normal3<T> &n2) {
    return (Dot(n, n2) < 0.f) ? -n : n;
}

template <typename T>
inline Vector3<T> Faceforward(const Vector3<T> &v, const Vector3<T> &v2) {
    return (Dot(v, v2) < 0.f) ? -v : v;
}

template <typename T>
inline Vector3<T> Faceforward(const Vector3<T> &v, const Normal3<T> &n2) {
    return (Dot(v, n2) < 0.f) ? -v : v;
}

template <typename T>
Normal3<T> Abs(const Normal3<T> &v) {
    return Normal3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

template <typename T>
Bounds3<T> Union(const Bounds3<T> &b, const Point3<T> &p) {
    Bounds3<T> ret;
    ret.pMin = Min(b.pMin, p);
    ret.pMax = Max(b.pMax, p);
    return ret;
}

template <typename T>
Bounds3<T> Union(const Bounds3<T> &b1, const Bounds3<T> &b2) {
    Bounds3<T> ret;
    ret.pMin = Min(b1.pMin, b2.pMin);
    ret.pMax = Max(b1.pMax, b2.pMax);
    return ret;
}

template <typename T>
Bounds3<T> Intersect(const Bounds3<T> &b1, const Bounds3<T> &b2) {
    Bounds3<T> ret;
    ret.pMin = Max(b1.pMin, b2.pMin);
    ret.pMax = Min(b1.pMax, b2.pMax);
    return ret;
}

template <typename T>
bool Overlaps(const Bounds3<T> &b1, const Bounds3<T> &b2) {
    bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
    bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
    bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);
    return (x && y && z);
}

template <typename T> 
bool Inside(const Point3<T> &p, const Bounds3<T> &b) {
    return p.x >= b.pMin.x && p.y >= b.pMin.y && p.z >= b.pMin.z &&
           p.x <= b.pMax.x && p.y <= b.pMax.y && p.z <= b.pMax.z;
}

template <typename T>
bool InsideExclusive(const Point3<T> &p, const Bounds3<T> &b) {
    return (p.x >= b.pMin.x && p.x < b.pMax.x && p.y >= b.pMin.y &&
            p.y < b.pMax.y && p.z >= b.pMin.z && p.z < b.pMax.z);
}

template <typename T, typename U>
inline Bounds3<T> Expand(const Bounds3<T> &b, U delta) {
    return Bounds3<T>(b.pMin - Vector3<T>(delta, delta, delta),
                      b.pMax + Vector3<T>(delta, delta, delta));
}

// Minimum squared distance from point to box; returns zero if point is
// inside.
template <typename T, typename U>
inline Float DistanceSquared(const Point3<T> &p, const Bounds3<U> &b) {
    Float dx = std::max({Float(0), b.pMin.x - p.x, p.x - b.pMax.x});
    Float dy = std::max({Float(0), b.pMin.y - p.y, p.y - b.pMax.y});
    Float dz = std::max({Float(0), b.pMin.z - p.z, p.z - b.pMax.z});
    return dx * dx + dy * dy + dz * dz;
}

template <typename T, typename U>
inline Float Distance(const Point3<T> &p, const Bounds3<U> &b) {
    return std::sqrt(DistanceSquared(p, b));
}

template <typename T>
Bounds2<T> Union(const Bounds2<T> &b, const Point2<T> &p) {
    Bounds2<T> ret;
    ret.pMin = Min(b.pMin, p);
    ret.pMax = Max(b.pMax, p);
    return ret;
}

template <typename T>
Bounds2<T> Union(const Bounds2<T> &b, const Bounds2<T> &b2) {
    Bounds2<T> ret;
    ret.pMin = Min(b.pMin, b2.pMin);
    ret.pMax = Max(b.pMax, b2.pMax);
    return ret;
}

template <typename T>
Bounds2<T> Intersect(const Bounds2<T> &b1, const Bounds2<T> &b2) {
    Bounds2<T> ret;
    ret.pMin = Max(b1.pMin, b2.pMin);
    ret.pMax = Min(b1.pMax, b2.pMax);
    return ret;
}

template <typename T>
bool Overlaps(const Bounds2<T> &ba, const Bounds2<T> &bb) {
    bool x = (ba.pMax.x >= bb.pMin.x) && (ba.pMin.x <= bb.pMax.x);
    bool y = (ba.pMax.y >= bb.pMin.y) && (ba.pMin.y <= bb.pMax.y);
    return (x && y);
}

template <typename T>
bool Inside(const Point2<T> &pt, const Bounds2<T> &b) {
    return (pt.x >= b.pMin.x && pt.x <= b.pMax.x && pt.y >= b.pMin.y &&
            pt.y <= b.pMax.y);
}

template <typename T>
bool InsideExclusive(const Point2<T> &pt, const Bounds2<T> &b) {
    return (pt.x >= b.pMin.x && pt.x < b.pMax.x && pt.y >= b.pMin.y &&
            pt.y < b.pMax.y);
}

template <typename T, typename U>
Bounds2<T> Expand(const Bounds2<T> &b, U delta) {
    return Bounds2<T>(b.pMin - Vector2<T>(delta, delta),
                      b.pMax + Vector2<T>(delta, delta));
}

template <typename T>
inline bool Bounds3<T>::IntersectP(const Ray &ray, Float &hitt0, Float &hitt1) const {
    Float t0 = 0, t1 = ray.tMax;
    for (int i = 0; i < 3; ++i) {
        // Update interval for _i_th bounding box slab
        Float invRayDir = 1 / ray.d[i];
        Float tNear = (pMin[i] - ray.o[i]) * invRayDir;
        Float tFar = (pMax[i] - ray.o[i]) * invRayDir;

        // Update parametric interval from slab intersection $t$ values
        if (tNear > tFar) std::swap(tNear, tFar);

        // Update _tFar_ to ensure robust ray--bounds intersection
        tFar *= 1 + 2 * pbrt::gamma(3);
        t0 = tNear > t0 ? tNear : t0; // choose latest enter and first go out
        t1 = tFar < t1 ? tFar : t1;
        if (t0 > t1) return false;
    }
    hitt0 = t0;
    hitt1 = t1;
    return true;
}

template <typename T>
inline bool Bounds3<T>::IntersectP(const Ray &ray, const Vector3f &invDir, const int dirIsNeg[3]) const {
    const Bounds3f &bounds = *this;
    // Check for ray intersection against $x$ and $y$ slabs
    Float tMin = (bounds[dirIsNeg[0]].x - ray.o.x) * invDir.x;
    Float tMax = (bounds[1 - dirIsNeg[0]].x - ray.o.x) * invDir.x;
    Float tyMin = (bounds[dirIsNeg[1]].y - ray.o.y) * invDir.y;
    Float tyMax = (bounds[1 - dirIsNeg[1]].y - ray.o.y) * invDir.y;

    // Update _tMax_ and _tyMax_ to ensure robust bounds intersection
    tMax *= 1 + 2 * gamma(3);
    tyMax *= 1 + 2 * gamma(3);
    if (tMin > tyMax || tyMin > tMax) return false;
    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    // Check for ray intersection against $z$ slab
    Float tzMin = (bounds[dirIsNeg[2]].z - ray.o.z) * invDir.z;
    Float tzMax = (bounds[1 - dirIsNeg[2]].z - ray.o.z) * invDir.z;

    // Update _tzMax_ to ensure robust bounds intersection
    tzMax *= 1 + 2 * gamma(3);
    if (tMin > tzMax || tzMin > tMax) return false;
    if (tzMin > tMin) tMin = tzMin;
    if (tzMax < tMax) tMax = tzMax;
    return (tMin < ray.tMax) && (tMax > 0);
}

inline Vector3f SphericalDirection(Float sinTheta, Float cosTheta, Float phi) {
    return Vector3f(sinTheta * std::cos(phi), sinTheta * std::sin(phi),
                    cosTheta);
}

inline Vector3f SphericalDirection(Float sinTheta, Float cosTheta, Float phi,
                                   const Vector3f &x, const Vector3f &y,
                                   const Vector3f &z) {
    return sinTheta * std::cos(phi) * x + sinTheta * std::sin(phi) * y +
           cosTheta * z;
}

inline Float SphericalTheta(const Vector3f &v) {
    return std::acos(Clamp(v.z, -1, 1));
}

inline Float SphericalPhi(const Vector3f &v) {
    Float p = std::atan2(v.y, v.x);
    return (p < 0) ? (p + 2 * Pi) : p;
}


} // namespace pbrt

#endif // PBRT_SRC_CORE_GEOMETRIC_H_