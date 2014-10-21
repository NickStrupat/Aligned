#ifndef CACHE_ALIGNED_BASE_HPP_INCLUDED
#define CACHE_ALIGNED_BASE_HPP_INCLUDED

#include <cstddef>
#include "CacheLineSize/cacheLineSize.h"

class CacheAlignedBase {
protected:
	CacheAlignedBase() {}
	static std::size_t cacheLineSize() {
		static std::size_t cacheLineSize = cache_line_size();
		return cacheLineSize;
	}
};

#endif