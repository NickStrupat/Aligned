#ifndef CACHEALIGNED_HPP_INCLUDED
#define CACHEALIGNED_HPP_INCLUDED

#include <cstddef>
#include <initializer_list>
#include <type_traits>

#include "Aligned.hpp"
#include "CacheAlignedBase.hpp"

template<typename T>
class CacheAligned : public Aligned<T>, CacheAlignedBase<T> {
public:
	template<typename... Args>
	CacheAligned(Args &&... args) : Aligned(cacheLineSize(), std::forward<Args>(args)...) {}
};

template<typename T>
class CacheAligned<T[]> : public Aligned<T[]>, CacheAlignedBase<T> {
public:
	CacheAligned(std::size_t size) : Aligned(cacheLineSize(), size) {}
	CacheAligned(std::initializer_list<T> const & list)
		: CacheAligned(list.size())
	{
		int i = 0;
		for (auto it = list.begin(); it != list.end(); ++it)
			this->element(i++) = *it;
	}
};

#endif