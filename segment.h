#ifndef OOP_ASSIGNMENTS_SEGMENT_H
#define OOP_ASSIGNMENTS_SEGMENT_H

#include "ishape.h"
#include <sstream>

namespace geometry {

bool inline S(const Point& p1, const Point& p2, const Point& a) {
  if (p1.x_ == p2.x_) {
    return (a.x_ == p1.x_);
  }
  if (p1.y_ == p2.y_) {
    return (a.y_ == p1.y_);
  }
  return (a.x_ - p1.x_) / (p2.x_ - p1.x_) == (a.y_ - p1.y_) / (p2.y_ - p1.y_);
}

class Segment : public IShape {
 public:
  Point p1_;
  Point p2_;

  Segment() = default;
  Segment(const Point& point1, const Point& point2) {
    p1_ = point1;
    p2_ = point2;
  }
  bool ContainsPoint(const Point& point) const override {
    return S(p1_, p2_, point) && point.y_ <= std::max(p1_.y_, p2_.y_) && point.x_ <= std::max(p1_.x_, p2_.x_) &&
           point.y_ >= std::min(p1_.y_, p2_.y_) && point.x_ >= std::min(p1_.x_, p2_.x_);
  }

  Segment& Move(const Vector& vector) override {
    p1_.Move(vector);
    p2_.Move(vector);
    return *this;
  }

  bool CrossesSegment(const Segment& s) const override;

  Segment* Clone() const override {
    auto s = new Segment;
    s->p1_ = p1_;
    s->p2_ = p2_;
    return s;
  }

  std::string ToString() const override {
    std::ostringstream os;
    os << "Segment(Point(" << p1_.x_ << ", " << p1_.y_ << "), Point(" << p2_.x_ << ", " << p2_.y_ << "))" << '\n';
    return os.str();
  }
};

bool inline Point::CrossesSegment(const Segment& s) const {
  return S(s.p1_, s.p2_, *this) && y_ <= std::max(s.p1_.y_, s.p2_.y_) && x_ <= std::max(s.p1_.x_, s.p2_.x_) &&
         y_ >= std::min(s.p1_.y_, s.p2_.y_) && x_ >= std::min(s.p1_.x_, s.p2_.x_);
}

}  // namespace geometry
#endif  // OOP_ASSIGNMENTS_SEGMENT_H
