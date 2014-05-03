#ifndef ALIGNED_HPP_INCLUDED
#define ALIGNED_HPP_INCLUDED

#include <cstddef>
#include <cstdint>
#include <memory>
#include "AlignedBase.hpp"
#include "RoundUp.hpp"

template<typename T, std::size_t Alignment = -1>
class Aligned : AlignedBase<Alignment> {
	static const std::size_t SizeOfTPaddedToAlignmentLessOne = RoundUp<sizeof(T), Alignment>::value - 1;
protected:
	uint8_t bytes[SizeOfTPaddedToAlignmentLessOne + SizeOfTPaddedToAlignmentLessOne + 1];
public:
	class PaddedType { uint8_t bytes[RoundUp<sizeof(T), Alignment>::value]; };
	Aligned() {}
	Aligned(T const & value) { ref() = value; }
	T & ref() {
		return *reinterpret_cast<T *>(uintptr_t(bytes) + SizeOfTPaddedToAlignmentLessOne & ~uintptr_t(SizeOfTPaddedToAlignmentLessOne));
	}
	T const & ref() const { return ref(); }
};

template<typename T, std::size_t Size, std::size_t Alignment>
class Aligned<T[Size], Alignment> : AlignedBase<Alignment> {
	static const std::size_t SizeOfTPaddedToAlignment = RoundUp<sizeof(T), Alignment>::value;
	uint8_t bytes[sizeof(T) + (SizeOfTPaddedToAlignment * Size) - 1];
public:
	Aligned() {}
	T & operator[](std::size_t index) {
		return *reinterpret_cast<T *>((uintptr_t(bytes) + (SizeOfTPaddedToAlignment - 1) & ~uintptr_t(SizeOfTPaddedToAlignment - 1)) + (SizeOfTPaddedToAlignment * index));
	}
	T const & operator[](std::size_t index) const { return operator[](index); }
};

template<typename T, std::size_t Alignment>
class Aligned<T[], Alignment> : AlignedBase<Alignment>{
	static const std::size_t SizeOfTPaddedToAlignment = RoundUp<sizeof(T), Alignment>::value;
	std::unique_ptr<uint8_t[]> const pBytes;
	Aligned();
public:
	Aligned(std::size_t size) : pBytes(new uint8_t[sizeof(T) + (SizeOfTPaddedToAlignment * size) - 1]) {}
	T & operator[](std::size_t index) {
		return *reinterpret_cast<T *>((uintptr_t(pBytes.get()) + (SizeOfTPaddedToAlignment - 1) & ~uintptr_t(SizeOfTPaddedToAlignment - 1)) + (SizeOfTPaddedToAlignment * index));
	}
	T const & operator[](std::size_t index) const { return operator[](index); }
};

template<typename T>
class Aligned<T, -1> : AlignedBase<-1>{
	std::size_t const sizeOfTPaddedToAlignmentLessOne;
	std::unique_ptr<uint8_t[]> const pBytes;
public:
	Aligned(std::size_t alignment = AlignedBase::cacheLineSize()) : AlignedBase(alignment), sizeOfTPaddedToAlignmentLessOne(roundUp(sizeof(T), alignment) - 1), pBytes(new uint8_t[sizeof(T) +sizeOfTPaddedToAlignmentLessOne]) {}
	Aligned(T const & value, std::size_t alignment) : Aligned(alignment) { ref() = value; }
	T & ref() {
		return *reinterpret_cast<T *>(uintptr_t(pBytes.get()) + sizeOfTPaddedToAlignmentLessOne & ~uintptr_t(sizeOfTPaddedToAlignmentLessOne));
	}
	T const & ref() const { return ref(); }
};

template<typename T>
class Aligned<T[], -1> : AlignedBase<-1>{
	std::size_t const sizeOfTPaddedToAlignment;
	std::unique_ptr<uint8_t> const pBytes;
	Aligned();
public:
	Aligned(std::size_t size, std::size_t alignment = AlignedBase::cacheLineSize())
		: AlignedBase<-1>(alignment),
		sizeOfTPaddedToAlignment(roundUp(sizeof(T), alignment)), pBytes(new uint8_t[sizeof(T) + (sizeOfTPaddedToAlignment * size) - 1]) {}
	T & operator[](std::size_t index) {
		return *reinterpret_cast<T *>((uintptr_t(pBytes.get()) + (sizeOfTPaddedToAlignment - 1) & ~uintptr_t(sizeOfTPaddedToAlignment - 1)) + (sizeOfTPaddedToAlignment * index));
	}
	T const & operator[](std::size_t index) const { return operator[](index); }
};

#endif