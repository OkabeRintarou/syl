#include "Allocator.h"
#include <iostream>
#include <vector>
#include "Type_Traits.h"
#include "fixed_mempool.h"
#include <crtdbg.h>

#include <windows.h>
int main()
{
	{
		using namespace MiniSTL;
		fixed_mempool pool(sizeof(int));
		pool.alloc();

	}

	Sleep(100);
}