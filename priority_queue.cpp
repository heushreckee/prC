#include <iostream>

void Swap(int &a, int &b) {
  int temp = a;
  a = b;
  b = temp;
}

class PQueue {
    int *arr;
    int size = 0;

    int Parent(int i) {
      return (i - 1) / 2;
    }

    int LeftSon(int i) {
      return 2 * i + 1;
    }

    int RightSon(int i) {
      return 2 * i + 2;
    }

    void SiftDown(int i, int &new_i, int size) {
      int left = LeftSon(i);
      int right = RightSon(i);
      int largest = i;

      if (left < size && (arr[left] > arr[largest])) {
        largest = left;
      }

      if (right < size && (arr[right] > arr[largest])) {
        largest = right;
      }

      if (largest != i) {
        Swap(arr[i], arr[largest]);
        new_i = largest;
        SiftDown(largest, new_i, size);
      }
    }

    void SiftUp(int i, int &new_i) {
      int parent = Parent(i);
      if (i > 0 && arr[parent] < arr[i]) {
        new_i = parent;
        Swap(arr[i], arr[parent]);
        SiftUp(parent, new_i);
      }
    }

    const int &ExtractMax(int size, int &idx) {
      Swap(arr[0], arr[size - 1]);
      SiftDown(0, idx, size - 1);
      return arr[size - 1];
    }

    void Delete(int i) {
      int tmp;
      arr[i] = arr[size - 1];
      --size;
      SiftDown(0, tmp, size);
      SiftUp(size - 1, tmp);
      std::cout << i << '\n';
    }
};

int main() {
  int m, n;
  std::cin >> m >> n;
  PQueue q;
  q.arr = new int[m];
  int sa;
  int idx, num;
  for (int i = 0; i < n; ++i) {
    std::cin >> sa;
    if (sa == 1) {
      if (q.size == 0) {
        std::cout << -1 << '\n';
      } else {
        idx = 0;
        int max = q.ExtractMax(q.size, idx);
        --q.size;
        if (q.size == 0) {
          std::cout << 0 << ' ' << max << '\n';
        } else {
          std::cout << idx + 1 << ' ' << max << '\n';
        }
      }
    } else if (sa == 2) {
      std::cin >> num;
      if (q.size == m) {
        std::cout << -1 << '\n';
      } else {
        q.arr[q.size] = num;
        idx = q.size;
        q.SiftUp(q.size, idx);
        ++q.size;
        std::cout << idx + 1 << '\n';
      }
    }
    if (sa == 3) {
      int r;
      std::cin >> r;
      if (q.size < r) {
        std::cout << q.arr[r - 1] << '\n';
        q.Delete(r - 1);
      } else {
        std::cout << -1 << '\n';
      }
    }
  }
  for (int i = 0; i < q.size; ++i) {
    std::cout << q.arr[i] << ' ';
  }
  delete[] q.arr;
}
