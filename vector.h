#ifndef OOP_ASSIGNMENTS_VECTOR_H
#define OOP_ASSIGNMENTS_VECTOR_H

#include "library.h"
#include "point.h"

namespace geometry {
class Vector {
 public:
  double x_;
  double y_;
  Vector() = default;
  Vector(double x, double y) : x_(x), y_(y) {
  }
  Vector(const Point& p1, const Point& p2) : x_(p2.x_ - p1.x_), y_(p2.y_ - p1.y_) {
  }
  ~Vector() = default;
  bool operator==(const Vector& v) const;
  bool operator!=(const Vector& v) const;
  void operator+=(const Vector& v);
  void operator-=(const Vector& v);
  Vector operator-();
  Vector operator+();
  Vector operator+(const Vector& another) const;
  Vector operator-(const Vector& another) const;
  Vector operator*(double scalar) const;
  Vector operator/(double scalar) const;
};

inline Vector Vector::operator-() {
  Vector v(-x_, -y_);
  return v;
}

inline Vector Vector::operator+() {
  Vector v(x_, y_);
  return v;
}

inline void Vector::operator+=(const Vector& v) {
  x_ += v.x_;
  y_ += v.y_;
}
inline void Vector::operator-=(const Vector& v) {
  x_ -= v.x_;
  y_ -= v.y_;
}
inline bool Vector::operator==(const Vector& v) const {
  return (IsDoubleEqual(x_, v.x_) && IsDoubleEqual(y_, v.y_));
}

inline bool Vector::operator!=(const Vector& v) const {
  return !(*this == v);
}

inline Vector Vector::operator+(const Vector& another) const {
  Vector v(x_ + another.x_, y_ + another.y_);
  return v;
}

inline Vector Vector::operator-(const Vector& another) const {
  Vector v(x_ - another.x_, y_ - another.y_);
  return v;
}

inline Vector Vector::operator*(double scalar) const {
  Vector v(x_ * scalar, y_ * scalar);
  return v;
}

inline Vector Vector::operator/(double scalar) const {
  Vector v(x_ / scalar, y_ / scalar);
  return v;
}

inline Point& Point::Move(const Vector& vector) {
  x_ += vector.x_;
  y_ += vector.y_;
  return *this;
}
inline Vector operator-(const Point& left, const Point& right) {
  Vector v(right, left);
  return v;
}
}  // namespace geometry
#endif
