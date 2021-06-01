#pragma once

#include <cassert>
#include <cstddef>

template<typename T>
class DoubleLinkedList {
private:
	template<typename N>
	class Node {
	public:
		Node() : val() {
			prev = this;
			next = this;
		}

		explicit Node(N v) : val(v) {
			prev = this;
			next = this;
		}
		Node *prev;
		Node *next;
		N val;
	};


	Node<T> head;
	size_t size_ {0};
private:
	void remove(Node<T> *n);
	Node<T> *find(const T &v);
public:
	void add(T v);
	bool remove(T v);
	bool is_empty() const {
		return head.next == &head;
	}
	size_t size() const {
		return size_;
	}
};

template<typename T>
void DoubleLinkedList<T>::add(T v) {
	Node<T> *newn = new Node<T>(v);
	Node<T> **ppFirst = &this->head.next;

	newn->prev = &head;
	(newn->next = *ppFirst)->prev = newn;
	*ppFirst = newn;
	++size_;
}

template<typename T>
bool DoubleLinkedList<T>::remove(T v) {
	Node<T> *n = find(v);
	if (n != nullptr) {
		--size_;
		remove(n);
		return true;
	}
	return false;
}

template<typename T>
void DoubleLinkedList<T>::remove(Node<T> *n) {
	assert(n != nullptr);
	assert(n->next != nullptr && n->prev != nullptr);

	n->prev->next = n->next;
	n->next->prev = n->prev;
}

template<typename T>
DoubleLinkedList<T>::Node<T> *DoubleLinkedList<T>::find(const T &v) {
	Node<T> *cur = head.next;
	while (cur != &head) {
		if (v == cur->val) {
			return cur;
		}
		cur = cur->next;
	}
	return nullptr;
}
