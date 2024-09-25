/*
 *  Types for dealing with vectors.
 */

#ifndef _ecs_vec_h
#define _ecs_vec_h

#include "iro/common.h"
#include "iro/io/format.h"

// Not a fan of doing this, but whatever.
#include "cmath"

/* ============================================================================
 */
template<typename T>
struct vec2
{
  union
  {
    T arr[2];
    struct { T x; T y; };
    struct { T w; T h; };
    struct { T u; T v; };
  };

  vec2() { x = 0; y = 0; }
  vec2(T x, T y) { this->x = x; this->y = y; }
  
  template<typename X>
  vec2(const vec2<X>& in) { x = in.x; y = in.y; }

  /* --------------------------------------------------------------------------
   */
  inline vec2 operator+ (const vec2& rhs) const 
  {
    return { x + rhs.x, y + rhs.y };
  }

  /* --------------------------------------------------------------------------
   */
  inline void operator+= (const vec2& rhs)
  {
    *this = *this + rhs;
  }

  /* --------------------------------------------------------------------------
   */
  inline vec2 operator- (const vec2& rhs) const
  {
    return { x - rhs.x, y - rhs.y };
  }

  /* --------------------------------------------------------------------------
   */
  inline void operator-= (const vec2& rhs)
  {
    *this = *this - rhs;
  }

  /* --------------------------------------------------------------------------
   */
  inline vec2 operator* (T rhs) const
  {
    return { x * rhs, y * rhs };
  }

  /* --------------------------------------------------------------------------
   */
  inline void operator*= (T rhs)
  {
    *this = *this * rhs;
  }

  /* --------------------------------------------------------------------------
   */
  inline vec2 operator/ (T rhs) const
  {
    return { x / rhs, y / rhs };
  }

  /* --------------------------------------------------------------------------
   */
  inline void operator/= (T rhs)
  {
    *this = *this / rhs;
  }

  /* --------------------------------------------------------------------------
   */
  inline vec2 operator- () const
  {
    return { -x, -y };
  }

  /* --------------------------------------------------------------------------
   */
  inline vec2& set(T x, T y)
  {
    this->x = x;
    this->y = y;
    return *this;
  }

  /* --------------------------------------------------------------------------
   */
  inline vec2& setX(T x)
  {
    this->x = x;
    return *this;
  }

  /* --------------------------------------------------------------------------
   */
  inline vec2& setY(T y)
  {
    this->y = y;
    return *this;
  }

  /* --------------------------------------------------------------------------
   */
  inline T dot(const vec2& rhs) const
  {
    return (x * rhs.x) + (y * rhs.y);
  }

  /* --------------------------------------------------------------------------
   */
  inline vec2 perp() const
  {
    return { -y, x };
  }

  /* --------------------------------------------------------------------------
   */
  inline T mag() const
  {
    return sqrt(magSq());
  }

  /* --------------------------------------------------------------------------
   */
  inline T magSq() const
  {
    return x * x + y * y;
  }

  /* --------------------------------------------------------------------------
   */
  inline void normalize()
  {
    *this = normalized();
  }

  /* --------------------------------------------------------------------------
   */
  inline vec2 normalized() const
  {
    if (x || y)
      return *this / mag();
    else
      return *this;
  }

  /* --------------------------------------------------------------------------
   */
  inline void clampMag(T min, T max)
  {
    *this = clampedMag(min, max);
  }

  /* --------------------------------------------------------------------------
   */
  inline vec2 clampedMag(T min, T max) const
  {
    T m = mag();
    if (m < min)
      return min * normalized();
    else if (m > max)
      return max * normalized();
    else 
      return *this;
  }
};

typedef vec2<s32> vec2i;
typedef vec2<f32> vec2f;

namespace iro::io
{

template<typename T>
s64 format(io::IO* io, vec2<T>& v)
{
  return io::formatv(io, '(', v.x, ',', v.y, ')');
}

}

/* ----------------------------------------------------------------------------
 */
inline vec2f floor(const vec2f& x)
{
  return vec2f(floor(x.x), floor(x.y));
}

inline vec2f max(const vec2f& x, const vec2f& y)
{
  return vec2f(max(x.x, y.x), max(x.y, y.y));
}

inline vec2f min(const vec2f& x, const vec2f& y)
{
  return vec2f(min(x.x, y.x), min(x.y, y.y));
}

/* ============================================================================
 */
template<typename T>
struct vec4
{
  union
  {
    T arr[4];
    struct
    {
      union
      {
        vec2<T> xy;
        struct { T x, y; };
      };
      union
      {
        vec2<T> zw;
        struct { T z, w; };
      };
    };
  };

  vec4() { x = 0; y = 0; z = 0; w = 0; }
  vec4(T x, T y, T z, T w) 
  { 
    this->x = x; 
    this->y = y; 
    this->z = z;
    this->w = w;
  }
  
  template<typename X>
  vec4(const vec4<X>& in) { x = in.x; y = in.y; z = in.z; w = in.w; }

  /* --------------------------------------------------------------------------
   */
  inline vec4 operator+ (const vec4& rhs) const
  {
    return
    {
      x + rhs.x,
      y + rhs.y,
      z + rhs.z,
      w + rhs.w,
    };
  }

  /* --------------------------------------------------------------------------
   */
  inline void operator+= (const vec4& rhs)
  {
    *this = *this + rhs;
  }

  /* --------------------------------------------------------------------------
   */
  inline vec4 operator- (const vec4& rhs) const
  {
    return
    {
      x - rhs.x,
      y - rhs.y,
      z - rhs.z,
      w - rhs.w,
    };
  }

  /* --------------------------------------------------------------------------
   */
  inline void operator-= (const vec4& rhs)
  {
    *this = *this - rhs;
  }

  /* --------------------------------------------------------------------------
   */
  inline vec4 operator* (T rhs) const
  {
    return
    {
      x * rhs,
      y * rhs,
      z * rhs,
      w * rhs,
    };
  }

  /* --------------------------------------------------------------------------
   */
  inline void operator*= (T rhs)
  {
    *this = *this * rhs;
  }

  /* --------------------------------------------------------------------------
   */
  inline vec4 operator/ (T rhs) const
  {
    return
    {
      x / rhs,
      y / rhs,
      z / rhs,
      w / rhs,
    };
  }

  /* --------------------------------------------------------------------------
   */
  inline void operator/= (T rhs)
  {
    *this = *this / rhs;
  }

  /* --------------------------------------------------------------------------
   */
  inline vec4 operator- () const
  {
    return { -x, -y, -z, -w };
  }

  /* --------------------------------------------------------------------------
   */
  inline vec4& set(f32 x, f32 y, f32 z, f32 w) 
  {
    *this = { x, y, z, w };
    return *this;
  }

  /* --------------------------------------------------------------------------
   */
  inline vec4& setX(f32 v) { x = v; return *this; }
  inline vec4& setY(f32 v) { y = v; return *this; }
  inline vec4& setZ(f32 v) { z = v; return *this; }
  inline vec4& setW(f32 v) { w = v; return *this; }

  /* --------------------------------------------------------------------------
   */
  inline T dot(const vec4& rhs) const
  {
    return 
      x * rhs.x + 
      y * rhs.y + 
      z * rhs.z + 
      w * rhs.w;
  }
};

typedef vec4<s32> vec4i;
typedef vec4<f32> vec4f;

namespace iro::io
{

template<typename T>
s64 format(io::IO* io, vec4<T>& v)
{
  return io::formatv(io, '(', v.x, ',', v.y, ',', v.z, ',', v.w, ')');
}

}

#endif
