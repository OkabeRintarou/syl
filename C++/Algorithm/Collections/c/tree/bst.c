#include <assert.h>
#include <malloc.h>
#include <stdio.h>

typedef struct node_ {
  int key;
  struct node_ *lhs;
  struct node_ *rhs;
} node;

node *_new_node(int k) {
  node *n = (node *)malloc(sizeof(node));
  n->key = k;
  n->lhs = n->rhs = NULL;
  return n;
}

typedef struct bst_ { node *root; } bst;

bst *new_bst() {
  bst *t = (bst *)malloc(sizeof(bst));
  t->root = NULL;
  return t;
}

void bst_insert(bst *tree, int key) {
  node **r = &tree->root;
  node *pn;

  while ((pn = *r)) {
    if (pn->key == key)
      return;
    else if (pn->key < key) {
      r = &pn->rhs;
    } else {
      r = &pn->lhs;
    }
  }

  *r = _new_node(key);
}

void bst_remove(bst *tree, int key) {
  node **cur = &tree->root;
  node *pn;

  while ((pn = *cur)) {
    if (pn->key == key) {
      node *to_free = pn;
      if (pn->lhs && pn->rhs) {
        cur = &pn->rhs;
        while ((*cur)->lhs) {
          cur = &(*cur)->lhs;
        }
        pn->key = (*cur)->key;
        to_free = *cur;
        *cur = NULL;
      } else {
        *cur = pn->lhs ? pn->lhs : pn->rhs;
      }
      free(to_free);
      break;
    } else if (pn->key < key) {
      cur = &pn->rhs;
    } else {
      cur = &pn->lhs;
    }
  }
}

int bst_is_empty(bst *tree) { return !tree->root; }

int main(int argc, char *argv[]) {
  bst *tree = new_bst();
  bst_insert(tree, 1);
  bst_insert(tree, 4);
  bst_insert(tree, 3);
  bst_remove(tree, 4);
  bst_remove(tree, 3);
  bst_remove(tree, 1);
  free(tree);
  return 0;
}
