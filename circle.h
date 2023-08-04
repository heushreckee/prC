#ifndef OOP_ASSIGNMENTS_CIRCLE_H
#define OOP_ASSIGNMENTS_CIRCLE_H

#include "ishape.h"
#include <sstream>

namespace geometry {
double inline Length(const Point& a, const Point& b) {
  return sqrt((a.x_ - b.x_) * (a.x_ - b.x_) + (a.y_ - b.y_) * (a.y_ - b.y_));
}

double inline Dis(const Line& l, const Point& a) {
  double h = (l.a_ * a.x_ + l.b_ * a.y_ + l.c_) / sqrt(l.a_ * l.a_ + l.b_ * l.b_);
  if (h >= 0) {
    return h;
  }
  return -h;
}
class Circle : public IShape {
 public:
  Point center_;
  double rad_;
  Circle() = default;
  Circle(const Point& center, const double& radius) : center_(center), rad_(radius) {
  }
  bool ContainsPoint(const Point& point) const override {
    return (center_.x_ - point.x_) * (center_.x_ - point.x_) + (center_.y_ - point.y_) * (center_.y_ - point.y_) <=
           rad_ * rad_;
  }
  bool Cont(const Point& point) const {
    return (center_.x_ - point.x_) * (center_.x_ - point.x_) + (center_.y_ - point.y_) * (center_.y_ - point.y_) ==
           rad_ * rad_;
  }
  bool Cont2(const Point& point) const {
    return (center_.x_ - point.x_) * (center_.x_ - point.x_) + (center_.y_ - point.y_) * (center_.y_ - point.y_) <
           rad_ * rad_;
  }
  bool CrossesSegment(const Segment& segment) const override {
    if ((!ContainsPoint(segment.p1_) && ContainsPoint(segment.p2_)) ||
        (ContainsPoint(segment.p1_) && !ContainsPoint(segment.p2_))) {
      return true;
    }
    if (Cont2(segment.p1_) && Cont2(segment.p2_)) {
      return false;
    }
    if (Cont(segment.p1_) || Cont(segment.p2_)) {
      return true;
    }
    if (center_.CrossesSegment(segment)) {
      return true;
    }
    Line l(segment.p2_.y_ - segment.p1_.y_, segment.p1_.x_ - segment.p2_.x_,
           (segment.p2_.x_ - segment.p1_.x_) * segment.p1_.y_ - segment.p1_.x_ * (segment.p2_.y_ - segment.p1_.y_));
    Line ah(l.b_, -l.a_, center_.y_ * l.a_ - l.b_ * center_.x_);
    Point h(center_.x_, center_.y_);
    if (Dis(l, center_) != 0) {
      h.x_ = OordinateTeX(l, ah);
      h.y_ = OordinateTeY(l, ah);
    }
    double las;
    if (Section(segment.p1_, segment.p2_, h, l)) {
      las = Dis(l, center_);
    } else {
      las = std::min(Length(center_, segment.p2_), Length(center_, segment.p1_));
    }
    return las <= (rad_ + 1);
  }

  std::string ToString() const override {
    std::ostringstream os;
    os << "Circle(Point(" << center_.x_ << ", " << center_.y_ << "), " << rad_ << ")" << '\n';
    return os.str();
  }

  Circle* Clone() const override {
    auto* c = new Circle;
    c->center_ = center_;
    c->rad_ = rad_;
    return c;
  }

  Circle& Move(const Vector& vector) override {
    center_.Move(vector);
    return *this;
  }
};
}  // namespace geometry

#endif  // OOP_ASSIGNMENTS_CIRCLE_H
