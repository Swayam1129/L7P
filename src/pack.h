#ifndef PACK_H
#define PACK_H

namespace pack_detail {

// ---------- Size ----------
// Base case: empty list has size 0.
template <typename... Args>
constexpr int packSize = 0;

// Recursive case: one type + the rest.
template <typename T, typename... Args>
constexpr int packSize<T, Args...> = 1 + packSize<Args...>;

// ---------- Index -> type ----------
// Primary template. The requires clause rejects out-of-range indices at compile time
// (negative, or >= number of types). No definition needed: the specializations below cover
// every valid case.
template <int QueryIndex, typename... Args>
    requires(QueryIndex >= 0 && QueryIndex < packSize<Args...>)
struct _packIndexToType;

// Base case: index 0 -> the first type.
template <typename T, typename... Args>
struct _packIndexToType<0, T, Args...> {
    using type = T;
};

// Recursive case: drop the first type and look for index - 1 in the rest.
template <int QueryIndex, typename T, typename... Args>
struct _packIndexToType<QueryIndex, T, Args...> : _packIndexToType<QueryIndex - 1, Args...> {};

template <int QueryIndex, typename... Args>
using packIndexToType = typename _packIndexToType<QueryIndex, Args...>::type;

// ---------- Type -> index ----------
// Tail-recursive helper. Accumulated = index of the first type in Args.
// Base case: list is empty, QueryType was never found.
template <int Accumulated, typename QueryType, typename... Args>
constexpr int _packTypeToIndex = -1;

// Found: the first type in the list is QueryType.
template <int Accumulated, typename QueryType, typename... Args>
constexpr int _packTypeToIndex<Accumulated, QueryType, QueryType, Args...> = Accumulated;

// Not found yet: skip the first type and move one index forward.
template <int Accumulated, typename QueryType, typename T, typename... Args>
constexpr int _packTypeToIndex<Accumulated, QueryType, T, Args...> =
    _packTypeToIndex<Accumulated + 1, QueryType, Args...>;

template <typename QueryType, typename... Args>
constexpr int packTypeToIndex = _packTypeToIndex<0, QueryType, Args...>;

}  // namespace pack_detail

template <typename... Args>
struct Pack {
    static constexpr int size = pack_detail::packSize<Args...>;

    template <int QueryIndex>
    using type = pack_detail::packIndexToType<QueryIndex, Args...>;

    template <typename QueryType>
    constexpr static int index = pack_detail::packTypeToIndex<QueryType, Args...>;
};

#endif  // PACK_H
