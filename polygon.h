#ifndef OOP_ASSIGNMENTS_POLYGON_H
#define OOP_ASSIGNMENTS_POLYGON_H

#include "ishape.h"
#include <sstream>

namespace geometry {
class Polygon : public IShape {
 public:
  std::vector<Point> v_;

  Polygon() = default;
  Polygon(const std::vector<Point>& v) : v_(v) {  // NOLINT
  }
  bool ContainsPoint(const Point& point) const override;
  bool CrossesSegment(const Segment& segment) const override {
    Segment s;
    for (uint64_t i = 0; i < v_.size() - 1; ++i) {
      s = Segment(v_[i], v_[i + 1]);
      if (s.CrossesSegment(segment)) {
        return true;
      }
    }
    s = Segment(v_[v_.size() - 1], v_[0]);
    return (s.CrossesSegment(segment));
  }

  std::string ToString() const override {
    std::ostringstream os;
    os << "Polygon(";
    for (uint64_t i = 0; i < v_.size(); ++i) {
      if (i != 0) {
        os << ", ";
      }
      os << "Point(" << v_[i].x_ << ", " << v_[i].y_ << ")";
    }
    os << ")" << '\n';
    return os.str();
  }
  Polygon* Clone() const override {
    auto pl = new Polygon;
    pl->v_ = v_;
    return pl;
  }

  Polygon& Move(const Vector& vector) override {
    for (uint64_t i = 0; i < v_.size(); ++i) {
      v_[i].Move(vector);
    }
    return *this;
  }
};

bool inline Straight2(Point& p1, Point& p2, Point& a) {
  if (p1.x_ == p2.x_) {
    return (a.x_ == p1.x_);
  }
  if (p1.y_ == p2.y_) {
    return (a.y_ == p1.y_);
  }
  return (a.x_ - p1.x_) / (p2.x_ - p1.x_) == (a.y_ - p1.y_) / (p2.y_ - p1.y_);
}

bool inline Section1(Point& p1, Point& p2, Point& a) {
  return Straight2(p1, p2, a) && a.y_ <= std::max(p1.y_, p2.y_) && a.x_ <= std::max(p1.x_, p2.x_) &&
         a.y_ >= std::min(p1.y_, p2.y_) && a.x_ >= std::min(p1.x_, p2.x_);
}

bool inline Belongs(std::vector<Point> sa, int n, Point a) {
  bool result = false;
  int j = n - 1;
  if (Section1(sa[n - 1], sa[0], a)) {
    return true;
  }
  for (int i = 0; i < n - 1; ++i) {
    if (Section1(sa[i], sa[i + 1], a)) {
      return true;
    }
  }
  for (int i = 0; i < n; ++i) {
    if (sa[i] == a) {
      return true;
    }
    if (((sa[i].y_ < a.y_ && sa[j].y_ >= a.y_) || (sa[j].y_ < a.y_ && sa[i].y_ >= a.y_)) &&
        (sa[i].x_ + (a.y_ - sa[i].y_) / (sa[j].y_ - sa[i].y_) * (sa[j].x_ - sa[i].x_) < a.x_)) {
      result = !result;
    }
    j = i;
  }
  return result;
}

bool inline Polygon::ContainsPoint(const Point& point) const {
  return Belongs(v_, static_cast<int>(v_.size()), point);
}
}  // namespace geometry

#endif  // OOP_ASSIGNMENTS_POLYGON_H
