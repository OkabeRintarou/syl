#ifndef _XVM_XVM_H_
#define _XVM_XVM_H_

#include <string>
#include "Structure.h"
#include "Global.h"

namespace xvm
{
	class XVM
	{
	public:
		XVM(){}
		XVM(const std::string & filename){}
		LoadState LoadScript(const std::string & filename);
		void UnLoadScript();
	private:
		inline int GetOpType(int OpIndex);
		inline int GetOpAsInt(int OpIndex);
		inline float GetOpAsFloat(int OpIndex);
		inline char * GetOpAsString(int OpIndex);
		inline int GetOpAsStackIndex(int OpIndex);
		inline int GetOpAsInstrIndex(int OpIndex);
		inline int GetOpAsFuncIndex(int OpIndex);
		Value ResolveOpValue(int OpIndex);
		int ResolveOpAsInt(int OpIndex);
	private:
		std::string filename_;
		Script script_;
	};
}
#endif