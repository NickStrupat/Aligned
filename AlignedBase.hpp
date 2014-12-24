#ifndef ALIGNED_BASE_HPP_INCLUDED
#define ALIGNED_BASE_HPP_INCLUDED

#include <cstddef>
#include <exception>
#include "RoundUp.hpp"
#include "IsPowerOfTwo.hpp"

#define ALIGNMENT_MUST_BE_A_POWER_OF_2_ERROR_MESSAGE "Alignment must be a power of 2"

template<typename T, std::size_t Alignment = -1>
class AlignedBase {
	static_assert(IsPowerOfTwo<std::size_t, Alignment>::Result, ALIGNMENT_MUST_BE_A_POWER_OF_2_ERROR_MESSAGE);
protected:
	static const std::size_t sizeOfTPaddedToAlignment = RoundUp<std::size_t, sizeof(T), Alignment>::Result;
};

template<typename T>
class AlignedBase<T, -1> {
public:
	class AlignmentException : public std::exception {
		friend class AlignedBase<T, -1>;
		AlignmentException() : std::exception(ALIGNMENT_MUST_BE_A_POWER_OF_2_ERROR_MESSAGE) {}
	};
	AlignedBase & operator=(AlignedBase const &) = delete;
protected:
	std::size_t const sizeOfTPaddedToAlignment;
	AlignedBase(std::size_t alignment) : sizeOfTPaddedToAlignment(roundUp(sizeof(T), alignment)) {
		if (!isPowerOfTwo(alignment))
			throw new AlignmentException();
	}
};

#undef ALIGNMENT_MUST_BE_A_POWER_OF_2_ERROR_MESSAGE

#endif