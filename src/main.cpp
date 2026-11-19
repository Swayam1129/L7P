#include <type_traits>

#include "pack.h"

using P = Pack<int, double, char, int>;
using E = Pack<>;

// size
static_assert(E::size == 0);
static_assert(P::size == 4);
static_assert(Pack<float>::size == 1);

// index -> type
static_assert(std::is_same_v<P::type<0>, int>);
static_assert(std::is_same_v<P::type<1>, double>);
static_assert(std::is_same_v<P::type<2>, char>);
static_assert(std::is_same_v<P::type<3>, int>);
// P::type<4> or P::type<-1> would fail to compile (out of range).

// type -> index
static_assert(P::index<int> == 0);  // first occurrence
static_assert(P::index<double> == 1);
static_assert(P::index<char> == 2);
static_assert(P::index<float> == -1);  // not in pack
static_assert(E::index<int> == -1);

int main() { return 0; }
