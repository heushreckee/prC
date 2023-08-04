#ifndef UNTITLED_HEAP_H
#define UNTITLED_HEAP_H

#include <iostream>

template <typename T>

void Swap(T &a, T &b) {
  T temp = a;
  a = b;
  b = temp;
}

template <typename T>

void SiftDown(T *begin, int i, const int &size) {
  int left = 2 * i + 1;
  int right = 2 * i + 2;
  int largest = i;

  if (left < size && (*(begin + largest) < *(begin + left))) {
    largest = left;
  }

  if (right < size && (*(begin + largest) < *(begin + right))) {
    largest = right;
  }

  if (largest != i) {
    Swap(*(begin + i), *(begin + largest));
    SiftDown(begin, largest, size);
  }
}

template <typename T>

void SiftUp(int i, T *begin) {
  int parent = (i - 1) / 2;
  if (0 < i && *(begin + parent) < *(begin + i)) {
    Swap(*(begin + i), *(begin + parent));
    SiftUp(parent, begin);
  }
}

template <typename T>

void PopHeap(T *begin, T *end) {
  Swap(*(begin), *(end - 1));
  SiftDown(begin, 0, end - begin - 1);
}

template <typename T>

void PushHeap(T *begin, T *end) {
  SiftUp(end - begin - 1, begin);
}

#endif  // UNTITLED_HEAP_H
