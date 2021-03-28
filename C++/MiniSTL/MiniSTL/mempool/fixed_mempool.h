/**
* @file fixed_mempool.h
* @brief �̶��ڴ��
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

		// �ͷ������ڴ��
		~fixed_mempool();
		// ����unit_size�ֽڴ�С���ڴ棬ʧ���򷵻�NULL
		void * alloc();
		// �ڴ�ػ���pָ���ڴ�
		void free(void * p);
	private:
		_memory_block * block;
		ushort unit_size;//�ڴ���䵥Ԫ��С�����ֽ�Ϊ��λ
		ushort init_size;//��ʼ���ڴ���䵥Ԫ��Ŀ
		ushort grow_size;//����ʱ�ڴ���䵥Ԫ��Ŀ

	};


	/**/
	struct _memory_block
	{
		unsigned short size;//һ���ڴ����䵥Ԫ����Ŀ,������ͷ��Ϣ
		unsigned short nfree;//ʣ������ɷ��䵥Ԫ��Ŀ
		unsigned short first;//��һ���ɹ�������ڴ浥Ԫ���
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
