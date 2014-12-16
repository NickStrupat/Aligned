#ifndef IS_POWER_OF_TWO_HPP_INCLUDED
#define IS_POWER_OF_TWO_HPP_INCLUDED

#define IS_POWER_OF_TWO(x) ((x != 0) && ((x & (x - 1)) == 0))

template<typename T>
inline bool isPowerOfTwo(T x) {
	return IS_POWER_OF_TWO(x);
}

template<typename T, T X>
struct IsPowerOfTwo {
	static const bool Result = IS_POWER_OF_TWO(X);
};

#undef IS_POWER_OF_TWO

#endif