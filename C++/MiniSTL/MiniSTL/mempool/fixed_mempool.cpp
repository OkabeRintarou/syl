#include <fixed_mempool.h>

namespace MiniSTL{
	fixed_mempool::fixed_mempool(ushort unit_size, ushort init_size /* = 1024 */, ushort grow_size /* = 256 */)
	{
		this->block = NULL;
		this->init_size = init_size;
		this->grow_size = grow_size;

		if (unit_size > 4){
			this->unit_size = ((unit_size + MEMORY_ALIGN - 1) & (~(MEMORY_ALIGN - 1)));
		}
		else if (unit_size < 2){
			this->unit_size = 2;
		}
		else{
			this->unit_size = 4;
		}
	}

	void * fixed_mempool::alloc()
	{
		if (!block){
			// 第一次内存分配
			block = new (init_size,unit_size)_memory_block(init_size, unit_size);
			
			return (void*)block->data;
		}
		_memory_block * pblock = block;
		while (pblock  && !pblock->nfree){
			pblock = pblock->pnext;
		}

		if (pblock){
			
			char * pfree = pblock->data + (pblock->first * unit_size);
			pblock->first = *((ushort*)pfree);//每一个内存分配单元头两个字节记录下一个分配单元的编号
			pblock->nfree--;

			return (void*)pfree;
		}
		else{
			if (!grow_size){
				return NULL;
			}
			pblock = new(grow_size, unit_size) _memory_block(grow_size, unit_size);
			if (pblock == 0){
				return NULL;
			}

			pblock->pnext = block;
			block = pblock;

			return (void*)pblock->data;
		}

		return NULL;
	}

	void fixed_mempool::free(void * p)
	{
		if (block == NULL){
			return;
		}
		_memory_block  * pblock = block;
		_memory_block * block_forward = 0;
		while (pblock){
			if (p >= pblock->data && p < pblock->data + pblock->size){
				pblock->nfree++;
				*((unsigned short*)p) = pblock->first;
				pblock->first = (unsigned short)(((unsigned long)p - (unsigned long)pblock->data) / unit_size);
				if (pblock->nfree * unit_size == pblock->size){
					if (block_forward) {
						block_forward->pnext = pblock->pnext;
					}
					else{
						block = 0;
					}
					delete pblock;
				}
				break;
			}
			block_forward = pblock;
			pblock = pblock->pnext;
		}
	}

	fixed_mempool::~fixed_mempool(){
		_memory_block * p = block;
		_memory_block * next = 0;
		while (p){
			next = p->pnext;
			delete p;
			p = next;
		}
	}

	_memory_block::_memory_block(unsigned short types /* = 1 */, unsigned short unit_size /* = 0 */):
		size(types * unit_size),
		nfree(types - 1),
		first(1),
		pnext(0)
	{
		char * pData = data;
		for (unsigned short i = 1; i < types; i++){
			*reinterpret_cast<unsigned short*>(pData) = i;
			pData += unit_size;
		}
	}

}

