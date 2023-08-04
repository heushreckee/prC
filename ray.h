#ifndef OOP_ASSIGNMENTS_RAY_H
#define OOP_ASSIGNMENTS_RAY_H

#include "ishape.h"
#include <sstream>
#include "line.h"

namespace geometry {

bool inline RayIsTrue(Point p1, Point p2, Point a) {
  if (S(p1, p2, a)) {
    if (p1.y_ > p2.y_) {
      if (p1.x_ > p2.x_) {
        return a.x_ <= p1.x_ && a.y_ <= p1.y_;
      }
      return a.x_ >= p1.x_ && a.y_ <= p1.y_;
    }
    if (p1.x_ > p2.x_) {
      return a.x_ <= p1.x_ && a.y_ >= p1.y_;
    }
    return a.x_ >= p1.x_ && a.y_ >= p1.y_;
  }
  return false;
}

double inline OordinateTeY(const Line& l, const Line& ah) {
  return ((ah.c_ * l.a_ - l.c_ * ah.a_) / (l.b_ * ah.a_ - ah.b_ * l.a_));
}

double inline OordinateTeX(const Line& l, const Line& ah) {
  double y = OordinateTeY(l, ah);
  if (l.a_ != 0) {
    return (-l.c_ - l.b_ * y) / l.a_;
  }
  return (-ah.c_ - ah.b_ * y) / ah.a_;
}

class Ray : public IShape {
 public:
  Line l_;
  Point p1_;
  Point p2_;
  Ray() = default;
  Ray(const Point& point1, const Point& point2) {
    p1_ = point1;
    p2_ = point2;
    l_ = Line(point1, point2);
  }
  bool ContainsPoint(const Point& point) const override {
    return RayIsTrue(p1_, p2_, point);
  }
  bool CrossesSegment(const Segment& segment) const override {
    if (segment.CrossesSegment(Segment(p1_, p2_))) {
      return true;
    }
    Line ah(segment.p1_, segment.p2_);
    Point a(OordinateTeX(l_, ah), OordinateTeY(l_, ah));
    return segment.ContainsPoint(a) && ContainsPoint(a);
  }
  std::string ToString() const override {
    std::ostringstream os;
    os << "Ray(Point(" << p1_.x_ << ", " << p1_.y_ << "), Vector(" << p2_.x_ - p1_.x_ << ", " << p2_.y_ - p1_.y_ << "))"
       << '\n';
    return os.str();
  }
  Ray* Clone() const override {
    auto r = new Ray;
    r->p1_ = p1_;
    r->p2_ = p2_;
    r->l_ = Line(p1_, p2_);
    return r;
  }
  Ray& Move(const Vector& vector) override {
    l_.Move(vector);
    p1_.Move(vector);
    p2_.Move(vector);
    return *this;
  }
};
}  // namespace geometry
#endif  // OOP_ASSIGNMENTS_RAY_H
