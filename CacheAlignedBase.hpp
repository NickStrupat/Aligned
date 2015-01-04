#ifndef CACHEALIGNEDBASE_HPP_INCLUDED
#define CACHEALIGNEDBASE_HPP_INCLUDED

#include <cstddef>
extern "C" {
	#include "CacheLineSize/CacheLineSize.h"
}

template<typename T>
class CacheAlignedBase {
protected:
	CacheAlignedBase() {}
	static size_t cacheLineSize;
	/*inline static std::size_t cacheLineSize() {
		static std::size_t cacheLineSize = CacheLineSize();
		return cacheLineSize;
	}*/
};

template<typename T>
size_t CacheAlignedBase::cacheLineSize(CacheLineSize());

#endif