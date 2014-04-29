#ifndef ALIGNEDARRAY_HPP_INCLUDED
#define ALIGNEDARRAY_HPP_INCLUDED

#include <cstdint>

template <typename T, size_t size, size_t alignment = 64>
struct AlignedArray {
private:
	uint8_t bytes[sizeof(T) + (alignment * size) - 1];
public:
	T & operator[](size_t index) {
		return *reinterpret_cast<T *>(((intptr_t) bytes + (alignment - 1) & ~intptr_t(alignment - 1)) + (alignment * index));
	}
};

#endif