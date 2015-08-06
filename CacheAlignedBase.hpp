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
	static std::size_t cacheLineSize() {
		static std::size_t cacheLineSize = CacheLineSize();
		return cacheLineSize;
	}
};

#endif