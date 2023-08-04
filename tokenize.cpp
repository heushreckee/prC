#include "tokenize.h"

bool operator==(const UnknownToken &r, const UnknownToken &l) {
  return r.value == l.value;
}

bool operator==(const NumberToken &r, const NumberToken &l) {
  return r.value == l.value;
}

bool operator==(const EmptyToken &, const EmptyToken &) {
  return true;
}
