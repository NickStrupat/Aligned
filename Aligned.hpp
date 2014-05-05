#ifndef ALIGNED_HPP_INCLUDED
#define ALIGNED_HPP_INCLUDED

#include <cstddef>
#include <cstdint>
#include <memory>
#include "AlignedBase.hpp"

#define ALIGNED_BYTES_SIZE(T, sizeOfTPaddedToAlignment) sizeof(T) + sizeOfTPaddedToAlignment - 1
#define ALIGNED_REF(T, bytes, sizeOfTPaddedToAlignment) *reinterpret_cast<T *>(uintptr_t(bytes) + (sizeOfTPaddedToAlignment - 1) & ~uintptr_t(sizeOfTPaddedToAlignment - 1))
#define ALIGNED_ARRAY_BYTES_SIZE(T, sizeOfTPaddedToAlignment, size) sizeof(T) + (sizeOfTPaddedToAlignment * size) - 1
#define ALIGNED_ARRAY_INDEX(T, bytes, sizeOfTPaddedToAlignment, index) *reinterpret_cast<T *>((uintptr_t(bytes) + (sizeOfTPaddedToAlignment - 1) & ~uintptr_t(sizeOfTPaddedToAlignment - 1)) + (sizeOfTPaddedToAlignment * index))

template<typename T, std::size_t Alignment = -1>
class Aligned : AlignedBase<T, Alignment> {
	uint8_t bytes[ALIGNED_BYTES_SIZE(T, SizeOfTPaddedToAlignment)];
public:
	Aligned() {}
	Aligned(T const & value) { ref() = value; }
	T & ref() { return ALIGNED_REF(T, bytes, SizeOfTPaddedToAlignment); }
	T const & ref() const { return ref(); }
};

template<typename T>
class Aligned<T, -1> : AlignedBase<T, -1> {
	std::unique_ptr<uint8_t[]> const pBytes;
protected:
	Aligned(std::size_t alignment, void * ignoredParameterForOverload) : AlignedBase(alignment), pBytes(new uint8_t[alignment + sizeOfTPaddedToAlignment - 1]) {}
public:
	Aligned(std::size_t alignment) : AlignedBase(alignment), pBytes(new uint8_t[ALIGNED_BYTES_SIZE(T, sizeOfTPaddedToAlignment)]) {}
	Aligned(std::size_t alignment, T const & value) : Aligned(alignment) { ref() = value; }
	T & ref() { return ALIGNED_REF(T, pBytes.get(), sizeOfTPaddedToAlignment); }
	T const & ref() const { return ref(); }
};

template<typename T, std::size_t Size, std::size_t Alignment>
class Aligned<T[Size], Alignment> : AlignedBase<T, Alignment> {
	uint8_t bytes[ALIGNED_ARRAY_BYTES_SIZE(T, SizeOfTPaddedToAlignment, Size)];
public:
	Aligned() {}
	T & operator[](std::size_t index) { return ALIGNED_ARRAY_INDEX(T, bytes, SizeOfTPaddedToAlignment, index); }
	T const & operator[](std::size_t index) const { return operator[](index); }
};

template<typename T, std::size_t Alignment>
class Aligned<T[], Alignment> : AlignedBase<T, Alignment> {
	std::unique_ptr<uint8_t[]> const pBytes;
public:
	Aligned(std::size_t size) : pBytes(new uint8_t[ALIGNED_ARRAY_BYTES_SIZE(T, SizeOfTPaddedToAlignment, size)]) {}
	T & operator[](std::size_t index) { return ALIGNED_ARRAY_INDEX(T, pBytes.get(), SizeOfTPaddedToAlignment, index); }
	T const & operator[](std::size_t index) const { return operator[](index); }
};

template<typename T>
class Aligned<T[], -1> : AlignedBase<T, -1> {
	std::unique_ptr<uint8_t[]> const pBytes;
	Aligned();
public:
	Aligned(std::size_t alignment, std::size_t size) : AlignedBase<T, -1>(alignment), pBytes(new uint8_t[ALIGNED_ARRAY_BYTES_SIZE(T, sizeOfTPaddedToAlignment, size)]) {}
	T & operator[](std::size_t index) { return ALIGNED_ARRAY_INDEX(T, pBytes.get(), sizeOfTPaddedToAlignment, index); }
	T const & operator[](std::size_t index) const { return operator[](index); }
};

#undef ALIGNED_BYTES_SIZE
#undef ALIGNED_REF
#undef ALIGNED_ARRAY_BYTES_SIZE
#undef ALIGNED_ARRAY_INDEX

#endif