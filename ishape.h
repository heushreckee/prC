#ifndef OOP_ASSIGNMENTS_ISHAPE_H
#define OOP_ASSIGNMENTS_ISHAPE_H

#include "library.h"
#include <memory>
#include <string>

namespace geometry {

class IShape {
 public:
  virtual bool ContainsPoint(const Point& point) const = 0;
  virtual bool CrossesSegment(const Segment& segment) const = 0;
  virtual std::string ToString() const = 0;
  virtual IShape* Clone() const = 0;
  virtual IShape& Move(const Vector& vector) = 0;

  virtual ~IShape() = default;
};

std::ostream& operator<<(std::ostream& os, const IShape& shape);
}  // namespace geometry

#endif  // OOP_ASSIGNMENTS_ISHAPE_H
