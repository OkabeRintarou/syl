#include <assert.h>
#include <dlist.h>
#include <malloc.h>

struct item_t *new_item(void *data) {
  struct item_t *item = (struct item_t *)malloc(sizeof(struct item_t));
  if (!item) {
    return NULL;
  }
  item->data = data;
  item->prev = item->next = NULL;
  return item;
}

void free_item(struct item_t *item) {
  if (item->data) {
    free(item->data);
  }
  free(item);
}

struct dlist_t *new_dlist() {
  struct dlist_t *list = (struct dlist_t *)malloc(sizeof(struct dlist_t));
  if (!list) {
    return NULL;
  }
  list->head = new_item(NULL);
  if (!list->head) {
    free(list);
    return NULL;
  }

  list->head->prev = list->head->next = list->head;
  return list;
}

struct item_t *dlist_insert_head(struct dlist_t *list, void *data) {
  return dlist_insert_after(list, list->head, data);
}

struct item_t *dlist_insert_tail(struct dlist_t *list, void *data) {
  return dlist_insert_after(list, list->head->prev, data);
}

struct item_t *dlist_insert_after(struct dlist_t *list, struct item_t *item,
                                  void *data) {
  assert(list);
  struct item_t *ni = new_item(data);
  if (!ni) {
    return NULL;
  }
  ni->prev = item;
  ni->next = item->next;
  item->next->prev = ni;
  item->next = ni;
  return ni;
}

struct item_t *dlist_insert_before(struct dlist_t *list, struct item_t *item,
                                   void *data) {
  return dlist_insert_after(list, item->prev, data);
}

void dlist_free(struct dlist_t *list) {
  assert(list);
  struct item_t *cur = list->head->next;
  const struct item_t *const nil = list->head;
  struct item_t *to_free;

  while (cur != nil) {
    to_free = cur;
    cur = cur->next;
    free_item(to_free);
  }

  free(list->head);
  free(list);
}

void dlist_apply(const struct dlist_t *list, void (*func)(void *)) {
  assert(list);
  struct item_t *cur = list->head->next;
  const struct item_t *const nil = list->head;

  while (cur != nil) {
    func(cur->data);
    cur = cur->next;
  }
}

struct item_t *dlist_find(const struct dlist_t *list, int (*cmp)(void *data)) {
  assert(list);
  struct item_t *cur = list->head->next;
  const struct item_t *const nil = list->head;

  while (cur != nil) {
    if (cmp(cur->data)) {
      return cur;
    }
    cur = cur->next;
  }
  return NULL;
}

void dlist_remove(struct item_t *item) {
  assert(item && item->prev && item->next);
  item->prev->next = item->next;
  item->next->prev = item->prev;
}

struct item_t *dlist_get_head(const struct dlist_t *list) {
  if (!dlist_is_empty(list)) {
    return list->head->next;
  }
  return NULL;
}

struct item_t *dlist_get_tail(const struct dlist_t *list) {
  if (!dlist_is_empty(list)) {
    return list->head->prev;
  }
  return NULL;
}

int dlist_is_empty(const struct dlist_t *list) {
  return list->head->prev == list->head;
}

int dlist_size(const struct dlist_t *list) {
  assert(list);
  int size = 0;
  struct item_t *cur = list->head->next;
  const struct item_t *const nil = list->head;

  while (cur != nil) {
    ++size;
    cur = cur->next;
  }

  return size;
}
