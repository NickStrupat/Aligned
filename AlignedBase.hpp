#ifndef ALIGNEDBASE_HPP_INCLUDED
#define ALIGNEDBASE_HPP_INCLUDED

#include <cstddef>
#include <exception>
#include "CacheLineSize/cacheLineSize.h"

#define ALIGNMENT_MUST_BE_A_MULTIPLE_OF_2 "Alignment must be a multiple of 2"

template<std::size_t Alignment = -1>
class AlignedBase {
	static_assert(Alignment % 2 == 0, ALIGNMENT_MUST_BE_A_MULTIPLE_OF_2);
};

template<>
class AlignedBase<-1> {
public:
	class AlignmentException : public std::exception {
		friend class AlignedBase<-1>;
		AlignmentException() : std::exception(ALIGNMENT_MUST_BE_A_MULTIPLE_OF_2) {}
	};
protected:
	AlignedBase(std::size_t alignment) {
		if (alignment % 2 != 0)
			throw new AlignmentException();
	}
	static std::size_t cacheLineSize() {
		static std::size_t cacheLineSize = cache_line_size();
		return cacheLineSize;
	}
};

#undef ALIGNMENT_MUST_BE_A_MULTIPLE_OF_2

#endif