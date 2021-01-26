#pragma once

#include <tuple>

#include "traits.hh"

namespace it {
namespace detail {

template <typename... It>
struct ZipIterator {
    std::tuple<It...> element;

    ZipIterator &operator++() {
        std::apply([](auto &...el) { (++el, ...); }, element);
        return *this;
    }

    bool operator!=(const ZipIterator &rhs) const { return element != rhs.element; }

    std::tuple<ReferenceType<It>...> operator*() const {
        return std::apply([](auto &...el) { return std::make_tuple(std::ref(*el)...); }, element);
    }
};

template <typename... Container>
struct Zip {
    std::tuple<Container &...> holder;

    using iterator = ZipIterator<IteratorType<Container>...>;
    iterator begin() const {
        return {std::apply([](auto &...el) { return std::make_tuple(el.begin()...); }, holder)};
    }
    iterator end() const {
        return {std::apply([](auto &...el) { return std::make_tuple(el.end()...); }, holder)};
    }
};
template <typename... Container>
struct ZipCopy {
    std::tuple<Container...> holder;

    using iterator = ZipIterator<IteratorType<Container>...>;
    iterator begin() {
        return {std::apply([](auto &...el) { return std::make_tuple(el.begin()...); }, holder)};
    }
    iterator end() {
        return {std::apply([](auto &...el) { return std::make_tuple(el.end()...); }, holder)};
    }
};
}  // namespace detail

///
/// @brief Main interfaces
///
/// NOTE: These will take const references as well, since the Containers will be a const Containers
///
template <typename... Container>
detail::Zip<Container...> zip(Container &...containers) {
    return {std::forward_as_tuple(containers...)};
}
template <typename... Container>
detail::Zip<Container...> zip(Container &&...containers) {
    return {std::forward_as_tuple(containers...)};
}
template <typename... Container>
detail::ZipCopy<Container...> zip_copy(const Container &...containers) {
    return {std::make_tuple(containers...)};
}
}  // namespace it
