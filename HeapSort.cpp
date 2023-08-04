#include <iostream>

void SiftDown(int *sa, int size, int i) {
  while (2 * i + 1 < size) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int j = left;
    if (right < size && sa[right] < sa[left]) {
      j = right;
    }
    if (sa[i] <= sa[j]) {
      break;
    }
    std::swap(sa[i], sa[j]);
    i = j;
  }
}

int ExtractMin(int *sa, int &size) {
  int x = sa[0];
  sa[0] = sa[size - 1];
  --size;
  SiftDown(sa, size, 0);
  return x;
}

void HeapSort(int *sa, int *as, int size) {
  for (int i = size / 2; i >= 0; --i) {
    SiftDown(sa, size, i);
  }
  int t = size;
  for (int i = 0; i < t; ++i) {
    as[i] = ExtractMin(sa, size);
  }
  delete[] sa;
}

int main() {
  int n = 0;
  std::cin >> n;
  int *sa = new int[n];
  int *as = new int[n];

  for (int i = 0; i < n; ++i) {
    std::cin >> sa[i];
  }
  HeapSort(sa, as, n);
  for (int i = 0; i < n; ++i) {
    std::cout << as[i] << ' ';
  }
  delete[] as;
}
