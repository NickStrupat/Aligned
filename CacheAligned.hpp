#ifndef CACHEALIGNED_HPP_INCLUDED
#define CACHEALIGNED_HPP_INCLUDED

#include <cstddef>
#include <cstdint>
#include "CacheAlignedBase.hpp"

template<typename T>
class CacheAligned : public Aligned<T>, CacheAlignedBase {
public:
	CacheAligned() : Aligned(cacheLineSize()) {}
	CacheAligned(T const & value) : Aligned(cacheLineSize(), value) {}
};

template<typename T>
class CacheAligned<T[]> : public Aligned<T[]>, CacheAlignedBase {
public:
	CacheAligned(std::size_t size) : Aligned(cacheLineSize(), size) {}
};

#endif