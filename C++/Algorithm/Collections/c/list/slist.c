#include <malloc.h>
#include <stdio.h>

struct slist_node_ {
  int value;
  struct slist_node_ *next;
};

struct slist_node_ *_new_node(int val) {
  struct slist_node_ *node =
      (struct slist_node_ *)malloc(sizeof(struct slist_node_));
  node->value = val;
  node->next = NULL;
  return node;
}

typedef struct slist_ { struct slist_node_ *head; } slist;

slist *new_slist() {
  slist *list = (slist *)malloc(sizeof(slist));
  list->head = NULL;
  return list;
}

void slist_insert(slist *list, int val) {
  struct slist_node_ *new_node = _new_node(val);
  new_node->next = list->head;
  list->head = new_node;
}

void slist_remove(slist *list, int (*rm)(int)) {
  struct slist_node_ **cur;
  struct slist_node_ *entry;

  for (cur = &list->head; *cur;) {
    entry = *cur;
    if (rm(entry->value)) {
      *cur = entry->next;
      free(entry);
    } else {
      cur = &entry->next;
    }
  }
}

void slist_apply(slist *list, void (*fn)(int)) {
  struct slist_node_ *cur;
  for (cur = list->head; cur; cur = cur->next) {
    fn(cur->value);
  }
}

void print(int val) { printf("%d ", val); }

int remove_odd(int val) { return val & 1; }

int remove_all(int val) { return 1; }

int main(int argc, char *argv[]) {
  slist *list = new_slist();
  int i;
  for (i = 0; i < 100; i++) {
    slist_insert(list, i);
  }
  slist_apply(list, print);
  putchar('\n');
  slist_remove(list, remove_odd);
  slist_apply(list, print);
  putchar('\n');
  slist_remove(list, remove_all);
  slist_apply(list, print);
  putchar('\n');
  free(list);
  return 0;
}
