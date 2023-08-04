#include "big_integer.h"

void AppendWithNulls(std::string &s) {
  unsigned int size = s.size();
  if (size < 9) {
    std::string temp = "0";
    for (unsigned int i = 1; i < 9 - size; ++i) {
      temp += '0';
    }
    s = temp + s;
  }
}
int64_t MakeBigDigit(std::string s) {
  int64_t answer = 0;
  int64_t ten = 1;
  for (int i = static_cast<int>(s.size()) - 1; i >= 0; --i) {
    answer += ten * (s[i] - '0');
    ten *= 10;
  }
  return answer;
}

void BigInteger::KillLeadingNulls() {
  while (!body_.empty()) {
    if (body_.back() == 0) {
      body_.pop_back();
    } else {
      break;
    }
  }
  if (body_.empty()) {
    body_.push_back(0);
  }
}
BigInteger::BigInteger() {
  body_.push_back(0);
  sign_ = 0;
}
BigInteger::BigInteger(long long num) {  // NOLINT
  if (num < 0) {
    num *= -1;
    sign_ = -1;
  } else if (num == 0) {
    sign_ = 0;
  } else {
    sign_ = 1;
  }
  do {
    body_.push_back((num % kBase));
    num /= kBase;
  } while (num >= kBase);
  if (num != 0) {
    body_.push_back(num);
  }
}
BigInteger::BigInteger(std::string s) {
  sign_ = 0;
  if (s[0] != '0') {
    sign_ = 1;
  }
  if (s[0] == '-') {
    sign_ = -1;
    s = s.substr(1);
  }
  if (s[0] == '+') {
    s = s.substr(1);
  }
  for (auto i = static_cast<int>(s.length()); i > 0; i -= 9) {
    if (i < 9) {
      body_.push_back(MakeBigDigit(s.substr(0, i)));
    } else {
      body_.push_back(MakeBigDigit(s.substr(i - 9, 9)));
    }
  }
  KillLeadingNulls();
  if (body_.size() > 3334) {
    throw BigIntegerOverflow{};
  }
}
BigInteger::BigInteger(const char *c) {
  sign_ = 0;
  std::string s;
  for (int i = 0; c[i] != '\0'; ++i) {
    s += c[i];
  }
  BigInteger temp(s);
  *this = temp;
}
BigInteger::BigInteger(const BigInteger &num) {
  body_ = num.body_;
  sign_ = num.sign_;
}

const BigInteger BigInteger::operator+() const {
  return BigInteger(*this);
}
const BigInteger BigInteger::operator-() const {
  BigInteger temp(*this);
  temp.sign_ = -temp.sign_;
  return temp;
}
BigInteger &BigInteger::operator+=(const BigInteger &right) {
  if (sign_ == 0) {
    *this = right;
    return *this;
  }
  if (right.sign_ == 0) {
    return *this;
  }
  if (sign_ < right.sign_) {
    BigInteger temp = right;
    temp -= -(*this);
    *this = temp;
    return *this;
  }
  if (sign_ > right.sign_) {
    *this -= -right;
    return *this;
  }

  int64_t carry = 0;
  for (unsigned int i = 0; i < std::max(body_.size(), right.body_.size()) || carry; ++i) {
    if (i == body_.size()) {
      body_.push_back(0);
    }
    body_[i] += carry + (i < right.body_.size() ? right.body_[i] : 0);
    carry = body_[i] >= kBase;
    if (carry != 0) {
      body_[i] -= kBase;
    }
  }
  if (body_.size() > 3334) {
    throw BigIntegerOverflow{};
  }
  return *this;
}
BigInteger &BigInteger::operator-=(const BigInteger &right) {
  if (sign_ == 0) {
    (*this) = -right;
    return *this;
  }
  if (right.sign_ == 0) {
    return *this;
  }
  if (sign_ != right.sign_) {
    *this += -right;
    return *this;
  }
  if (sign_ == -1) {
    BigInteger pos_this = -(*this);
    if (-right >= pos_this) {
      BigInteger temp = -right;
      temp -= pos_this;
      *this = temp;
      return *this;
    }
    pos_this -= -right;
    *this = -pos_this;
    return *this;
  }
  if (sign_ == 1 && *this < right) {
    BigInteger temp = right;
    temp -= *this;
    *this = -temp;
    return *this;
  }

  int64_t carry = 0;
  for (unsigned int i = 0; i < body_.size() || carry; ++i) {
    body_[i] -= carry + (i < right.body_.size() ? right.body_[i] : 0);
    carry = body_[i] < 0;
    if (carry != 0) {
      body_[i] += kBase;
    }
  }
  KillLeadingNulls();
  if (body_[0] == 0) {
    sign_ = 0;
  }

  return *this;
}
BigInteger &BigInteger::operator*=(const BigInteger &right) {
  if (sign_ == 0) {
    return *this;
  }
  if (right.sign_ == 0) {
    sign_ = 0;
    body_ = right.body_;
    return *this;
  }
  std::vector<int64_t> answer(body_.size() + right.body_.size());
  for (unsigned int i = 0; i < body_.size(); ++i) {
    int64_t carry = 0;
    for (unsigned int j = 0; j < right.body_.size() || carry != 0; ++j) {
      int64_t cur = answer[i + j] + body_[i] * (j < right.body_.size() ? right.body_[j] : 0) + carry;
      answer[i + j] = static_cast<int64_t>(cur % kBase);
      carry = static_cast<int64_t>(cur / kBase);
    }
  }
  body_ = answer;
  sign_ *= right.sign_;
  KillLeadingNulls();
  if (body_.size() > 3334) {
    throw BigIntegerOverflow{};
  }

  return *this;
}

