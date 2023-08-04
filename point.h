#ifndef OOP_ASSIGNMENTS_POINT_H
#define OOP_ASSIGNMENTS_POINT_H

#include "ishape.h"
#include <sstream>

bool inline IsDoubleEqual(double x, double y) {
  return (std::abs(x - y) < 0.000001);
}
namespace geometry {

class Point : public IShape {

 public:
  double x_;
  double y_;

  Point() = default;
  Point(double x, double y) : x_(x), y_(y) {
  }
  // ~Point() = default;
  //  Point(const Point&) = default;
  //   Point& operator=(const Point&) = default;
  bool operator==(const Point& point) const {
    return (IsDoubleEqual(x_, point.x_) && IsDoubleEqual(y_, point.y_));
  }
  bool ContainsPoint(const Point& point) const override {
    return *this == point;
  }

  Point& Move(const Vector& vector) override;

  bool CrossesSegment(const Segment& s) const override;

  Point* Clone() const override {
    auto p = new Point;
    p->x_ = x_;
    p->y_ = y_;
    return p;
  }

  std::string ToString() const override {
    std::ostringstream os;
    os << "Point(" << x_ << ", " << y_ << ")" << '\n';
    return os.str();
  }
};
}  // namespace geometry

#endif
