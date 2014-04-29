#ifndef ALIGNED_HPP_INCLUDED
#define ALIGNED_HPP_INCLUDED

#include <cstdint>

template <typename T, size_t alignment = 64>
struct Aligned {
private:
	uint8_t bytes[sizeof(T) + alignment - 1];
public:
	Aligned() {}
	Aligned(T const & value) {
		reference() = value;
	}
	T & reference() {
		return *reinterpret_cast<T *>((intptr_t) bytes + (alignment - 1) & ~intptr_t(alignment - 1));
	}
	T const & reference() const {
		return reference();
	}
};

#endif