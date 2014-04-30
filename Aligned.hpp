#ifndef ALIGNED_HPP_INCLUDED
#define ALIGNED_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include "RoundUp.hpp"

template<typename T, size_t Alignment = -1>
class Aligned {
	static const size_t SizeOfTPaddedToAlignment = RoundUp<sizeof(T), Alignment>::value;
	uint8_t bytes[sizeof(T) + SizeOfTPaddedToAlignment - 1];
public:
	Aligned() {}
	Aligned(T const & value) { reference() = value; }
	T & reference() {
		return *reinterpret_cast<T *>(intptr_t(bytes) + (SizeOfTPaddedToAlignment - 1) & ~intptr_t(SizeOfTPaddedToAlignment - 1));
		//return ALIGNED_ALIGNMENT(bytes, SizeOfTPaddedToAlignment);
	}
	T const & reference() const { return reference(); }
};

template<typename T>
class Aligned<T, -1> {
	size_t const sizeOfTPaddedToAlignment;
	std::unique_ptr<uint8_t[]> const pBytes;
public:
	Aligned(size_t alignment) : sizeOfTPaddedToAlignment(roundUp(sizeof(T), alignment)), pBytes(new uint8_t[sizeof(T) + sizeOfTPaddedToAlignment - 1]) {}
	Aligned(T const & value, size_t alignment) : Aligned(alignment) { reference() = value; }
	T & reference() {
		return *reinterpret_cast<T *>(intptr_t(pBytes.get()) + (sizeOfTPaddedToAlignment - 1) & ~intptr_t(sizeOfTPaddedToAlignment - 1));
		//return ALIGNED_ALIGNMENT(pBytes.get(), sizeOfTPaddedToAlignment);
	}
	T const & reference() const { return reference(); }
};

template<typename T, size_t Size, size_t Alignment>
class Aligned<T[Size], Alignment> {
	static const size_t SizeOfTPaddedToAlignment = RoundUp<sizeof(T), Alignment>::value;
	uint8_t bytes[sizeof(T) + (SizeOfTPaddedToAlignment * Size) - 1];
public:
	Aligned() {};
	T & operator[](size_t index) {
		return *reinterpret_cast<T *>((intptr_t(bytes) + (SizeOfTPaddedToAlignment - 1) & ~intptr_t(SizeOfTPaddedToAlignment - 1)) + (SizeOfTPaddedToAlignment * index));
		//return ALIGNED_INDEX_ALIGNMENT(bytes, SizeOfTPaddedToAlignment, index);
	}
	T const & operator[](size_t index) const { return operator[](index); }
};

template<typename T, size_t Alignment>
class Aligned<T[], Alignment> {
	static const size_t SizeOfTPaddedToAlignment = RoundUp<sizeof(T), Alignment>::value;
	std::unique_ptr<uint8_t[]> const pBytes;
	Aligned();
public:
	Aligned(size_t size) : pBytes(new uint8_t[sizeof(T) + (SizeOfTPaddedToAlignment * size) - 1]) {}
	T & operator[](size_t index) {
		return *reinterpret_cast<T *>((intptr_t(pBytes.get()) + (SizeOfTPaddedToAlignment - 1) & ~intptr_t(SizeOfTPaddedToAlignment - 1)) + (SizeOfTPaddedToAlignment * index));
		//return ALIGNED_INDEX_ALIGNMENT(pBytes.get(), SizeOfTPaddedToAlignment, index);
	}
	T const & operator[](size_t index) const { return operator[](index); }
};

template<typename T>
class Aligned<T[], -1> {
	size_t const sizeOfTPaddedToAlignment;
	std::unique_ptr<uint8_t> const pBytes;
	Aligned();
public:
	Aligned(size_t size, size_t alignment) : sizeOfTPaddedToAlignment(roundUp(sizeof(T), alignment)), pBytes(new uint8_t[sizeof(T) + (sizeOfTPaddedToAlignment * size) - 1]) {}
	T & operator[](size_t index) {
		return *reinterpret_cast<T *>((intptr_t(pBytes.get()) + (sizeOfTPaddedToAlignment - 1) & ~intptr_t(sizeOfTPaddedToAlignment - 1)) + (sizeOfTPaddedToAlignment * index));
	}
	T const & operator[](size_t index) const { return operator[](index); }
};

#undef ALIGNMENT_EQUATION

#endif