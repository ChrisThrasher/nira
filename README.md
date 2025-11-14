# Nira

[![CI](https://github.com/ChrisThrasher/nira/actions/workflows/ci.yml/badge.svg)](https://github.com/ChrisThrasher/nira/actions/workflows/ci.yml)

Nira is a header-only C++20 library that provides `constexpr` arithmetic types that model different types of numbers.
The types provided implement arithmetic operations like addition, subtraction, multiplication, division, and comparisons.
They can be used as drop-in replacements for built-in arithmetic types like `int` or `float`.

## `nira::Rational`

`Rational` models a [rational number](https://en.wikipedia.org/wiki/Rational_number), a number which can be represented as a fraction.
Rationals are automatically reduced to their simplest form.

```cpp
#include <nira/rational.hpp>
...

nira::Rational value(10, 3); // 10 / 3
value += nira::Rational(1, 2); // Add one half, new value is 23 / 6
double real_value = value.real(); // Convert to floating-point
```

By default `Rational` uses `int` for the underlying integer type.
You may supply any signed integer type as a template parameter:

```cpp
nira::Rational<std::int64_t> big_value(389'674'130'590'529, 30'943'876'602'937);
nira::Rational<char> small_value(17, 13);
```

## `nira::FixedPoint`

`FixedPoint` models a [fixed point number](https://en.wikipedia.org/wiki/Fixed-point_arithmetic), a number with a fixed number of fractional digits.

```cpp
#include <nira/fixed_point.hpp>
...

nira::FixedPoint<2> value(10, 50); // 10.50
value += nira::FixedPoint<2>(1, 25); // Add 1.25, new value is 11.75
auto whole = value.whole(); // Get whole part
auto fractional = value.fractional(); // Get fractional part
```

By default `FixedPoint` uses `int` for the underlying integer type.
You may supply any signed integer type as a template parameter:

```cpp
nira::FixedPoint<4, std::int64_t> precise_value(123'456, 7890); // 123456.7890
nira::FixedPoint<1, char> small_value(12, 3); // 12.3
```
