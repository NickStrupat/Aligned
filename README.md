Aligned
=======

Memory alignment wrappers, useful for avoiding false sharing

## Usage

Aligned<T, 64> gives you access to a stack allocated, uninitialized instance of T, which is aligned to a 64-byte boundary in memory (a memory address which is a multiple of 64), and padded out at least to the next 64-byte boundary. This effetively gives you a container to hold an object which you need to be on a boundary.

This is useful if you need to avoid false sharing in a concurrent system. For example, if your cache line size is 64 bytes, like most modern processors...

    Aligned<std::atomic<int>, 64> alignedCount = 0;

...will provide you with easy access to an `std::atomic<int>` instance which won't be subject to false sharing, since `alignedCount.reference()` returns a reference to a `std::atomic<int>` instance which lies alone in memory which is aligned and sized correctly to fit in the 64-byte cache line.

You can use the `std::atomic<int>` as you'd expect with `++count.reference();`, for example.

`T`s with `Alignment` unknown at compile time (are allocated on the heap, of course, and) are supported with

    Aligned<T> alignedFoo(cacheLineSize);

Stack allocated arrays are supported with

    Aligned<Foo[12], 64> alignedFoos;
    alignedFoos[0] = Foo();

Heap allocated arrays with `Alignment` unknown at compile time are supported with

    Aligned<Foo[12]> alignedFoos(cacheLineSize);
    
Heap allocated arrays with size and alignment unkown at compile time are supported with

    Aligned<Foo[]> alignedFoos(12, cacheLineSize();

More to come!
`std::tuple<T...>` support
