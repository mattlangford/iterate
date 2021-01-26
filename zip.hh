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

template <typename... Container>
struct Zip {
    Zip(Container... containers) : holder{std::make_tuple(ContainerWrapper<Container>{containers}...)} {}
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
