#pragma once

#include <cmath>
#include <iostream>

template<typename T>
struct Vec2 {
    union {
        struct {
            T u, v;
        };
        struct {
            T x, y;
        };
        T raw[2];
    };

    Vec2() : u(T()), v(T()) {}
    Vec2(T a, T b) : u(a), v(b) {}
    inline Vec2<T> operator+(const Vec2<T> &o) const { return Vec2<T>(u + o.u, v + o.v); }
    inline Vec2<T> operator-(const Vec2<T> &o) const { return Vec2<T>(u - o.u, v - o.v); }
    inline Vec2<T> operator*(T f) const { return Vec2<T>(u * f, v * f); }
    template<typename>
    friend std::ostream &operator<<(std::ostream &os, const Vec2<T> &o);
};

template<typename T>
inline Vec2<T> operator+(const Vec2<T> &lhs, const Vec2<T> &rhs) {
    return lhs.operator+(rhs);
}

template<typename T>
inline Vec2<T> operator-(const Vec2<T> &lhs, const Vec2<T> &rhs) {
    return lhs.operator-(rhs);
}

template<typename T>
inline Vec2<T> operator*(const Vec2<T> &lhs, const T rhs) {
    return lhs.operator*(rhs);
}

template<typename T>
inline Vec2<T> operator*(const T lhs, const Vec2<T> &rhs) {
    return rhs.operator*(lhs);
}

template<typename T>
struct Vec3 {
    union {
        struct {
            T x, y, z;
        };
        struct {
            T ivert, iuv, inorm;
        };
        T raw[3];
    };
    Vec3() : x(T()), y(T()), z(T()) {}
    Vec3(T a, T b, T c) : x(a), y(b), z(c) {}
    inline Vec3<T> operator+(const Vec3<T> &o) const { return Vec3<T>(x + o.x, y + o.y, z + o.z); }
    inline Vec3<T> operator-(const Vec3<T> &o) const { return Vec3<T>(x - o.x, y - o.y, z - o.z); }
    inline Vec3<T> operator*(T f) const { return Vec3<T>(x * f, y * f, z * f); }
    inline T operator*(const Vec3<T> &o) const { return x * o.x + y * o.y + z * o.z; }
    inline Vec3<T> operator^(const Vec3<T> &o) const { return Vec3<T>(y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x); }

    T norm() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3<T> &normalize(T l = 1) {
        *this = (*this) / (l / norm());
        return *this;
    }
    template<typename>
    friend std::ostream &operator<<(std::ostream &os, const Vec3<T> &o);
};

template<typename T>
inline Vec3<T> operator+(const Vec3<T> &lhs, const Vec3<T> &rhs) {
    return lhs.operator+(rhs);
}

template<typename T>
inline Vec3<T> operator-(const Vec3<T> &lhs, const Vec3<T> &rhs) {
    return lhs.operator-(rhs);
}

template<typename T>
inline Vec3<T> operator*(const Vec3<T> &lhs, const Vec3<T> &rhs) {
    return lhs.operator*(rhs);
}

template<typename T>
inline Vec3<T> operator^(const Vec3<T> &lhs, const Vec3<T> &rhs) {
    return lhs.operator^(rhs);
}

using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;
using Vec3f = Vec3<float>;
using Vec3i = Vec3<int>;

template<typename T>
std::ostream &operator<<(std::ostream &os, const Vec2<T> &o) {
    os << "(" << o.x << ", " << o.y << ")\n";
    return os;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const Vec3<T> &o) {
    os << "(" << o.x << ", " << o.y << ", " << o.z << ")\n";
    return os;
}
