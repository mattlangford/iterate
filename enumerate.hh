#pragma once

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

    // NOTE: This may return a const or mutable reference depending on the constness of the iterator
    std::pair<size_t, ReferenceType<It>> operator*() const { return {index, *element}; }
    bool operator!=(const EnumerateIterator &rhs) const { return element != rhs.element; }
};

template <typename Container>
struct Enumerate {
    using iterator = EnumerateIterator<IteratorType<Container>>;
    iterator begin() const { return iterator{0, begin_it}; }
    iterator end() const { return iterator{0, end_it}; }

    IteratorType<Container> begin_it;
    IteratorType<Container> end_it;
};
}  // namespace detail

///
/// @brief Main interfaces
///
template <typename Container>
detail::Enumerate<Container> enumerate(Container &container) {
    return {container.begin(), container.end()};
}
template <typename Container>
detail::Enumerate<Container> enumerate(const Container &container) {
    return {container.cbegin(), container.cend()};
}
}  // namespace it
