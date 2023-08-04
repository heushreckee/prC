#include <iostream>

struct Node {
    int key;
    Node *left;
    Node *right;
};

class Tree {
    Node *root = nullptr;

    Node *newNode(int key) {
      Node *node = new Node();
      node->key = key;
      node->left = node->right = nullptr;
      return (node);
    }

    Node *rightRotate(Node *x) {
      Node *y = x->left;
      x->left = y->right;
      y->right = x;
      return y;
    }

    Node *leftRotate(Node *x) {
      Node *y = x->right;
      x->right = y->left;
      y->left = x;
      return y;
    }
    
    Node *splay(Node *root, int key) {
      if (root == nullptr || root->key == key)
        return root;
      if (root->key > key) {
        if (root->left == nullptr) return root;
        if (root->left->key > key) {
          root->left->left = splay(root->left->left, key);
          root = rightRotate(root);
        } else if (root->left->key < key) {
          root->left->right = splay(root->left->right, key);
          if (root->left->right != nullptr)
            root->left = leftRotate(root->left);
        }
        return (root->left == nullptr) ? root : rightRotate(root);
      } else {
        if (root->right == nullptr) return root;
        if (root->right->key > key) {
          root->right->left = splay(root->right->left, key);
          if (root->right->left != nullptr)
            root->right = rightRotate(root->right);
        } else if (root->right->key < key) {
          root->right->right = splay(root->right->right, key);
          root = leftRotate(root);
        }
        return (root->right == nullptr) ? root : leftRotate(root);
      }
    }
    
    int FindMin(int x) {
      int temp = -1;
      for (Node *node = root; node != nullptr;) {
        if (node->key == x) {
          root = splay(root, x);
          return x;
        }
        if (x < node->key) {
          temp = node->key;
          node = node->left;
        } else {
          node = node->right;
        }
      }
      root = splay(root, temp);
      return temp;
    }

    void preorder(Node *root) {
      if (root != nullptr) {
        std::cout << root->key << " ";
        preorder(root->left);
        preorder(root->right);
      }
    }

    Node *Insert(Node *root, int k) {
      if (root == nullptr) {
        return newNode(k);
      }
      root = splay(root, k);
      if (root->key == k) return root;
      Node *tmp = newNode(k);
      if (root->key > k) {
        tmp->right = root;
        tmp->left = root->left;
        root->left = nullptr;
      } else {
        tmp->left = root;
        tmp->right = root->right;
        root->right = nullptr;
      }
      return tmp;
    }

    void Clear(Node *t) {
      if (t != nullptr) {
        Clear(t->left);
        Clear(t->right);
        delete t;
      }
    }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n;
  std::cin >> n;
  char sa;
  bool flag = true;
  Tree t;
  int x;
  int y = -1;
  for (int i = 0; i < n; ++i) {
    std::cin >> sa;
    if (sa == '+') {
      if (flag) {
        std::cin >> x;
        t.root = t.Insert(t.root, x);
      } else {
        std::cin >> x;
        x = (x + y) % 1000000000;
        t.root = t.Insert(t.root, x);
        flag = true;
      }
    } else {
      flag = false;
      std::cin >> x;
      y = t.FindMin(x);
      std::cout << y << '\n';
    }
  }
  //t.preorder(t.root);
  t.Clear(t.root);
}
