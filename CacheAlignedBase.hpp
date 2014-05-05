#ifndef CACHEALIGNEDBASE_HPP_INCLUDED
#define CACHEALIGNEDBASE_HPP_INCLUDED

#include <cstddef>
#include "CacheLineSize/cacheLineSize.h"

class CacheAlignedBase {
protected:
	static std::size_t cacheLineSize() {
		static std::size_t cacheLineSize = cache_line_size();
		return cacheLineSize;
	}
};

#endif