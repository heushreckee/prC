#ifndef OOP_ASSIGNMENTS_LINE_H
#define OOP_ASSIGNMENTS_LINE_H

#include "ishape.h"
#include <sstream>
#include <cmath>

namespace geometry {
class Line : public IShape {
 public:
  double a_;
  double b_;
  double c_;

  Line() = default;
  Line(double a, double b, double c) : a_(a), b_(b), c_(c) {
  }

  Line(const Point& first, const Point& second) {
    {
      if (first.x_ == second.x_) {
        b_ = 0;
        a_ = 1;
        c_ = -first.x_;
        return;
      }
      if (first.y_ == second.y_) {
        b_ = 1;
        a_ = 0;
        c_ = -first.y_;
        return;
      }
      a_ = second.y_ - first.y_;
      b_ = first.x_ - second.x_;
      c_ = (second.x_ - first.x_) * first.y_ - first.x_ * (second.y_ - first.y_);
    }
  }
  bool ContainsPoint(const Point& point) const override {
    return (a_ * point.x_ + b_ * point.y_ + c_) == 0;
  }
  Line& Move(const Vector& vector) override {
    c_ += -a_ * vector.x_ - b_ * vector.y_;
    return *this;
  }
  double OordinateTeY(const Line& l, const Line& ah) const {
    return ((ah.c_ * l.a_ - l.c_ * ah.a_) / (l.b_ * ah.a_ - ah.b_ * l.a_));
  }

  double OordinateTeX(const Line& l, const Line& ah) const {
    double y = OordinateTeY(l, ah);
    if (l.a_ != 0) {
      return (-l.c_ - l.b_ * y) / l.a_;
    }
    return (-ah.c_ - ah.b_ * y) / ah.a_;
  }

  bool Section1(const Point& p1, const Point& p2, const Point& a) const {
    return S(p1, p2, a) && a.y_ <= std::max(p1.y_, p2.y_) && a.x_ <= std::max(p1.x_, p2.x_) &&
           a.y_ >= std::min(p1.y_, p2.y_) && a.x_ >= std::min(p1.x_, p2.x_);
  }

  bool CrossesSegment(const Segment& segment) const override {
    if (ContainsPoint(segment.p1_) || ContainsPoint(segment.p2_)) {
      return true;
    }
    Line l(segment.p1_, segment.p2_);
    Point a(OordinateTeX(*this, l), OordinateTeY(*this, l));
    return Section1(segment.p1_, segment.p2_, a);
  }

  Line* Clone() const override {
    auto l = new Line;
    l->a_ = a_;
    l->b_ = b_;
    l->c_ = c_;
    return l;
  }

  std::string ToString() const override {
    std::ostringstream os;
    os << "Line(" << a_ << ", " << b_ << ", " << c_ << ")" << '\n';
    return os.str();
  }
};

bool inline Distance(Line line1, Line line2, Point p1, Point p2, Point p3, Point p4) {
  if (line1.a_ * line2.b_ - line1.b_ * line2.a_ == 0) {
    if (line1.a_ > line2.a_) {
      double x = line1.a_ / line2.a_;
      line1.a_ /= x;
      line1.b_ /= x;
      line1.c_ /= x;
    } else {
      if (line1.a_ != 0) {
        double x = line2.a_ / line1.a_;
        line2.a_ /= x;
        line2.b_ /= x;
        line2.c_ /= x;
      } else {
        if (line1.b_ > line2.b_) {
          double x = line1.b_ / line2.b_;
          line1.a_ /= x;
          line1.b_ /= x;
          line1.c_ /= x;
        } else {
          if (line1.b_ != 0) {
            double x = line2.b_ / line1.b_;
            line2.a_ /= x;
            line2.b_ /= x;
            line2.c_ /= x;
          }
        }
      }
    }

    if (p1 == p3 || p3 == p2 || p1 == p4 || p4 == p2) {
      return true;
    }
    double x = (line2.c_ - line1.c_) / sqrt(line1.a_ * line1.a_ + line1.b_ * line1.b_);
    if (x == 0) {
      if (p1.x_ == p2.x_ && p2.x_ == p3.x_ && p3.x_ == p4.x_) {
        return std::min(p3.y_, p4.y_) <= std::max(p2.y_, p1.y_);
      }
      return std::min(p3.x_, p4.x_) <= std::max(p2.x_, p1.x_);
    }
    return false;
  }
  double y = (line2.c_ * line1.a_ - line1.c_ * line2.a_) / (line1.b_ * line2.a_ - line2.b_ * line1.a_);
  double x = 0;
  if (line1.a_ != 0) {
    x = (-line1.b_ * y - line1.c_) / line1.a_;
  } else {
    x = (-line2.b_ * y - line2.c_) / line2.a_;
  }
  Point a(x, y);
  if (a.x_ <= std::max(p1.x_, p2.x_) && a.x_ <= std::max(p3.x_, p4.x_)) {
    if (a.y_ <= std::max(p1.y_, p2.y_) && a.y_ <= std::max(p3.y_, p4.y_)) {
      if (a.x_ >= std::min(p1.x_, p2.x_) && a.x_ >= std::min(p3.x_, p4.x_)) {
        return a.y_ >= std::min(p1.y_, p2.y_) && a.y_ >= std::min(p3.y_, p4.y_);
      }
    }
  }
  return false;
}

bool inline Straight(const Line& l, const Point& a) {
  return l.a_ * a.x_ + l.b_ * a.y_ + l.c_ == 0;
}

bool inline Section(const Point& p1, const Point& p2, const Point& a, const Line& l) {
  return Straight(l, a) && a.y_ <= std::max(p1.y_, p2.y_) && a.x_ <= std::max(p1.x_, p2.x_) &&
         a.y_ >= std::min(p1.y_, p2.y_) && a.x_ >= std::min(p1.x_, p2.x_);
}

bool inline Segment::CrossesSegment(const Segment& s) const {
  Line l1(0, 0, 0);
  if (!(p1_ == p2_)) {
    l1 = Line(p2_.y_ - p1_.y_, p1_.x_ - p2_.x_, (p2_.x_ - p1_.x_) * p1_.y_ - p1_.x_ * (p2_.y_ - p1_.y_));
  }
  Line l2(0, 0, 0);
  if (!(s.p1_ == s.p2_)) {
    l2 = Line(s.p2_.y_ - s.p1_.y_, s.p1_.x_ - s.p2_.x_,
              (s.p2_.x_ - s.p1_.x_) * s.p1_.y_ - s.p1_.x_ * (s.p2_.y_ - s.p1_.y_));
  }
  if (!(p1_ == p2_) && !(s.p1_ == s.p2_)) {
    return Distance(l1, l2, p1_, p2_, s.p1_, s.p2_);
  }
  if (s.p1_ == s.p2_) {
    return Section(p1_, p2_, s.p1_, l1);
  }
  return Section(s.p1_, s.p2_, p1_, l2);
}

}  // namespace geometry
#endif  // OOP_ASSIGNMENTS_LINE_H
