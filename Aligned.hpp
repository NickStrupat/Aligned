#ifndef ALIGNED_HPP_INCLUDED
#define ALIGNED_HPP_INCLUDED

#include <cstddef>
#include <cstdint>
#include <memory>
#include "AlignedBase.hpp"

#define ALIGNED_BYTES_SIZE(alignment, sizeOfTPaddedToAlignment) alignment + sizeOfTPaddedToAlignment - 1
#define ALIGNED_POINTER(T, bytes, sizeOfTPaddedToAlignment) reinterpret_cast<T *>(uintptr_t(bytes) + (sizeOfTPaddedToAlignment - 1) & ~uintptr_t(sizeOfTPaddedToAlignment - 1))
#define ALIGNED_ARRAY_BYTES_SIZE(T, sizeOfTPaddedToAlignment, size) sizeof(T) + (sizeOfTPaddedToAlignment * size) - 1
#define ALIGNED_ARRAY_INDEX_POINTER(T, bytes, sizeOfTPaddedToAlignment, index) reinterpret_cast<T *>((uintptr_t(bytes) + (sizeOfTPaddedToAlignment - 1) & ~uintptr_t(sizeOfTPaddedToAlignment - 1)) + (sizeOfTPaddedToAlignment * index))

template<typename T, std::size_t Alignment = -1>
class Aligned : AlignedBase<T, Alignment> {
	uint8_t bytes[ALIGNED_BYTES_SIZE(Alignment, SizeOfTPaddedToAlignment)];
	T * pValue;
public:
	Aligned(T const & value = T())
		: pValue(ALIGNED_POINTER(T, bytes, SizeOfTPaddedToAlignment))
	{ *pValue = T(value); }
	~Aligned() { pValue->~T(); }
	T & Get() { return *pValue; }
	T const & Get() const { return *pValue; }
};

template<typename T>
class Aligned<T, -1> : AlignedBase<T, -1> {
	std::unique_ptr<uint8_t[]> const pBytes;
	T * pValue;
public:
	Aligned(std::size_t alignment, T const & value = T())
		: AlignedBase(alignment),
		pBytes(new uint8_t[ALIGNED_BYTES_SIZE(alignment, sizeOfTPaddedToAlignment)]),
		pValue(ALIGNED_POINTER(T, pBytes.get(), sizeOfTPaddedToAlignment))
	{ *pValue = T(value); }
	~Aligned() { pValue->~T(); }
	T & Get() { return *pValue; }
	T const & Get() const { return *pValue; }
};

template<typename T, std::size_t Size, std::size_t Alignment>
class Aligned<T[Size], Alignment> : AlignedBase<T, Alignment>{
	static const std::size_t size = Size;
	uint8_t bytes[ALIGNED_ARRAY_BYTES_SIZE(T, SizeOfTPaddedToAlignment, Size)];
	T * pFirstElement;
	T & element(std::size_t index) { return *reinterpret_cast<T *>(uintptr_t(pFirstElement) + (SizeOfTPaddedToAlignment * index)); }
public:
	Aligned() : pFirstElement(ALIGNED_POINTER(T, bytes, SizeOfTPaddedToAlignment)) {}
	T & operator[](std::size_t index) { return element(index); }
	T const & operator[](std::size_t index) const { return element(index); }
	std::size_t Size() const { return size; }
};

template<typename T, std::size_t Alignment>
class Aligned<T[], Alignment> : AlignedBase<T, Alignment> {
	std::size_t const size;
	std::unique_ptr<uint8_t[]> const pBytes;
	T * pFirstElement;
	T & element(std::size_t index) { return *reinterpret_cast<T *>(uintptr_t(pFirstElement) + (SizeOfTPaddedToAlignment * index)); }
public:
	Aligned(std::size_t size)
		: size(size),
		pBytes(new uint8_t[ALIGNED_ARRAY_BYTES_SIZE(T, SizeOfTPaddedToAlignment, size)]),
		pFirstElement(ALIGNED_POINTER(T, pBytes.get(), SizeOfTPaddedToAlignment))
	{
		std::cout << (int *)(pBytes.get()) << std::endl;
		std::cout << pFirstElement << std::endl;
	}
	T & operator[](std::size_t index) { return element(index); }
	T const & operator[](std::size_t index) const { return element(index); }
	std::size_t Size() const { return size; }
};

template<typename T>
class Aligned<T[], -1> : AlignedBase<T, -1> {
	std::size_t size;
	std::unique_ptr<uint8_t[]> const pBytes;
	T * pFirstElement;
	T & element(std::size_t index) { return *reinterpret_cast<T *>(uintptr_t(pFirstElement) + (sizeOfTPaddedToAlignment * index)); }
	Aligned();
public:
	Aligned(std::size_t alignment, std::size_t size)
		: AlignedBase<T, -1>(alignment),
		size(size),
		pBytes(new uint8_t[ALIGNED_ARRAY_BYTES_SIZE(T, sizeOfTPaddedToAlignment, size)]),
		pFirstElement(ALIGNED_POINTER(T, pBytes.get(), sizeOfTPaddedToAlignment))
	{}
	T & operator[](std::size_t index) { return element(index); }
	T const & operator[](std::size_t index) const { return element(index); }
	std::size_t Size() const { return size; }
};

#undef ALIGNED_BYTES_SIZE
#undef ALIGNED_POINTER
#undef ALIGNED_ARRAY_BYTES_SIZE
#undef ALIGNED_ARRAY_INDEX_POINTER

#endif