BigInteger &BigInteger::operator++() {
  *this += 1;
  return *this;
}
BigInteger &BigInteger::operator--() {
  *this -= 1;
  return *this;
}
BigInteger BigInteger::operator++(int) {
  BigInteger temp = *this;
  ++(*this);
  return temp;
}
BigInteger BigInteger::operator--(int) {
  BigInteger temp = *this;
  --(*this);
  return temp;
}
BigInteger::operator bool() const {
  return sign_ != 0;
}

std::string BigInteger::GetString() const {
  std::string s = std::to_string(body_[body_.size() - 1]);
  if (sign_ == -1) {
    s = '-' + s;
  }
  if (body_.size() == 1) {
    return s;
  }
  std::string temp;
  for (int i = static_cast<int>(body_.size()) - 2; i >= 0; --i) {
    temp = std::to_string(body_[i]);
    AppendWithNulls(temp);
    s += temp;
  }
  return s;
}
int BigInteger::GetSign() const {
  return sign_;
}
int BigInteger::GetLen() const {
  return static_cast<int>(body_.size());
}
int64_t BigInteger::GetBigDigit(int i) const {
  return body_[i];
}
bool BigInteger::IsNegative() const {
  return (sign_ == -1);
}

BigInteger operator+(const BigInteger &left, const BigInteger &right) {
  BigInteger tmp = left;
  tmp += right;
  return tmp;
}
BigInteger operator-(const BigInteger &left, const BigInteger &right) {
  BigInteger tmp = left;
  tmp -= right;
  return tmp;
}
BigInteger operator*(const BigInteger &left, const BigInteger &right) {
  BigInteger tmp = left;
  tmp *= right;
  return tmp;
}

bool operator==(const BigInteger &left, const BigInteger &right) {
  if (left.GetSign() != right.GetSign()) {
    return false;
  }
  if (left.GetSign() == 0) {
    return true;
  }
  if (left.GetLen() != right.GetLen()) {
    return false;
  }
  for (int i = 0; i < left.GetLen(); ++i) {
    if (left.GetBigDigit(i) != right.GetBigDigit(i)) {
      return false;
    }
  }
  return true;
}
bool operator<(const BigInteger &left, const BigInteger &right) {
  if (left == right) {
    return false;
  }
  if (left.GetSign() == -1) {
    if (right.GetSign() == -1) {
      return ((-right) < (-left));
    }
    return true;
  }
  if (right.GetSign() == -1) {
    return false;
  }
  if (left.GetLen() != right.GetLen()) {
    return left.GetLen() < right.GetLen();
  }
  for (int i = left.GetLen() - 1; i >= 0; --i) {
    if (left.GetBigDigit(i) != right.GetBigDigit(i)) {
      return left.GetBigDigit(i) < right.GetBigDigit(i);
    }
  }
  return false;
}
bool operator!=(const BigInteger &left, const BigInteger &right) {
  return !(left == right);
}
bool operator<=(const BigInteger &left, const BigInteger &right) {
  return (left < right || left == right);
}
bool operator>(const BigInteger &left, const BigInteger &right) {
  return !(left <= right);
}
bool operator>=(const BigInteger &left, const BigInteger &right) {
  return !(left < right);
}

std::ostream &operator<<(std::ostream &out, const BigInteger &r) {
  out << r.GetString();
  return out;
}
std::istream &operator>>(std::istream &in, BigInteger &r) {
  std::string s;
  in >> s;
  BigInteger temp(s);
  r = temp;
  return in;
}
