#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct item_t {
  void *data;
  struct item_t *prev;
  struct item_t *next;
};

struct item_t *new_item(void *data);

void free_item(struct item_t *item);

struct dlist_t {
  struct item_t *head;
};

struct dlist_t *new_dlist();

struct item_t *dlist_insert_head(struct dlist_t *list, void *data);

struct item_t *dlist_insert_tail(struct dlist_t *list, void *data);

struct item_t *dlist_insert_after(struct dlist_t *list, struct item_t *item,
                                  void *data);

struct item_t *dlist_insert_before(struct dlist_t *list, struct item_t *item,
                                   void *data);

void dlist_free(struct dlist_t *list);

void dlist_apply(const struct dlist_t *list, void (*func)(void *));

struct item_t *dlist_find(const struct dlist_t *list, int (*cmp)(void *));

void dlist_remove(struct item_t *item);

struct item_t *dlist_get_head(const struct dlist_t *list);

struct item_t *dlist_get_tail(const struct dlist_t *list);

int dlist_is_empty(const struct dlist_t *list);

int dlist_size(const struct dlist_t *list);

#ifdef __cplusplus
}
#endif
