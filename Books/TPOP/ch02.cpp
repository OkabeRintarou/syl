#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define _DEBUG

static void swap(int v[],int i,int j)
{
	int tmp;
	tmp 	= 	v[i];
	v[i] 	= 	v[j];
	v[j] 	= 	tmp;
}

void quicksort(int v[],int n)
{
	int i,last;
	if(n <= 1)return;
	swap(v,0,rand() % n );
	last = 0;

	for(i = 1; i < n;++i){
		if(v[i] < v[0]){
			swap(v,++last,i);
		}
	}
	swap(v,0,last);
	quicksort(v,last);
	quicksort(v + last + 1 , n - last - 1);
}

typedef struct Nameval Nameval;

struct Nameval
{
	char * name;
	int value;
	Nameval * next;
};

int lookup(char * name,Nameval tab[],int ntab)
{
	int low,mid,high,cmp;
	low = 0;
	high = ntab - 1;
	while(low <= high){
		mid = (low + high) / 2;
		cmp = strcmp(name,tab[mid].name);
		if(cmp < 0){
			high = mid - 1;
		}
		else if(cmp > 0){
			low = mid + 1;
		}
		else{
			return mid;
		}
	}

	return -1;
}

void * emalloc(size_t size)
{
	void * data = malloc(size);
	if(data == NULL){
		printf("bad memory alloc.\n");
		exit(-1);
	}
	return data;
}

/*newitem:create new item from name and value*/
Nameval * newitem(char * name,int value)
{
	Nameval * newp;
	newp = (Nameval*)emalloc(sizeof(Nameval));
	newp->name = name;
	newp->value = value;
	newp->next = NULL;

	return newp;
}

/* addfront:add newp to front of listp
   and return the pointer of the head of the list
   Example:nvlist = addfront(nvlist,newitem("smiley",0x263A));
*/
Nameval * addfront(Nameval * listp,Nameval * newp)
{
	newp->next = listp;
	return newp;
}

/* addend:add newp to end of listp
   and return the pointer of the head of the list
*/
Nameval * addend(Nameval * listp,Nameval * newp)
{
	Nameval * cur;
	cur = listp;
	if(cur == NULL)return listp;
	while(cur->next != NULL){
		cur = cur->next;
	}
	cur->next = newp;
	return listp;
}

/* lookup:sequential search for name in listp
   if not find name then return NULL
*/
Nameval * lookup(Nameval * listp,char * name)
{
	for(;listp != NULL;listp = listp->next){
		if(strcmp(name,listp->name) == 0){
			return listp;
		}
	}
	return NULL;
}

/* apply:execute fn for each element of listp*/
void apply(Nameval * listp,
	void (*fn)(Nameval *,void*),
	void * arg)
{
	for(;listp != NULL;listp = listp->next){
		(*fn)(listp,arg);
	}
}

/* freeall:free all elements of listp*/
void freeall(Nameval * listp)
{
	Nameval * next;
	for(;listp != NULL;listp = next){
		next = listp->next;
		free(listp);
	}
}

/* delitem:delete first "name" from listp*/
Nameval *  delitem(Nameval * listp,char * name)
{
	typedef Nameval * pointer;
	pointer p,prev;
	prev = NULL;

	for(p = listp;p != NULL;p = listp->next){
		if(strcmp(name,p->name) == 0){
			if(prev == NULL){
				listp = p->next;
			}
			else{
				prev->next = p->next;
			}
			free(p);
			return listp;
		}
		prev = p;
	}	

	printf("delitem:%s not in list.\n",name);
	return NULL;
}

#ifdef _DEBUG
void printNameValList(Nameval * listp)
{
	Nameval * p ;
	for(p = listp;p != NULL;p = p->next){
		printf("name : %s , value : %d.\n",p->name,p->value);
	}
}
void printnv(Nameval * listp,void * arg)
{
	char * fmt = (char*)arg;
	printf(fmt,listp->name,listp->value);
}

void inccounter(Nameval * p,void * arg)
{
	int * pcounter = (int*)arg;
	++(*pcounter);
}
#endif

typedef struct NamevalTree NamevalTree;
struct NamevalTree
{
	char * name;
	int value;
	NamevalTree * left;
	NamevalTree * right;
};

/* insert: insert newp in treep,return treep*/
NamevalTree * insert(NamevalTree * treep,NamevalTree * newp)
{
	if(treep == NULL)return newp;
	int cmp = strcmp(treep->name,newp->name);
	if(cmp <= 0){
		treep->left = insert(treep->left,newp);
	}
	else if(cmp > 0){
		treep->right = insert(treep->right,newp);
	}
	return treep;
}

/* lookup:look up name in tree treep*/
NamevalTree * lookup(NamevalTree * treep,char * name)
{
	if(treep == NULL)return NULL;
	int cmp;
	cmp = strcmp(treep->name,name);
	if(cmp  ==  0){
		return treep;
	}
	else if(cmp < 0){
		return lookup(treep->left,name);
	}
	else{
		return lookup(treep->right,name);
	}
}


int main(int argc,char * argv[])
{
	Nameval tab[] = {
		"AElig"     ,  0x00c6, NULL,
		"Aacute"    ,  0x00c1, NULL,
		"Acirc"     ,  0x00c2, NULL,
		"frac12"    ,  0x00c3, NULL,
		"zeta"      ,  0x03b6, NULL,
		"zfrt"      ,  0x04ff, NULL,
	};

	printf("%d\n",lookup("frac12",tab,6));
	
	int vint[] = {3,5,6,1,2,3,1,99,8,7};
	const int size = sizeof(vint) / sizeof(*vint);
	quicksort(vint,size);
	for(int i = 0; i < size; ++i){
		printf("%d ",vint[i]);
	}

	printf("\n");
	Nameval * listp = newitem("test",0);
	for(int i = 1; i < 10;++i){
		char tmp[1];
		listp = addfront(listp,newitem("test",i));
		listp = addend(listp,newitem("test",i));
	}
	apply(listp,printnv,(void*)"%s:%x\n");
	int n = 0;
	apply(listp,inccounter,&n);
	printf("the new length of the list is %d.\n",n);


	return 0;
}