#pragma once

#include <stdexcept>
#include <tuple>

#include "traits.hh"

namespace it {
namespace detail {

/// Since other iterate iterators don't return references, we need this indirection instead of just std::ref() so we
/// don't mix up values and references.
template <typename T>
std::reference_wrapper<T> maybe_ref(T &t) {
    return t;
}
template <typename T>
T maybe_ref(T &&t) {
    return std::move(t);
}

template <typename... It>
struct ZipIterator {
    std::tuple<It...> element;

    ZipIterator &operator++() {
        std::apply([](auto &...el) { (++el, ...); }, element);
        return *this;
    }

    using difference_type = int;
    using value_type = std::tuple<ReferenceType<It>...>;
    using reference = value_type;
    using pointer = std::add_pointer<value_type>;
    using iterator_category = std::forward_iterator_tag;

    bool operator==(const ZipIterator &rhs) const { return element == rhs.element; }
    bool operator!=(const ZipIterator &rhs) const { return element != rhs.element; }

    reference operator*() const {
        return std::apply([](auto &...el) { return std::make_tuple(maybe_ref(*el)...); }, element);
    }
};

template <typename... Container>
struct Zip {
    Zip(Container... containers)
        : holder{std::make_tuple(ContainerWrapper<Container>{std::forward<Container>(containers)}...)} {
        // TODO: If the containers passed in are different sizes, things will go very poorly. Probably good to add a
        // way to handle that better than exceptions?
        auto throw_if_wrong_size = [this](const auto &el) {
            if (el.container.size() != size()) throw std::runtime_error("Zipping with different sized inputs!");
        };
        std::apply([&](auto &...el) { (throw_if_wrong_size(el), ...); }, holder);
    }
    std::tuple<ContainerWrapper<Container>...> holder;

    using iterator = ZipIterator<typename ContainerWrapper<Container>::iterator...>;
    using const_iterator = ZipIterator<typename ContainerWrapper<Container>::const_iterator...>;
    using reverse_iterator = ZipIterator<typename ContainerWrapper<Container>::reverse_iterator...>;
    using const_reverse_iterator = ZipIterator<typename ContainerWrapper<Container>::const_reverse_iterator...>;

    template <typename F>
    auto apply_to_each(F f) {
        return std::apply([&f](auto &...el) { return std::make_tuple(f(el)...); }, holder);
    }

    iterator begin() {
        return {apply_to_each([](auto &el) { return el.begin(); })};
    }
    iterator end() {
        return {apply_to_each([](auto &el) { return el.end(); })};
    }
    const_iterator cbegin() const {
        return {apply_to_each([](auto &el) { return el.cbegin(); })};
    }
    const_iterator cend() const {
        return {apply_to_each([](auto &el) { return el.cend(); })};
    }
    reverse_iterator rbegin() const {
        return {apply_to_each([](auto &el) { return el.rbegin(); })};
    }
    reverse_iterator rend() const {
        return {apply_to_each([](auto &el) { return el.rend(); })};
    }
    const_reverse_iterator crbegin() const {
        return {apply_to_each([](auto &el) { return el.crbegin(); })};
    }
    const_reverse_iterator crend() const {
        return {apply_to_each([](auto &el) { return el.crend(); })};
    }
    size_t size() const { return std::get<0>(holder).container.size(); }
};
}  // namespace detail

///
/// @brief Main interfaces
///
/// NOTE: These will take const references as well, since the Containers will be a const Containers
///
template <typename... Container>
detail::Zip<Container &...> zip(Container &...containers) {
    return {containers...};
}
template <typename... Container>
detail::Zip<Container &...> zip(Container &&...containers) {
    return {containers...};
}
template <typename... Container>
detail::Zip<Container...> zip_copy(const Container &...containers) {
    return {containers...};
}
}  // namespace it
