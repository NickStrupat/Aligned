#ifndef ROUNDUP_HPP_INCLUDED
#define ROUNDUP_HPP_INCLUDED

template<size_t From, size_t To>
struct RoundUp {
	enum { value = From + To - 1 - (From -1) % To };
};

#endif