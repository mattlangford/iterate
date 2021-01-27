# Iterate

The goal of this library is to provide simple python-like iteration of vectors, list, etc.

## Examples
In the case of iterating over multiple vectors (or really any container), instead of:
```
std::vector<DataType> data = ...;
std::vector<DataType> result = ...;
for (size_t i = 0; i < data.size(); ++i)
{
    const DataType& data_element = data[i];
    DataType& result_element = other_data[i];
    ...
    result_element += data_element;
}
```
this library allows for more python-like syntax with `it::enumerate()`:
```
#include "iterate/enumerate.hh"
...
std::vector<DataType> data = ...;
std::vector<DataType> result = ...;
for (auto [i, data_element] : it::enumerate(data))
{
    result[i] += data_element;
}
```
Note that even though it looks like `data_element` and `result_element` are being copied, they are both references. If you were to explicitly put the type (instead of auto), you'd use
```
for (std::pair<size_t, DataType&> element : it::enumerate(data))
{
    result[std::get<0>(element)] += std::get<1>(element);
}
```
which is a bit more nasty than the original version.

In this example, perhaps another way is with `it::zip`:
```
#include "iterate/zip.hh"
...
std::vector<DataType> data = ...;
std::vector<DataType> result = ...;
for (auto [data_element, result_element] : it::zip(data, result))
{
    result_element += data_element;
}
```
Each of the above take the vector by reference, but it works with const or rvalue elements as well. If you want a copy in the body of the for loop, use the `*_copy` method:
```
#include "iterate/iterate.hh"
...
for (auto [i, data_element] : it::enumerate_copy(data)) {...}
```

Another cool feature of the library is `it::reverse()`. Instead of making copies, or dealing with awkward indexing, how about:
```
#include "iterate/reverse.hh"
...
std::vector<DataType> process_backwards = ...;
for (const DataType& data : it::reverse(process_backwards))
{
    process(data);
}
```

## Notes
 - The header `iterate.hh` includes all functionality
 - Generally functions and be composed, for example: `it::enumerate(it::zip(a, b))`
 - Currently this only supports C++17 and up
 - I'm using bazel for this, but since it's header only including `iterate.hh` should be all that's needed

## TODO
 - `it::append()` which would iterate over each thing one at a time (like a double for loop)
