#ifndef CACHEALIGNED_HPP_INCLUDED
#define CACHEALIGNED_HPP_INCLUDED

#include <cstddef>
#include "CacheAlignedBase.hpp"

template<typename T>
class CacheAligned : public Aligned<T>, CacheAlignedBase<T> {
public:
	template<typename... Args>
	CacheAligned(Args &&... args) : Aligned(cacheLineSize()) {}
};

template<typename T>
class CacheAligned<T[]> : public Aligned<T[]>, CacheAlignedBase<T> {
public:
	CacheAligned(std::size_t size) : Aligned(cacheLineSize(), size) {}
};

#endif