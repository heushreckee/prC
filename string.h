#ifndef OOP_ASSIGNMENTS_D_H
#define OOP_ASSIGNMENTS_D_H

#include <iostream>
#include <cstring>
#include <stdexcept>
class StringOutOfRange : public std::out_of_range {
 public:
  StringOutOfRange() : std::out_of_range("StringOutOfRange") {
  }
};

class String {
 private:
  size_t capacity_;
  size_t size_;
  char* str_ = nullptr;
  void Remake(size_t length);

 public:
  String();
  String(int n, char s);
  String(const char* s);  // NOLINT
  String(const char* s, int n);
  String(const String& s);
  ~String();
  String& operator=(const String& s);
  const char& operator[](size_t index) const;
  char& operator[](size_t index);
  const char& At(size_t index) const;
  char& At(size_t index);
  const char& Front() const;
  char& Front();
  const char& Back() const;
  char& Back();
  const char* Data() const;
  char* Data();
  const char* CStr() const;
  char* CStr();
  bool Empty() const;
  size_t Size() const;
  size_t Length() const;
  size_t Capacity() const;
  void Clear();
  void Swap(String& other);
  void PushBack(char c);
  void PopBack();
  String& operator+=(const String& s);
  void ShrinkToFit();
  void Resize(size_t new_size, char symbol);
  void Reserve(size_t n);
};
bool operator<(const String& a, const String& b);
bool operator<=(const String& a, const String& b);
bool operator>(const String& a, const String& b);
bool operator>=(const String& a, const String& b);
bool operator!=(const String& a, const String& b);
bool operator==(const String& a, const String& b);
String operator+(const String& left, const String& right);
std::ostream& operator<<(std::ostream& out, const String& s);
#endif  // OOP_ASSIGNMENTS_D_H
