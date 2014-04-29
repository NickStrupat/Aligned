#ifndef ALIGNED_HPP_INCLUDED
#define ALIGNED_HPP_INCLUDED

#include <cstdint>
#include "RoundUp.hpp"

template <typename T, size_t alignment = 64>
struct Aligned {
private:
	enum { SizeOfTPaddedToAlignment = RoundUp<sizeof(T), alignment>::value };
	uint8_t bytes[sizeof(T) + SizeOfTPaddedToAlignment - 1];
public:
	Aligned() {}
	Aligned(T const & value) {
		reference() = value;
	}
	T & reference() {
		return *reinterpret_cast<T *>((intptr_t) bytes + (SizeOfTPaddedToAlignment - 1) & ~intptr_t(SizeOfTPaddedToAlignment - 1));
	}
	T const & reference() const {
		return reference();
	}
};

#endif