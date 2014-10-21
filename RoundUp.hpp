#ifndef ROUND_UP_HPP_INCLUDED
#define ROUND_UP_HPP_INCLUDED

#define ROUND_UP(from, to) from + to - 1 - (from - 1) % to

template<typename T>
inline T roundUp(T from, T to) {
	return ROUND_UP(from, to);
}

template<typename T, T From, T To>
struct RoundUp {
	static const T Result = ROUND_UP(From, To);
};

#undef ROUND_UP

#endif