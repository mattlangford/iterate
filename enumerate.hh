namespace it
{
namespace detail
{
template <typename It>
struct EnumerateIterator {
    size_t index;
    It element;

    EnumerateIterator& operator++()
    {
        ++index;
        ++element;
        return *this;
    }

    bool operator!=(const EnumerateIterator& rhs) const
    {
        return element != rhs.element;
    }

    using T = typename std::iterator_traits<It>::value_type;
    std::pair<size_t, T&> operator*() const
    {
        return { index, *element };
    }
};

template <typename Container>
struct Enumerate {
    using It = typename Container::iterator;

    using iterator = EnumerateIterator<It>;
    iterator begin() const
    {
        return EnumerateIterator<It> { 0, start_ };
    }
    iterator end() const
    {
        return EnumerateIterator<It> { 0, end_ };
    }

    It start_;
    It end_;

};
}


template <typename Container>
Enumerate<Container> enumerate(Container& container)
{
    return {container.begin(), container.end()};
}
}
