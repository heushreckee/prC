#ifndef OOP_ASSIGNMENTS_TOKENIZE_H
#define OOP_ASSIGNMENTS_TOKENIZE_H

#include <iostream>
#include <variant>
#include <string>
#include <type_traits>
#include <vector>
#include <cmath>
#include <string>

struct EmptyToken {
  virtual ~EmptyToken() = default;
};

struct PlusToken : EmptyToken {};
struct MinusToken : EmptyToken {
  MinusToken() = default;
};
struct MultiplyToken : EmptyToken {};
struct DivideToken : EmptyToken {};
struct ResidualToken : EmptyToken {};
struct OpeningBracketToken : EmptyToken {};
struct ClosingBracketToken : EmptyToken {};
struct SqrToken : EmptyToken {};
struct MaxToken : EmptyToken {};
struct MinToken : EmptyToken {};
struct AbsToken : EmptyToken {};
struct NumberToken {
  int value;
  NumberToken(const int &val) : value(val) {  // NOLINT
  }
  NumberToken(const std::string &s) {  // NOLINT
    value = std::stoi(s);
  }
  NumberToken() = default;
  ~NumberToken() = default;
};
struct UnknownToken {
  std::string value;
  UnknownToken(const std::string &s) : value(s) {  // NOLINT
  }
  UnknownToken() = default;
  ~UnknownToken() = default;
};

bool operator==(const UnknownToken &r, const UnknownToken &l);

bool operator==(const NumberToken &r, const NumberToken &l);

bool operator==(const EmptyToken &, const EmptyToken &);

using Token = std::variant<PlusToken, MinusToken, MultiplyToken, DivideToken, ResidualToken, OpeningBracketToken,
                           ClosingBracketToken, SqrToken, MaxToken, MinToken, AbsToken, NumberToken, UnknownToken>;

inline bool IsItNumber(const char &ch) {
  return ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == '8' ||
         ch == '9' || ch == '0';
}

