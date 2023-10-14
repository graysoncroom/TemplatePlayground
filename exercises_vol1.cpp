#include <iostream>
#include <type_traits>

// Find the challenge here: https://www.slamecka.cz/posts/2021-03-17-cpp-metaprogramming-exercises-1/

namespace {

/**
 * 1. Define Vector, a template level list of integers.
 * Hint: Use non-type template parameter pack.
 */

// Your code goes here:
template <int ...>
struct Vector {};
// ^ Your code goes here

static_assert(std::is_same_v<Vector<1,2>, Vector<1,2>>);


/**
 * 2. Define function print() that prints Vector-s.
 * Example: print(Vector<1,2,3>{}); // prints "1 2 3" (newline)
 * See main() below.
 */

// Your code goes here:
template <typename T>
void print(T x) {}

template <int N, int ...Ns>
void print(Vector<N, Ns...> x) {
    std::cout << N << ' ';
    print(Vector<Ns...>{});
}

template <int N>
void print(Vector<N> x) {
    std::cout << N << std::endl;
}
// ^ Your code goes here

/**
 * 3. Define Prepend.
 * Hint: Use `using type = ...` inside a struct that has both non-type and type template parameters.
 */

// Your code goes here:
template <int X, typename V>
struct Prepend {};

template <int X, int ...Xs>
struct Prepend<X, Vector<Xs...>> {
    using type = Vector<X, Xs...>;
};
// ^ Your code goes here

static_assert(std::is_same_v<Prepend<1, Vector<2,3>>::type, Vector<1,2,3>>);


/**
 * 4. Define PrependT that can be used without ::type.
 * Hint: See how enable_if_t is defined in terms of enable_if.
 *
 * This technique is not used further to reduce boilerplate.
 */

// Your code goes here:
template <int X, typename V>
using PrependT = typename Prepend<X, V>::type;

// ^ Your code goes here

static_assert(std::is_same_v<PrependT<1, Vector<2,3>>, Vector<1,2,3>>);


/**
 * 5. Define Append.
 */

// Your code goes here:
template <int, typename>
struct Append {};

template <int X, int ...Xs>
struct Append<X, Vector<Xs...>> {
    using type = Vector<Xs..., X>;
};
// ^ Your code goes here

static_assert(std::is_same_v< Append<4, Vector<1,2,3>>::type , Vector<1,2,3,4> >);


/**
 * 6. Define PopBack.
 */

// Your code goes here:
template <typename>
struct PopBack {};

template <int X, int ...Xs>
struct PopBack<Vector<X, Xs...>> {
    using type = typename Prepend< X, typename PopBack< Vector<Xs...> >::type >::type;
};

template <int X>
struct PopBack<Vector<X>> {
    using type = Vector<>;
};
// ^ Your code goes here

static_assert(std::is_same_v< PopBack<Vector<1,2,3,4>>::type , Vector<1,2,3> >);


/**
 * 7. Define RemoveFirst, that removes the first occurence of element R from vector V.
 */

// Your code goes here:
template <int R, typename V>
struct RemoveFirst {};

template <int R, int X, int ...Xs>
struct RemoveFirst<R, Vector<X, Xs...>> {
    using type = typename Prepend< X, typename RemoveFirst< R, Vector<Xs...> >::type >::type;
};

template <int R, int ...Xs>
struct RemoveFirst<R, Vector<R, Xs...>> {
    using type = Vector<Xs...>;
};
// ^ Your code goes here

static_assert(std::is_same_v<RemoveFirst<1, Vector<1,1,2>>::type, Vector<1,2>>);


/**
 * 8. Define RemoveAll, that removes all occurences of element R from vector V.
 */

// Your code goes here:
template <int R, typename V>
struct RemoveAll {};

template <int R, int X, int ...Xs>
struct RemoveAll<R, Vector<X, Xs...>> {
    using type = typename Prepend <X, typename RemoveAll< R, Vector<Xs...> >::type >::type;
};

template <int R, int ...Xs>
struct RemoveAll<R, Vector<R, Xs...>> {
    using type = typename RemoveAll< R, Vector<Xs...> >::type;
};

template <int R>
struct RemoveAll<R, Vector<>> {
    using type = Vector<>;
};
// ^ Your code goes here

static_assert(std::is_same_v<RemoveAll<9, Vector<1,9,2,9,3,9>>::type, Vector<1,2,3>>);

/**
 * 9. Define Length.
 * Hint: Use `static constexpr int value = ...` inside the struct.
 */

// Your code goes here:
template <typename V>
struct Length {};

template <int X, int ...Xs>
struct Length<Vector<X, Xs...>> {
    static constexpr int value = 1 + Length<Vector<Xs...>>::value;
};

template <>
struct Length<Vector<>> {
    static constexpr int value = 0;
};
// ^ Your code goes here

static_assert(Length<Vector<1,2,3>>::value == 3);


/**
 * 10. Define length, which works like Length<V>::value.
 * Hint: See how is_same_v is defined in terms of is_same.
 */

// Your code goes here:

template <typename V>
inline constexpr int length = Length<V>::value;
// ^ Your code goes here

// static_assert(length<Vector<>> == 0);
// static_assert(length<Vector<1,2,3>> == 3);


/**
 * 11. Define Min, that stores the minimum of a vector in its property `value`.
 */

// Your code goes here:
template <int X, int Y>
struct LessThan { static constexpr bool value = X < Y; };

template <typename V>
struct Min {};

template <int X1, int X2, int ...Xs>
struct Min<Vector<X1, X2, Xs...>> {
    constexpr static int value = Min< Vector< (X1 <= X2 ? X1 : X2), Xs... > >::value;
};

template <int X>
struct Min<Vector<X>> { constexpr static int value = X; };
// ^ Your code goes here

static_assert(Min<Vector<3,1,2>>::value == 1);
static_assert(Min<Vector<1,2,3>>::value == 1);
static_assert(Min<Vector<3,2,1>>::value == 1);


/**
 * 12. Define Sort.
 */

// Your code goes here:
template <typename V, int min = Min<V>::value>
struct Sort {
    using type = typename Prepend< min, typename Sort< typename RemoveFirst<min, V>::type >::type >::type;
};

template <int X>
struct Sort<Vector<X>> {
    using type = Vector<X>;
};
// ^ Your code goes here

static_assert(std::is_same_v<Sort<Vector<4,1,2,5,6,3>>::type, Vector<1,2,3,4,5,6>>);
static_assert(std::is_same_v<Sort<Vector<3,3,1,1,2,2>>::type, Vector<1,1,2,2,3,3>>);
static_assert(std::is_same_v<Sort<Vector<2,2,1,1,3,3>>::type, Vector<1,1,2,2,3,3>>);


/**
 * 13. Define Uniq.
 */

// Your code goes here:
template <typename V>
struct Uniq {};

template <int X, int ...Xs>
struct Uniq<Vector<X, Xs...>> {
    using type = typename Prepend<X, typename Uniq< Vector<Xs...> >::type>::type;
    // using type = typename Uniq<typename Prepend<X, Vector<Xs...>>::type >::type;
};

template <int X, int ...Xs>
struct Uniq<Vector<X, X, Xs...>> {
    using type = typename Uniq<typename Prepend<X, Vector<Xs...>>::type >::type;
};

template <>
struct Uniq<Vector<>> {
    using type = Vector<>;
};
// ^ Your code goes here

static_assert(std::is_same_v<Uniq<Vector<1,1,2,2,1,1>>::type, Vector<1,2,1>>);
static_assert(std::is_same_v< Uniq<Vector<1,1,1,1,2,2,2,1,1,2,2,1,1>>::type, Vector<1,2,1,2,1> >);



/**
 * 14. Define type Set.
 */

// Your code goes here:
template <int ...Xs>
struct Set {
    using type = typename Uniq< typename Sort<Vector<Xs...>>::type >::type;
};
// ^ Your code goes here

static_assert(std::is_same_v<Set<2,1,3,1,2,3>::type, Set<1,2,3>::type>);


/**
 * 15. Define SetFrom.
 */

// Your code goes here:
template <typename V>
struct SetFrom {};

template <int ...Xs>
struct SetFrom<Vector<Xs...>> {
    using type = typename Set<Xs...>::type;
};
// ^ Your code goes here

static_assert(std::is_same_v<SetFrom<Vector<2,1,3,1,2,3>>::type, Set<1,2,3>::type>);


/**
 * 16. Define Get for Vector.
 * Provide an improved error message when accessing outside of Vector bounds.
 */

// Your code goes here:
template <int N, typename V>
struct Get {};

template <int N>
struct Get<N, Vector<>> { 
    static constexpr char value[] = "ERROR: Index out of bounds"; 
};

template <int N, int X, int ...Xs>
struct Get<N, Vector<X, Xs...>> {
    static constexpr int value = Get<N-1, Vector<Xs...>>::value;
};

template <int X, int ...Xs>
struct Get<0, Vector<X, Xs...>> {
    static constexpr int value = X;
};
// ^ Your code goes here

static_assert(Get<0, Vector<0,1,2>>::value == 0);
static_assert(Get<1, Vector<0,1,2>>::value == 1);
static_assert(Get<2, Vector<0,1,2>>::value == 2);
// static_assert(Get<9, Vector<0,1,2>>::value == 2); // How good is your error message?


/**
 * 17. Define BisectLeft for Vector.
 * Given n and arr, return the first index i such that arr[i] >= n.
 * If it doesn't exist, return the length of the array.
 *
 * Don't worry about complexity but aim for the binary search pattern.
 *
 * Hint: You might find it convenient to define a constexpr helper function.
 */

// Your code goes here:
// ^ Your code goes here

// static_assert(BisectLeft<  3, Vector<0,1,2,3,4>>::value == 3);
// static_assert(BisectLeft<  3, Vector<0,1,2,4,5>>::value == 3);
// static_assert(BisectLeft<  9, Vector<0,1,2,4,5>>::value == 5);
// static_assert(BisectLeft< -1, Vector<0,1,2,4,5>>::value == 0);
// static_assert(BisectLeft<  2, Vector<0,2,2,2,2,2>>::value == 1);


/**
 * 18. Define Insert for Vector, it should take position, value and vector.
 * Don't worry about bounds.
 * Hint: use the enable_if trick, e.g.
 *   template<typename X, typename Enable = void> struct Foo;
 *   template<typename X> struct<std::enable_if_t<..something      about X..>> Foo {...};
 *   template<typename X> struct<std::enable_if_t<..something else about X..>> Foo {...};
 */

// Your code goes here:
// ^ Your code goes here

// static_assert(std::is_same_v<Insert<0, 3, Vector<4,5,6>>::type, Vector<3,4,5,6>>);
// static_assert(std::is_same_v<Insert<1, 3, Vector<4,5,6>>::type, Vector<4,3,5,6>>);
// static_assert(std::is_same_v<Insert<2, 3, Vector<4,5,6>>::type, Vector<4,5,3,6>>);
// static_assert(std::is_same_v<Insert<3, 3, Vector<4,5,6>>::type, Vector<4,5,6,3>>);

}

int main()
{
    print(Vector<>{});
    print(Vector<1>{});
    print(Vector<1,2,3,4,5,6>{});
//     std::cout << typeid(Vector<1,2,3,4,5,6>{}).name() << '\n';
}
