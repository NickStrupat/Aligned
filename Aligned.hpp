#ifndef ALIGNED_HPP_INCLUDED
#define ALIGNED_HPP_INCLUDED

#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>
#include "AlignedArrayBase.hpp"
#include "AlignedBase.hpp"

#define ALIGNED_BYTES_SIZE(alignment, sizeOfTPaddedToAlignment) alignment + sizeOfTPaddedToAlignment - 1
#define ALIGNED_POINTER(T, bytes, sizeOfTPaddedToAlignment) reinterpret_cast<T *>(uintptr_t(bytes) + (sizeOfTPaddedToAlignment - 1) & ~uintptr_t(sizeOfTPaddedToAlignment - 1))
#define ALIGNED_ARRAY_BYTES_SIZE(T, sizeOfTPaddedToAlignment, size) sizeof(T) + (sizeOfTPaddedToAlignment * size) - 1
#define ALIGNED_ARRAY_INDEX_POINTER(T, pFirstElement, sizeOfTPaddedToAlignment, index) reinterpret_cast<T *>(uintptr_t(pFirstElement) + (sizeOfTPaddedToAlignment * index))

template<typename T, std::size_t Alignment = -1>
class Aligned : AlignedBase<T, Alignment> {
	uint8_t bytes[ALIGNED_BYTES_SIZE(Alignment, sizeOfTPaddedToAlignment)];
	T * pValue;
public:
	Aligned(T const & value = T())
		: pValue(ALIGNED_POINTER(T, bytes, sizeOfTPaddedToAlignment))
	{ *pValue = T(value); }
	~Aligned() { pValue->T::~T(); }
	T & Ref() { return *pValue; }
	T const & Ref() const { return *pValue; }
};

template<typename T>
class Aligned<T, -1> : AlignedBase<T, -1> {
	std::unique_ptr<uint8_t[]> const pBytes;
	T * pValue;
public:
	Aligned(std::size_t alignment, T const & value = T())
		: AlignedBase(alignment)
		, pBytes(new uint8_t[ALIGNED_BYTES_SIZE(alignment, sizeOfTPaddedToAlignment)])
		, pValue(ALIGNED_POINTER(T, pBytes.get(), sizeOfTPaddedToAlignment))
	{ *pValue = T(value);	}
	~Aligned() { pValue->T::~T(); }
	T & Ref() { return *pValue; }
	T const & Ref() const { return *pValue; }
};

template<typename T, bool IsTriviallyDefaultConstructible = std::is_trivially_default_constructible<T>::value>
class AlignedArrayConstructorCaller {
	AlignedArrayConstructorCaller();
public:
	static void Call(std::size_t const, T * const, std::size_t const) {}
};

template<typename T>
class AlignedArrayConstructorCaller<T, false> {
	AlignedArrayConstructorCaller();
public:
	static void Call(std::size_t const size, T * const pFirstElement, std::size_t const sizeOfTPaddedToAlignment) {
		for (std::size_t i = 0; i != size; ++i)
			new (ALIGNED_ARRAY_INDEX_POINTER(T, pFirstElement, sizeOfTPaddedToAlignment, i)) T;
	}
};

template<typename T, bool IsTriviallyDestructible = std::is_trivially_destructible<T>::value>
class AlignedArrayDestructorCaller {
	AlignedArrayDestructorCaller();
public:
	static void Call(std::size_t const, T * const, std::size_t const) {}
};

template<typename T>
class AlignedArrayDestructorCaller<T, false> {
	AlignedArrayDestructorCaller();
public:
	static void Call(std::size_t const size, T * const pFirstElement, std::size_t const sizeOfTPaddedToAlignment) {
		for (std::size_t i = 0; i != size; ++i)
			ALIGNED_ARRAY_INDEX_POINTER(T, pFirstElement, sizeOfTPaddedToAlignment, i)->T::~T();
	}
};