inline std::vector<Token> Tokenize(const std::string &s) {
  std::vector<Token> v;
  if (s.empty()) {
    return v;
  }
  uint64_t co = 0;
  while (s[co] == ' ') {
    ++co;
  }
  if (co == s.size()) {
    return v;
  }

  for (uint64_t i = 0; i < s.size(); ++i) {
    if (s[i] == ' ') {
      continue;
    }

    if (s[i] == '-') {
      if (s[i + 1] == ' ' || (i + 1) == s.size()) {
        MinusToken a;
        v.emplace_back(a);
        std::cerr << "-";
        continue;
      }
      if (IsItNumber(s[i + 1])) {
        ++i;
        std::string str = "-";
        while (IsItNumber(s[i]) && i < s.size()) {
          str += s[i];
          ++i;
        }
        auto a = NumberToken(str);
        std::cerr << str << '\n';
        v.emplace_back(a);
        continue;
      }
    }

    if (s[i] == '+') {
      if (s[i + 1] == ' ' || (i + 1) == s.size()) {
        PlusToken a;
        v.emplace_back(a);
        std::cerr << "+";
        continue;
      }
      if (IsItNumber(s[i + 1])) {
        ++i;
        std::string str;
        while (IsItNumber(s[i]) && i < s.size()) {
          str += s[i];
          ++i;
        }
        auto a = NumberToken(str);
        std::cerr << str << '\n';
        v.emplace_back(a);
        continue;
      }
    }

    if (s[i] == '/' && (s[i + 1] == ' ' || (i + 1) == s.size())) {
      DivideToken a;
      v.emplace_back(a);
      std::cerr << "/";
      continue;
    }

    if (s[i] == '*' && (s[i + 1] == ' ' || (i + 1) == s.size())) {
      MultiplyToken a;
      v.emplace_back(a);
      std::cerr << "*";
      continue;
    }

    if (s[i] == '%' && (s[i + 1] == ' ' || (i + 1) == s.size())) {
      ResidualToken a;
      v.emplace_back(a);
      std::cerr << "%";
      continue;
    }

    if (s[i] == '(' && (s[i + 1] == ' ' || (i + 1) == s.size())) {
      OpeningBracketToken a;
      v.emplace_back(a);
      std::cerr << "(";
      continue;
    }

    if (s[i] == ')' && (s[i + 1] == ' ' || (i + 1) == s.size())) {
      ClosingBracketToken a;
      v.emplace_back(a);
      std::cerr << ")";
      continue;
    }

    if (s[i] == 's') {
      if (s[i + 1] == 'q') {
        if (s[i + 2] == 'r' && (s[i + 3] == ' ' || (i + 3) == s.size())) {
          SqrToken a;
          v.emplace_back(a);
          i += 2;
          std::cerr << "sqr";
          continue;
        }
        std::string str = "sq";
        i += 2;
        while (s[i] != ' ' && i < s.size()) {
          str += s[i];
          ++i;
        }
        auto a = UnknownToken(str);
        std::cerr << "str";
        v.emplace_back(a);
        continue;
      }
      std::string str = "s";
      ++i;
      while (s[i] != ' ' && i < s.size()) {
        str += s[i];
        ++i;
      }
      auto a = UnknownToken(str);
      std::cerr << "str";
      v.emplace_back(a);
      continue;
    }

    if (s[i] == 'a') {
      if (s[i + 1] == 'b') {
        if (s[i + 2] == 's' && (s[i + 3] == ' ' || (i + 3) == s.size())) {
          AbsToken a;
          v.emplace_back(a);
          std::cerr << "abs";
          i += 2;
          continue;
        }
        std::string str = "ab";
        i += 2;
        while (s[i] != ' ' && i < s.size()) {
          str += s[i];
          ++i;
        }
        auto a = UnknownToken(str);
        std::cerr << "str";
        v.emplace_back(a);
        continue;
      }
      std::string str = "a";
      ++i;
      while (s[i] != ' ' && i < s.size()) {
        str += s[i];
        ++i;
      }
      auto a = UnknownToken(str);
      std::cerr << "str";
      v.emplace_back(a);
      continue;
    }

    if (s[i] == 'm') {
      if (s[i + 1] == 'a') {
        if (s[i + 2] == 'x' && (s[i + 3] == ' ' || (i + 3) == s.size())) {
          MaxToken a;
          v.emplace_back(a);
          i += 2;
          std::cerr << "max";
          continue;
        }
        std::string str = "ma";
        i += 2;
        while (s[i] != ' ' && i < s.size()) {
          str += s[i];
          ++i;
        }
        auto a = UnknownToken(str);
        std::cerr << "str";
        v.emplace_back(a);
        continue;
      }
      if (s[i + 1] == 'i') {
        if (s[i + 2] == 'n' && (s[i + 3] == ' ' || (i + 3) == s.size())) {
          MinToken a;
          v.emplace_back(a);
          i += 2;
          std::cerr << "min";
          continue;
        }
        std::string str = "mi";
        i += 2;
        while (s[i] != ' ' && i < s.size()) {
          str += s[i];
          ++i;
        }
        auto a = UnknownToken(str);
        std::cerr << "str";
        v.emplace_back(a);
        continue;
      }
      std::string str = "m";
      ++i;
      while (s[i] != ' ' && i < s.size()) {
        str += s[i];
        ++i;
      }
      auto a = UnknownToken(str);
      v.emplace_back(a);
      std::cerr << "str";
      continue;
    }

    if (IsItNumber(s[i])) {
      std::string str;
      while (IsItNumber(s[i])) {
        str += s[i];
        ++i;
      }
      if (s[i] == ' ') {
        auto a = NumberToken(str);
        std::cerr << str << '\n';
        v.emplace_back(a);
        continue;
      }
      while (s[i] != ' ' && i < s.size()) {
        str += s[i];
        ++i;
      }
      auto a = UnknownToken(str);
      v.emplace_back(a);
      std::cerr << "str";
      continue;
    }

    std::string str;
    while (s[i] != ' ' && i < s.size()) {
      str += s[i];
      ++i;
    }
    auto a = UnknownToken(str);
    v.emplace_back(a);
    std::cerr << str << '\n';
    std::cerr << "u";
  }
  return v;
}

#endif  // OOP_ASSIGNMENTS_TOKENIZE_H
