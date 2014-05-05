#ifndef ISPOWEROFTWO_HPP_INCLUDED
#define ISPOWEROFTWO_HPP_INCLUDED

#define ISPOWEROFTWO(x) (x != 0) && ((x & (x - 1)) == 0)

template<typename T>
inline bool isPowerOfTwo(T x) {
	return ISPOWEROFTWO(x);
}

template<typename T, T X>
struct IsPowerOfTwo {
	static const bool Result = ISPOWEROFTWO(X);
};

#undef ISPOWEROFTWO

#endif