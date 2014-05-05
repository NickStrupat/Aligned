#ifndef ROUNDUP_HPP_INCLUDED
#define ROUNDUP_HPP_INCLUDED

#define ROUNDUP(from, to) from + to - 1 - (from - 1) % to

template<typename T>
inline T roundUp(T from, T to) {
	return ROUNDUP(from, to);
}

template<typename T, T From, T To>
struct RoundUp {
	static const T Result = ROUNDUP(From, To);
};

#undef ROUNDUP

#endif