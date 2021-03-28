/**
* @file fixed_mempool.h
* @brief 固定内存池
*
* @author sylcoder
* @version 1.0
* @date 2014/12/27
*/
#ifndef _FIXED_MEMPOOL_INCLUDED_
#define _FIXED_MEMPOOL_INCLUDED_

#include <cstddef>
namespace MiniSTL{

	struct _memory_block;

	class fixed_mempool
	{
	private:
		typedef unsigned short ushort;
		enum {MEMORY_ALIGN = 8};
	public:
		fixed_mempool(ushort unit_size,
			ushort init_size = 1024,
			ushort grow_size = 256);

		// 释放整个内存池
		~fixed_mempool();
		// 分配unit_size字节大小的内存，失败则返回NULL
		void * alloc();
		// 内存池回收p指向内存
		void free(void * p);
	private:
		_memory_block * block;
		ushort unit_size;//内存分配单元大小，以字节为单位
		ushort init_size;//初始化内存分配单元数目
		ushort grow_size;//增长时内存分配单元数目

	};


	/**/
	struct _memory_block
	{
		unsigned short size;//一个内存块分配单元的数目,不包括头信息
		unsigned short nfree;//剩余的自由分配单元数目
		unsigned short first;//下一个可供分配的内存单元编号
		unsigned short dummy_align;

		_memory_block * pnext;
		char data[1];

		static void * operator new(size_t,unsigned short types,unsigned short uint_size )
		{
			return ::operator new(sizeof(_memory_block) + types * uint_size);
		}

		static void operator delete(void * p, size_t)
		{
			::operator delete(p);
		}

		_memory_block(unsigned short types = 1, unsigned short unit_size = 0);
		~_memory_block(){}
	};
}
#endif
