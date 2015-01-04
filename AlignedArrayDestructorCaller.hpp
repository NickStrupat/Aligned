#ifndef ALIGNEDARRAYDESTRUCTORCALLER_HPP_INCLUDED
#define ALIGNEDARRAYDESTRUCTORCALLER_HPP_INCLUDED

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

#endif