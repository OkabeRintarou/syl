#include <cassert>
#include <stdio.h>
#include "dlist.h"

int main() {
	DoubleLinkedList<int> L;
	for (int i = 0; i < 10; i++) {
		L.add(i);
		assert(L.size() == i + 1);
	}
	for (int i = 9; i >= 0; i--) {
		L.remove(i);
		assert(L.size() == i);
	}
	assert(L.is_empty());
	return 0;
}