template<typename T, std::size_t Size, std::size_t Alignment>
class Aligned<T[Size], Alignment> : AlignedBase<T, Alignment> {
	Aligned(Aligned const &);
	Aligned & operator=(Aligned const &);
	static std::size_t const size = Size;
	uint8_t bytes[ALIGNED_ARRAY_BYTES_SIZE(T, sizeOfTPaddedToAlignment, Size)];
	T * const pFirstElement;
	T & element(std::size_t index) { return *ALIGNED_ARRAY_INDEX_POINTER(T, pFirstElement, sizeOfTPaddedToAlignment, index); }
public:
	Aligned()
		: pFirstElement(ALIGNED_POINTER(T, bytes, sizeOfTPaddedToAlignment))
	{
		AlignedArrayConstructorCaller<T>::Call(size, pFirstElement, sizeOfTPaddedToAlignment);
	}
	~Aligned() {
		AlignedArrayDestructorCaller<T>::Call(size, pFirstElement, sizeOfTPaddedToAlignment);
	}
	T & operator[](std::size_t index) { return element(index); }
	T const & operator[](std::size_t index) const { return element(index); }
	std::size_t Size() const { return size; }
};

template<typename T, std::size_t Alignment>
class Aligned<T[], Alignment> : AlignedBase<T, Alignment> {
	Aligned(Aligned const &);
	Aligned & operator=(Aligned const &);
	std::size_t const size;
	std::unique_ptr<uint8_t[]> const pBytes;
	T * const pFirstElement;
	T & element(std::size_t index) { return *ALIGNED_ARRAY_INDEX_POINTER(T, pFirstElement, sizeOfTPaddedToAlignment, index); }
public:
	Aligned(std::size_t size)
		: size(size)
		, pBytes(new uint8_t[ALIGNED_ARRAY_BYTES_SIZE(T, sizeOfTPaddedToAlignment, size)])
		, pFirstElement(ALIGNED_POINTER(T, pBytes.get(), sizeOfTPaddedToAlignment))
	{
		AlignedArrayConstructorCaller<T>::Call(size, pFirstElement, sizeOfTPaddedToAlignment);
	}
	~Aligned() {
		AlignedArrayDestructorCaller<T>::Call(size, pFirstElement, sizeOfTPaddedToAlignment);
	}
	T & operator[](std::size_t index) { return element(index); }
	T const & operator[](std::size_t index) const { return element(index); }
	std::size_t Size() const { return size; }
};

template<typename T>
class Aligned<T[], -1> : AlignedBase<T, -1> {
	Aligned(Aligned const &);
	Aligned & operator=(Aligned const &);
	std::size_t const size;
	std::unique_ptr<uint8_t[]> const pBytes;
	T * const pFirstElement;
	T & element(std::size_t index) { return *ALIGNED_ARRAY_INDEX_POINTER(T, pFirstElement, sizeOfTPaddedToAlignment, index); }
	Aligned();
public:
	Aligned(std::size_t alignment, std::size_t size)
		: AlignedBase<T, -1>(alignment)
		, size(size)
		, pBytes(new uint8_t[ALIGNED_ARRAY_BYTES_SIZE(T, sizeOfTPaddedToAlignment, size)])
		, pFirstElement(ALIGNED_POINTER(T, pBytes.get(), sizeOfTPaddedToAlignment))
	{
		AlignedArrayConstructorCaller<T>::Call(size, pFirstElement, sizeOfTPaddedToAlignment);
	}
	~Aligned() {
		AlignedArrayDestructorCaller<T>::Call(size, pFirstElement, sizeOfTPaddedToAlignment);
	}
	T & operator[](std::size_t index) { return element(index); }
	T const & operator[](std::size_t index) const { return element(index); }
	std::size_t Size() const { return size; }
};

#undef ALIGNED_BYTES_SIZE
#undef ALIGNED_POINTER
#undef ALIGNED_ARRAY_BYTES_SIZE
#undef ALIGNED_ARRAY_INDEX_POINTER

#endif