#pragma once

#include <type_traits>

namespace it {
namespace detail {

template <typename Container>
struct Reverse {
    using iterator = typename std::decay_t<Container>::reverse_iterator;
    using const_iterator = typename std::decay_t<Container>::const_reverse_iterator;
    using reverse_iterator = typename std::decay_t<Container>::iterator;
    using const_reverse_iterator = typename std::decay_t<Container>::const_iterator;

    iterator begin() { return container.rbegin(); }
    iterator end() { return container.rend(); }
    const_iterator cbegin() const { return container.crbegin(); }
    const_iterator cend() const { return container.crend(); }
    reverse_iterator rbegin() { return container.begin(); }
    reverse_iterator rend() { return container.end(); }
    const_reverse_iterator crbegin() const { return container.cbegin(); }
    const_reverse_iterator crend() const { return container.cend(); }
    size_t size() const { return container.size(); }

    Container container;
};

template <typename Container>
struct Reverse<const Container &> {
    using iterator = typename std::decay_t<Container>::const_reverse_iterator;
    using const_iterator = typename std::decay_t<Container>::const_reverse_iterator;
    using reverse_iterator = typename std::decay_t<Container>::const_iterator;
    using const_reverse_iterator = typename std::decay_t<Container>::const_iterator;

    iterator begin() const { return container.crbegin(); }
    iterator end() const { return container.crend(); }
    const_iterator cbegin() const { return container.crbegin(); }
    const_iterator cend() const { return container.crend(); }
    reverse_iterator rbegin() const { return container.cbegin(); }
    reverse_iterator rend() const { return container.cend(); }
    const_reverse_iterator crbegin() const { return container.cbegin(); }
    const_reverse_iterator crend() const { return container.cend(); }
    size_t size() const { return container.size(); }

    const Container &container;
};
}  // namespace detail

///
/// @brief Main interfaces
///
/// NOTE: These will take const references as well, since the Container will be a const Container
///
template <typename Container>
detail::Reverse<Container &> reverse(Container &container) {
    return {container};
}
template <typename Container>
detail::Reverse<Container &> reverse(Container &&container) {
    return {container};
}
template <typename Container>
detail::Reverse<Container> reverse_copy(const Container &container) {
    return {container};
}
}  // namespace it
