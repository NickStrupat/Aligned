#ifndef CACHEALIGNED_HPP_INCLUDED
#define CACHEALIGNED_HPP_INCLUDED

#include <cstddef>
#include <cstdint>
#include "CacheAlignedBase.hpp"
#include "Aligned.hpp"
#include "RoundUp.hpp"

template<typename T>
class CacheAligned : protected Aligned<T>, CacheAlignedBase {
public:
	CacheAligned() : Aligned(cacheLineSize(), nullptr) {}
	CacheAligned(T const & value) : CacheAligned() { ref() = value; }
	T & ref() { return *reinterpret_cast<T *>(&Aligned::ref()); }
	T const & ref() const { return ref(); }
};

template<typename T, std::size_t Size>
class CacheAligned<T[Size]> : Aligned<T[]>, CacheAlignedBase{
public:
	CacheAligned() : Aligned(cacheLineSize(), Size) {}
	T & operator[](std::size_t index) { return *reinterpret_cast<T *>(&Aligned::operator[](index)); }
	T const & operator[](std::size_t index) const { return operator[](index); }
};

template<typename T>
class CacheAligned<T[]> : Aligned<T[]>, CacheAlignedBase {
public:
	CacheAligned(std::size_t size) : Aligned(cacheLineSize(), size) {}
	T & operator[](std::size_t index) { return *reinterpret_cast<T *>(&Aligned::operator[](index)); }
	T const & operator[](std::size_t index) const { return operator[](index); }
};

#endif