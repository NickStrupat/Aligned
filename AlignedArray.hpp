#ifndef ALIGNEDARRAY_HPP_INCLUDED
#define ALIGNEDARRAY_HPP_INCLUDED

#include <cstdint>
#include "RoundUp.hpp"

template <typename T, size_t size, size_t alignment = 64>
struct AlignedArray {
private:
	enum { SizeOfTPaddedToAlignment = RoundUp<sizeof(T), alignment>::value };
	uint8_t bytes[sizeof(T) + (SizeOfTPaddedToAlignment * size) - 1];
public:
	T & operator[](size_t index) {
		return *reinterpret_cast<T *>(((intptr_t) bytes + (SizeOfTPaddedToAlignment - 1) & ~intptr_t(SizeOfTPaddedToAlignment - 1)) + (SizeOfTPaddedToAlignment * index));
	}
};

#endif