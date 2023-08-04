#include "rational.h"
L Gcd(L a, L b) {
  while (b) {
    a %= b;
    std::swap(a, b);
  }
  return a;
}
void Rational::ToNormal() {
  L gcd = Gcd(num_, den_);
  den_ /= gcd;
  num_ /= gcd;
  if (den_ < 0) {
    den_ *= -1;
    num_ *= -1;
  }
}
Rational::Rational() : num_(0), den_(1) {
}
Rational::Rational(L r) : num_(r), den_(1) {
}
Rational::Rational(L n, L d) {
  if (d == 0) {
    throw RationalDivisionByZero{};
  }
  num_ = n;
  den_ = d;
  ToNormal();
}
Rational::Rational(const Rational &r) {
  if (r.den_ == 0) {
    throw RationalDivisionByZero{};
  }
  num_ = r.num_;
  den_ = r.den_;
  ToNormal();
}
Rational &Rational::operator=(const Rational &r) {
  if (r.den_ == 0) {
    throw RationalDivisionByZero{};
  }
  num_ = r.num_;
  den_ = r.den_;
  ToNormal();
  return *this;
}
L Rational::GetNumerator() const {
  return num_;
}
L Rational::GetDenominator() const {
  return den_;
}
void Rational::SetNumerator(L n) {
  num_ = n;
  ToNormal();
}
void Rational::SetDenominator(L d) {
  if (d == 0) {
    throw RationalDivisionByZero{};
  }
  den_ = d;
  ToNormal();
}
Rational &Rational::operator+=(const Rational &right) {
  if (right.den_ == 0) {
    throw RationalDivisionByZero{};
  }
  num_ *= right.den_;
  num_ += right.num_ * den_;
  den_ *= right.GetDenominator();
  ToNormal();
  return *this;
}
Rational &Rational::operator-=(const Rational &right) {
  if (right.den_ == 0) {
    throw RationalDivisionByZero{};
  }
  num_ *= right.GetDenominator();
  num_ -= right.GetNumerator() * den_;
  den_ *= right.GetDenominator();
  ToNormal();
  return *this;
}
Rational &Rational::operator*=(const Rational &right) {
  if (right.den_ == 0) {
    throw RationalDivisionByZero{};
  }
  num_ *= right.GetNumerator();
  den_ *= right.GetDenominator();
  ToNormal();
  return *this;
}
Rational &Rational::operator/=(const Rational &right) {
  if (right.den_ == 0 || right.num_ == 0) {
    throw RationalDivisionByZero{};
  }
  num_ *= right.GetDenominator();
  den_ *= right.GetNumerator();
  ToNormal();
  return *this;
}
Rational &Rational::operator++() {
  num_ += den_;
  return *this;
}
Rational &Rational::operator--() {
  num_ -= den_;
  return *this;
}
Rational Rational::operator++(int) {
  Rational temp(num_, den_);
  ++(*this);
  return temp;
}
Rational Rational::operator--(int) {
  Rational temp(num_, den_);
  --(*this);
  return temp;
}
Rational Rational::operator-() const {
  return {-num_, den_};
}
Rational Rational::operator+() const {
  return {num_, den_};
}
Rational operator+(const Rational &right, const Rational &left) {
  L n = right.GetNumerator() * left.GetDenominator() + left.GetNumerator() * right.GetDenominator();
  L d = right.GetDenominator() * left.GetDenominator();
  return {n, d};
}
Rational operator-(const Rational &right, const Rational &left) {
  L n = right.GetNumerator() * left.GetDenominator() - left.GetNumerator() * right.GetDenominator();
  L d = right.GetDenominator() * left.GetDenominator();
  return {n, d};
}
Rational operator*(const Rational &right, const Rational &left) {
  L n = right.GetNumerator() * left.GetNumerator();
  L d = right.GetDenominator() * left.GetDenominator();
  return {n, d};
}
Rational operator/(const Rational &right, const Rational &left) {
  L n = right.GetNumerator() * left.GetDenominator();
  L d = right.GetDenominator() * left.GetNumerator();
  return {n, d};
}
bool operator<(const Rational &right, const Rational &left) {
  L r = right.GetNumerator() * left.GetDenominator();
  L l = right.GetDenominator() * left.GetNumerator();
  return (r < l);
}
bool operator>(const Rational &right, const Rational &left) {
  L r = right.GetNumerator() * left.GetDenominator();
  L l = right.GetDenominator() * left.GetNumerator();
  return (r > l);
}
bool operator<=(const Rational &right, const Rational &left) {
  L r = right.GetNumerator() * left.GetDenominator();
  L l = right.GetDenominator() * left.GetNumerator();
  return (r <= l);
}
bool operator>=(const Rational &right, const Rational &left) {
  L r = right.GetNumerator() * left.GetDenominator();
  L l = right.GetDenominator() * left.GetNumerator();
  return (r >= l);
}
bool operator==(const Rational &right, const Rational &left) {
  L r = right.GetNumerator() * left.GetDenominator();
  L l = right.GetDenominator() * left.GetNumerator();
  return (r == l);
}
bool operator!=(const Rational &right, const Rational &left) {
  L r = right.GetNumerator() * left.GetDenominator();
  L l = right.GetDenominator() * left.GetNumerator();
  return (r != l);
}
std::ostream &operator<<(std::ostream &out, const Rational &r) {
  if (r.GetDenominator() != 1) {
    out << r.GetNumerator() << '/' << r.GetDenominator();
  } else {
    out << r.GetNumerator();
  }
  return out;
}
std::istream &operator>>(std::istream &in, Rational &r) {
  L n = 0;
  L d = 1;
  in >> n;
  if (in.peek() == '/') {
    in.ignore();
    in >> d;
    if (d == 0) {
      throw RationalDivisionByZero{};
    }
  }
  L gcd = Gcd(n, d);
  r.den_ = d / gcd;
  r.num_ = n / gcd;
  if (r.den_ < 0) {
    r.den_ *= -1;
    r.num_ *= -1;
  }
  return in;
}
