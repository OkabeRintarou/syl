#include <stdio.h>
#include <malloc.h>

#define MaxSkipListLevel  16

static int const FALSE = 0;
static int const TRUE = 1;
typedef int boolean;
typedef int KeyType;
typedef int ValueType;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

static uint32_t seed = 1;
static uint32_t A = 48271;
static uint32_t M = 0x7fffffff;

uint32_t static srand(uint32_t newseed)
{
	seed = newseed;
	if (seed == 0 || seed == M){
		seed = 1;
	}

	return seed;
}

uint32_t static rand()
{
	// Compute seed = (A * seed ) mod M
	uint64_t product = A * seed;
	// Compute product mod M,use the fact as follow:
	// ((x << 31) % M) ==  x
	seed = (uint32_t)((product >> 31) + (product & M));

	// the result may overflow 1 bit
	if (seed > M){
		seed -= M;
	}

	return seed;
}

// return in range[1,MaxSkipListLevel]
uint32_t static randomLevel()
{
	return rand() % MaxSkipListLevel + 1;
}

struct nodeStructure
{
	KeyType key;
	ValueType value;

	struct nodeStructure * forward[1];
};

struct skiplistStructure
{
	int current_max_level;

	struct nodeStructure * head;
};

typedef struct nodeStructure * node;
typedef struct skiplistStructure * list;

node createNode(int level, KeyType key, ValueType value)
{
	node newnode = (node)malloc(sizeof(struct nodeStructure) + level * sizeof(node));

	newnode->key = key;
	newnode->value = value;

	return newnode;
}

// create an empty skiplist
static struct nodeStructure Nil;
list createSkipList()
{
	list newlist = (list)malloc(sizeof(struct skiplistStructure));

	newlist->current_max_level = 0;

	newlist->head = createNode(MaxSkipListLevel, 0, 0);

	for (int i = 0; i < MaxSkipListLevel; ++i){
		newlist->head->forward[i] = &Nil;
	}

	return newlist;
}

boolean insert(list ls, KeyType k, ValueType v)
{
	int n = ls->current_max_level;
	node p, q;
	node update[MaxSkipListLevel];
	p = ls->head;

	do{
		q = p->forward[n];
		while (q != &Nil && q->key < k){
			p = q;
			q = p->forward[n];
		}
			
		update[n] = p;
	} while (--n >= 0);

	if (q && q->key == k){
		return FALSE;
	}

	n = randomLevel();
	if (n > ls->current_max_level){
		n = ++(ls->current_max_level);
		update[n] = ls->head;
	}

	q = createNode(n, k, v);

	do{
		p = update[n];
		q->forward[n] = p->forward[n];
		p->forward[n] = q;
	} while (--n >= 0);

	return TRUE;
}

boolean delete(list ls, KeyType k)
{
	int n = ls->current_max_level;
	int m = n;
	node p, q;
	node update[MaxSkipListLevel];

	p = ls->head;
	do{
		q = p->forward[n];
		while (q != &Nil && q->key < k){
			p = q;
			update[n] = p;
			q = p->forward[n];
		}
		// q == Nil || q->key >= k
		// p -> key < k
	} while (--n >= 0);

	// n == 0
	// find the node to be deleted
	if (q  && q->key == k){
		
		for (n = 0; n < m && (p = update[n])->forward[n] == q; ++n){
			p->forward[n] = q->forward[n];
			free(q);
		}

		while (ls->head->forward[m] == &Nil  && m > 0){
			--m;
		}

		ls->current_max_level = m;
		return TRUE;
	}

	return FALSE;
}

boolean find(list ls, KeyType k)
{
	int n = ls->current_max_level;
	node p, q;
	node update[MaxSkipListLevel];

	p = ls->head;
	do{
		q = p->forward[n];
		while (q != &Nil && q->key < k){
			p = q;
			update[n] = p;
			q = p->forward[n];
		}
		// q == Nil || q->key >= k
		// p -> key < k
	} while (--n >= 0);

	// n == 0
	// find the node to be deleted
	if (q  && q->key == k){
		return TRUE;
	}

	return FALSE;
}



int main(int argc, char * argv[])
{
	printf("Hello World");

	list tmp = createSkipList();
	insert(tmp, 4, 10);
	insert(tmp, 5, 10);
	insert(tmp, 1000, 200000);
	for (int i = 0; i < 10; i++){
		printf("%d  ", find(tmp, i));
	}
	printf("\n%d\n", find(tmp, 1000));
	//insert(tmp, 100, 10);
	return 0;
}

