#include <iterator>

namespace it::detail {
template <typename T>
using IteratorType = std::conditional_t<std::is_const_v<T>, typename T::iterator, typename T::const_iterator>;

template <typename T>
using ReferenceType = typename std::iterator_traits<T>::reference;
}  // namespace it::detail
