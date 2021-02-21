#include <iterator>

namespace it::detail {
///
/// @brief Decide if the input iterate is const or not and return the proper iterator type
///
template <typename T>
using IteratorType = std::conditional_t<std::is_const_v<T>, typename std::decay_t<T>::const_iterator,
                                        typename std::decay_t<T>::iterator>;

///
/// @brief Fetch the reference type of the iterator
///
template <typename T>
using ReferenceType = typename std::iterator_traits<T>::reference;

///
/// @brief Wraps a container. This redefines some types (specifically when the container is const) so things play nicely
///
template <typename Container>
struct ContainerWrapper {
    using iterator = typename std::decay_t<Container>::iterator;
    using const_iterator = typename std::decay_t<Container>::const_iterator;
    using reverse_iterator = typename std::decay_t<Container>::reverse_iterator;
    using const_reverse_iterator = typename std::decay_t<Container>::const_reverse_iterator;

    iterator begin() { return container.begin(); }
    iterator end() { return container.end(); }
    const_iterator cbegin() const { return container.cbegin(); }
    const_iterator cend() const { return container.cend(); }
    reverse_iterator rbegin() { return container.rbegin(); }
    reverse_iterator rend() { return container.rend(); }
    const_reverse_iterator crbegin() const { container.crbegin(); }
    const_reverse_iterator crend() const { return container.crend(); }

    Container container;
};

template <typename Container>
struct ContainerWrapper<const Container &> {
    using iterator = typename std::decay_t<Container>::const_iterator;
    using const_iterator = typename std::decay_t<Container>::const_iterator;
    using reverse_iterator = typename std::decay_t<Container>::const_reverse_iterator;
    using const_reverse_iterator = typename std::decay_t<Container>::const_reverse_iterator;

    iterator begin() const { return container.cbegin(); }
    iterator end() const { return container.cend(); }
    const_iterator cbegin() const { return container.cbegin(); }
    const_iterator cend() const { return container.cend(); }
    reverse_iterator rbegin() const { return container.crbegin(); }
    reverse_iterator rend() const { return container.crend(); }
    const_reverse_iterator crbegin() const { container.crbegin(); }
    const_reverse_iterator crend() const { return container.crend(); }

    const Container &container;
};
}  // namespace it::detail
