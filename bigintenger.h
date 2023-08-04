#ifndef OOP_ASSIGNMENTS_big_integer_H
#define OOP_ASSIGNMENTS_big_integer_H

#include <iostream>
#include <vector>
#include <stdexcept>

class BigIntegerOverflow : public std::runtime_error {
 public:
  BigIntegerOverflow() : std::runtime_error("BigIntegerOverflow") {
  }
};

void AppendWithNulls(std::string &s);
int64_t MakeBigDigit(std::string s);

static const int64_t kBase = 1000000000;
class BigInteger {
 private:
  std::vector<int64_t> body_;
  int sign_;
  void KillLeadingNulls();

 public:
  BigInteger();
  BigInteger(long long num);          //  NOLINT
  explicit BigInteger(std::string s);
  explicit BigInteger(const char *c);
  BigInteger(const BigInteger &num);
  BigInteger &operator=(const BigInteger &num) = default;
  ~BigInteger() = default;

  const BigInteger operator+() const;
  const BigInteger operator-() const;
  BigInteger &operator+=(const BigInteger &right);
  BigInteger &operator-=(const BigInteger &right);
  BigInteger &operator*=(const BigInteger &right);
  BigInteger &operator++();
  BigInteger &operator--();
  BigInteger operator++(int);
  BigInteger operator--(int);
  explicit operator bool() const;    //  NOLINT

  std::string GetString() const;     //  NOLINT
  int GetSign() const;               //  NOLINT
  int GetLen() const;                //  NOLINT
  int64_t GetBigDigit(int i) const;  //  NOLINT
  bool IsNegative() const;           //  NOLINT
};

BigInteger operator+(const BigInteger &left, const BigInteger &right);
BigInteger operator-(const BigInteger &left, const BigInteger &right);
BigInteger operator*(const BigInteger &left, const BigInteger &right);
bool operator==(const BigInteger &left, const BigInteger &right);
bool operator<(const BigInteger &left, const BigInteger &right);
bool operator!=(const BigInteger &left, const BigInteger &right);
bool operator<=(const BigInteger &left, const BigInteger &right);
bool operator>(const BigInteger &left, const BigInteger &right);
bool operator>=(const BigInteger &left, const BigInteger &right);

std::ostream &operator<<(std::ostream &out, const BigInteger &r);
std::istream &operator>>(std::istream &in, BigInteger &r);

#endif  // OOP_ASSIGNMENTS_big_integer_H
