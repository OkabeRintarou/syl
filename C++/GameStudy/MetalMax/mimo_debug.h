#ifndef _MIMO_DEBUG_
#define _MIMO_DEBUG_

namespace mimo{
	extern void debug_printf(const char * str);
#ifdef _DEBUG
#define MIMO_DEBUG(STR) debug_printf(STR)
#else
#define MIMO_DEBUG(STR) 
#endif

}
#endif //_MIMO_DEBUG_