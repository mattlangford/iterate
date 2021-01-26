#include <iterator>

namespace it::detail {
///
/// @brief Decide if the input iterate is const or not and return the proper iterator type
///
template <typename T>
using IteratorType = std::conditional_t<std::is_const_v<T>, typename T::iterator, typename T::const_iterator>;

///
/// @brief Fetch the reference type of the iterator
///
template <typename T>
using ReferenceType = typename std::iterator_traits<T>::reference;
}  // namespace it::detail
