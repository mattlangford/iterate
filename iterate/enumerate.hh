#pragma once

#include <iterator>

#include "traits.hh"

namespace it {
namespace detail {

template <typename It>
struct EnumerateIterator {
    size_t index;
    It element;

    EnumerateIterator &operator++() {
        ++index;
        ++element;
        return *this;
    }

    using difference_type = int;
    using value_type = std::pair<size_t, ReferenceType<It>>;
    using reference = value_type;
    using pointer = std::add_pointer<value_type>;
    using iterator_category = std::forward_iterator_tag;

    // NOTE: This may return a const or mutable reference depending on the constness of the iterator
    reference operator*() const { return {index, *element}; }

    bool operator==(const EnumerateIterator &rhs) const { return element == rhs.element; }
    bool operator!=(const EnumerateIterator &rhs) const { return element != rhs.element; }
    bool operator<(const EnumerateIterator &rhs) const { return element < rhs.element; }
};

/// Accepts by value, reference, or rvalue reference
template <typename Container>
struct Enumerate {
    using iterator = EnumerateIterator<typename std::decay_t<Container>::iterator>;
    using const_iterator = EnumerateIterator<typename std::decay_t<Container>::const_iterator>;
    using reverse_iterator = EnumerateIterator<typename std::decay_t<Container>::reverse_iterator>;
    using const_reverse_iterator = EnumerateIterator<typename std::decay_t<Container>::const_reverse_iterator>;

    iterator begin() { return {0, container.begin()}; }
    iterator end() { return {container.size(), container.end()}; }
    const_iterator cbegin() const { return {0, container.cbegin()}; }
    const_iterator cend() const { return {container.size(), container.cend()}; }
    reverse_iterator rbegin() { return {0, container.rbegin()}; }
    reverse_iterator rend() { return {container.size(), container.rend()}; }
    const_reverse_iterator crbegin() const { return {0, container.crbegin()}; }
    const_reverse_iterator crend() const { return {container.size(), container.crend()}; }
    size_t size() const { return container.size(); }

    Container container;
};

/// Provide specialization for when the Container is const
template <typename Container>
struct Enumerate<const Container &> {
    using iterator = EnumerateIterator<typename std::decay_t<Container>::const_iterator>;
    using const_iterator = EnumerateIterator<typename std::decay_t<Container>::const_iterator>;
    using reverse_iterator = EnumerateIterator<typename std::decay_t<Container>::const_reverse_iterator>;
    using const_reverse_iterator = EnumerateIterator<typename std::decay_t<Container>::const_reverse_iterator>;

    iterator begin() const { return {0, container.begin()}; }
    iterator end() const { return {container.size(), container.end()}; }
    const_iterator cbegin() const { return {0, container.cbegin()}; }
    const_iterator cend() const { return {container.size(), container.cend()}; }
    reverse_iterator rbegin() const { return {0, container.crbegin()}; }
    reverse_iterator rend() const { return {container.size(), container.crend()}; }
    const_reverse_iterator crbegin() const { return {0, container.crbegin()}; }
    const_reverse_iterator crend() const { return {container.size(), container.crend()}; }
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
detail::Enumerate<Container &> enumerate(Container &container) {
    return {container};
}
template <typename Container>
detail::Enumerate<Container &> enumerate(Container &&container) {
    return {container};
}
template <typename Container>
detail::Enumerate<Container> enumerate_copy(const Container &container) {
    return {container};
}
}  // namespace it
