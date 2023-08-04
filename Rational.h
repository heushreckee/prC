#include <stdexcept>
#define T long long
class RationalDivisionByZero : public std::runtime_error {
 public:
  RationalDivisionByZero() : std::runtime_error("RationalDivisionByZero") {
  }
};
#include <iostream>
T Gcd(T a, T b) {
  while (b) {
    a %= b;
    std::swap(a, b);
  }
  return a;
}
class Rational {
 private:
  T num;
  T den;
 public:
  Rational() : num(0), den(1) {
  }
  Rational(T num) : num(num), den(1) {  
  }                                     
  Rational(T n, T d) {
    if (d == 0) {
      throw RationalDivisionByZero();
    }
    T gcd = Gcd(n, d);
    den = d / gcd;
    num = n / gcd;
  }
  T GetNumerator() const {
    return num;
  }
  T GetDenominator() const {
    return den;
  }
  void SetNumerator(T a) {
    T gcd = Gcd(a, den);
    num = a / gcd;
    den = den / gcd;
  }
  void SetDenominator(T a) {
    T gcd = Gcd(a, num);
    num = num / gcd;
    den = a / gcd;
  }
  void operator+=(const Rational right) {  // const?
    num *= right.den;
    T r = right.num * den;
    num += r;
    T gcd = Gcd(den, num);
    num = num / gcd;
    den = den / gcd;
  }
  void operator-=(const Rational right) {
    num *= right.den;
    T r = right.num * den;
    num -= r;
    T gcd = Gcd(den, num);
    num = num / gcd;
    den = den / gcd;
  }
  void operator*=(const Rational right) {
    num *= right.num;
    den += right.den;
    T gcd = Gcd(den, num);
    num = num / gcd;
    den = den / gcd;
  }
  void operator/=(const Rational right) {
    if (right.num == 0) {
      throw RationalDivisionByZero();
    }
    num *= right.den;
    den += right.num;
    T gcd = Gcd(den, num);
    num = num / gcd;
    den = den / gcd;
  }
  Rational &operator++() {
    num += den;
    return *this;
  }  // версия префикс
  Rational &operator--() {
    num -= den;
    return *this;
  }  // версия префикс
  Rational operator++(int) {
    Rational tmp(this->GetNumerator(), this->GetDenominator());
    ++(*this);
    return tmp;
  }  // версия постфикс
  Rational operator--(int) {
    Rational tmp(this->GetNumerator(), this->GetDenominator());
    --(*this);
    return tmp;
  }  // версия постфикс
  Rational operator-() const {
    return Rational(-num, den);
  }
  Rational operator+() const {
    return Rational(num, den);
  }
};
Rational operator+(const Rational &right, const Rational &left) {
  T n = right.GetNumerator() * left.GetDenominator() + left.GetNumerator() * right.GetDenominator();
  T d = right.GetDenominator() * left.GetDenominator();
  return Rational(n, d);
}
Rational operator-(const Rational &right, const Rational &left) {
  T n = right.GetNumerator() * left.GetDenominator() - left.GetNumerator() * right.GetDenominator();
  T d = right.GetDenominator() * left.GetDenominator();
  return Rational(n, d);
}
Rational operator*(const Rational &right, const Rational &left) {
  T n = right.GetNumerator() * left.GetNumerator();
  T d = right.GetDenominator() * left.GetDenominator();
  return Rational(n, d);
}
Rational operator/(const Rational &right, const Rational &left) {
  T n = right.GetNumerator() * left.GetDenominator();
  T d = right.GetDenominator() * left.GetNumerator();
  if (d == 0) {
    throw RationalDivisionByZero();
  }
  return Rational(n, d);
}
bool operator<(const Rational &right, const Rational &left) {
  T r = right.GetNumerator() * left.GetDenominator();
  T l = right.GetDenominator() * left.GetNumerator();
  return (r < l);
}
bool operator>(const Rational &right, const Rational &left) {
  T r = right.GetNumerator() * left.GetDenominator();
  T l = right.GetDenominator() * left.GetNumerator();
  return (l < r);
}
bool operator<=(const Rational &right, const Rational &left) {
  T r = right.GetNumerator() * left.GetDenominator();
  T l = right.GetDenominator() * left.GetNumerator();
  return (r <= l);
}
bool operator>=(const Rational &right, const Rational &left) {
  T r = right.GetNumerator() * left.GetDenominator();
  T l = right.GetDenominator() * left.GetNumerator();
  return (l >= r);
}
bool operator==(const Rational &right, const Rational &left) {
  T r = right.GetNumerator() *
left.GetDenominator();
  T l = right.GetDenominator() * left.GetNumerator();
  return (r == l);
}
bool operator!=(const Rational &right, const Rational &left) {
  T r = right.GetNumerator() * left.GetDenominator();
  T l = right.GetDenominator() * left.GetNumerator();
  return (r != l);
}
template <typename Stream>
Stream& operator<<(Stream &out, const Rational &n) {
  if (n.GetDenominator() != 1) {
    out << n.GetNumerator() << '/' << n.GetDenominator();
  } else {
    out << n.GetNumerator();
  }
  return out;
}
template <typename Stream>
Stream &operator>>(Stream &in, Rational &r) {
  char c = in.get();
  while (!in.eof() && c != '/') {
    c = in.get();
  }
  return in;
}
