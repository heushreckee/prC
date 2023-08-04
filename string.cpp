#include "cppstring.h"
void String::Remake(size_t length) {
  if (capacity_ > length) {
    return;
  }
  length *= 2;
  auto* temp_str = new char[length];
  capacity_ = length;
  if (str_ == nullptr) {
    size_ = 0;
    str_ = temp_str;
    return;
  }
  memcpy(temp_str, str_, size_);
  delete[] str_;
  str_ = temp_str;
}
String::String() {
  capacity_ = 0;
  size_ = 0;
  str_ = nullptr;
}
String::String(int n, char s) {
  if (n == 0) {
    size_ = 0;
    capacity_ = 0;
    str_ = nullptr;
    return;
  }
  size_ = n;
  capacity_ = size_;
  str_ = new char[capacity_];
  memset(str_, s, size_);
}
String::String(const char* s) {
  size_ = strlen(s);
  capacity_ = size_;
  if (size_ == 0) {
    str_ = nullptr;
    return;
  }
  str_ = new char[capacity_];
  memcpy(str_, s, size_);
}
String::String(const char* s, int n) {
  size_ = n;
  capacity_ = size_;
  if (size_ == 0) {
    str_ = nullptr;
    return;
  }
  str_ = new char[capacity_];
  memcpy(str_, s, size_);
}
String::String(const String& s) {
  capacity_ = s.capacity_;
  size_ = s.size_;
  if (s.str_ == nullptr) {
    delete[] str_;
    str_ = nullptr;
    return;
  }
  delete[] str_;
  str_ = new char[capacity_];
  memcpy(str_, s.str_, size_);
}
String& String::operator=(const String& s) {
  if (&s == this) {
    return *this;
  }
  capacity_ = s.capacity_;
  size_ = s.size_;
  if (s.str_ == nullptr) {
    delete[] str_;
    str_ = nullptr;
    return *this;
  }
  delete[] str_;
  str_ = new char[capacity_];
  memcpy(str_, s.str_, size_);
  return *this;
}
String::~String() {
  delete[] str_;
  str_ = nullptr;
}
const char& String::operator[](size_t index) const {
  return str_[index];
}
char& String::operator[](size_t index) {
  return str_[index];
}
const char& String::At(size_t index) const {
  if (index >= size_) {
    throw StringOutOfRange{};
  }
  return str_[index];
}
char& String::At(size_t index) {
  if (index >= size_) {
    throw StringOutOfRange{};
  }
  return str_[index];
}
const char& String::Front() const {
  return str_[0];
}
char& String::Front() {
  return str_[0];
}
const char& String::Back() const {
  return str_[size_ - 1];
}
char& String::Back() {
  return str_[size_ - 1];
}
const char* String::CStr() const {
  return str_;
}
char* String::CStr() {
  return str_;
}
const char* String::Data() const {
  if (size_ == 0) {
    return nullptr;
  }
  return str_;
}
char* String::Data() {
  if (size_ == 0) {
    return nullptr;
  }
  return str_;
}
bool String::Empty() const {
  return size_ == 0;
}
size_t String::Size() const {
  return size_;
}
size_t String::Length() const {
  return size_;
}
size_t String::Capacity() const {
  return capacity_;
}
void String::Clear() {
  size_ = 0;
}
void String::Swap(String& other) {
  String tmp = other;
  other = *(this);
  *(this) = tmp;
}
void String::PushBack(char c) {
  if (capacity_ == size_) {
    Remake(capacity_ + 1);
  }
  str_[size_] = c;
  ++size_;
}
void String::PopBack() {
  --size_;
}
String& String::operator+=(const String& s) {
  if (s.size_ == 0) {
    return *this;
  }
  Remake(size_ + s.Size());
  memcpy(str_ + size_, s.str_, s.Size());
  size_ += s.Size();
  return *this;
}
void String::ShrinkToFit() {
  capacity_ = size_;
  auto* temp = new char[size_];
  memcpy(temp, str_, size_);
  delete[] str_;
  str_ = temp;
}
void String::Resize(size_t new_size, char symbol) {
  if (new_size > capacity_) {
    capacity_ = 2 * new_size;
    if (str_ == nullptr) {
      str_ = new char[capacity_];
    }
    memset(str_ + size_, symbol, new_size - size_);
  } else if (size_ < new_size) {
    memset(str_ + size_, symbol, new_size - size_);
  }
  size_ = new_size;
}
void String::Reserve(size_t n) {
  n = std::max(capacity_, n);
  auto* temp = new char[n];
  memcpy(temp, str_, size_);
  delete[] str_;
  str_ = temp;
  capacity_ = n;
}
bool operator<(const String& a, const String& b) {
  if (a.Empty() && b.Empty()) {
    return false;
  }
  if (a.Empty()) {
    return true;
  }
  if (b.Empty()) {
    return false;
  }
  size_t n = std::min(b.Size(), a.Size());
  size_t i = 0;
  bool flag = false;
  for (; i < n; ++i) {
    if (a.Data()[i] != b.Data()[i]) {
      flag = true;
      break;
    }
  }
  if (!flag && b.Size() <= a.Size()) {
    return false;
  }
  if (!flag && b.Size() > a.Size()) {
    return true;
  }
  return a.Data()[i] < b.Data()[i];
}
bool operator<=(const String& a, const String& b) {
  return (a < b || a == b);
}
bool operator>(const String& a, const String& b) {
  return !(a <= b);
}
bool operator>=(const String& a, const String& b) {
  return !(a < b);
}
bool operator!=(const String& a, const String& b) {
  return !(a == b);
}
bool operator==(const String& a, const String& b) {
  if (a.Size() != b.Size()) {
    return false;
  }
  if (a.Data() == nullptr) {
    return true;
  }
  for (size_t i = 0; i < a.Size(); ++i) {
    if (a.Data()[i] != b.Data()[i]) {
      return false;
    }
  }
  return true;
}
String operator+(const String& left, const String& right) {
  String res = left;
  res += right;
  return res;
}
std::ostream& operator<<(std::ostream& out, const String& s) {
  for (size_t i = 0; i < s.Size(); ++i) {
    out << s[i];
  }
  return out;
}
