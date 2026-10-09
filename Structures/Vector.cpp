#pragma once
// Vector structures with operations
// * means elemwise multiplication, not dot or cross product!
template<class T>
struct Vec2 {
    T x, y;
    void operator+=(const Vec2<T>& other) { x += other.x; y += other.y; }
    void operator-=(const Vec2<T>& other) { x -= other.x; y -= other.y; }
    void operator*=(const Vec2<T>& other) { x *= other.x; y *= other.y; }
    Vec2<T> operator+(const Vec2<T>& other) const { return {x + other.x, y + other.y}; }
    Vec2<T> operator-(const Vec2<T>& other) const { return {x - other.x, y - other.y}; }
    Vec2<T> operator*(const Vec2<T>& other) const { return {x * other.x, y * other.y}; }
};
template<class T>
using Point = Vec2<T>;

template<class T>
struct Vec3 {
    T x, y, z;
    void operator+=(const Vec3<T>& o) { x += o.x; y += o.y; z += o.z; }
    void operator-=(const Vec3<T>& o) { x -= o.x; y -= o.y; z -= o.z; }
    void operator*=(const Vec3<T>& o) { x *= o.x; y *= o.y; z *= o.z; }
    Vec3<T> operator+(const Vec3<T>& o) const { return {x + o.x, y + o.y, z + o.z}; }
    Vec3<T> operator-(const Vec3<T>& o) const { return {x - o.x, y - o.y, z - o.z}; }
    Vec3<T> operator*(const Vec3<T>& o) const { return {x * o.x, y * o.y, z * o.z}; }
};
template<class T>
using Point3D = Vec3<T>;

template<class T>
struct Vec4 {
    T x, y, z, t;
    void operator+=(const Vec4<T>& o) { x += o.x; y += o.y; z += o.z; t += o.t; }
    void operator-=(const Vec4<T>& o) { x -= o.x; y -= o.y; z -= o.z; t -= o.t; }
    void operator*=(const Vec4<T>& o) { x *= o.x; y *= o.y; z *= o.z; t *= o.t; }
    Vec4<T> operator+(const Vec4<T>& o) const { return {x + o.x, y + o.y, z + o.z, t + o.t}; }
    Vec4<T> operator-(const Vec4<T>& o) const { return {x - o.x, y - o.y, z - o.z, t - o.t}; }
    Vec4<T> operator*(const Vec4<T>& o) const { return {x * o.x, y * o.y, z * o.z, t * o.t}; }
};