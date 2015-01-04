#ifndef ALIGNEDARRAYCONSTRUCTORCALLER_HPP_INCLUDED
#define ALIGNEDARRAYCONSTRUCTORCALLER_HPP_INCLUDED

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

#endif