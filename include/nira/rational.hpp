#pragma once

#include <cassert>
#include <concepts>
#include <limits>
#include <numeric>
#include <ostream>

namespace nira {
template <std::signed_integral IntType = int>
class Rational {
public:
    constexpr Rational() noexcept = default;

    constexpr Rational(const IntType numerator, const IntType denominator = 1) noexcept
        : m_num(numerator)
        , m_den(denominator)
    {
        assert(m_den != 0);

        // Reduce fraction
        // Flip signs if both halves of fraction are non-positive
        auto gcd = std::gcd(m_num, m_den);
        if (m_num <= 0 && m_den < 0)
            gcd *= -1;
        m_num /= gcd;
        m_den /= gcd;
    }

    template <typename U>
    constexpr Rational(const Rational<U>& value)
        requires(sizeof(IntType) > sizeof(U))
        : m_num(value.m_num)
        , m_den(value.m_den)
    {
    }

    template <std::floating_point RealType = double>
    [[nodiscard]] constexpr RealType real() const noexcept
    {
        return RealType(m_num) / RealType(m_den);
    }

    [[nodiscard]] constexpr Rational operator-() const noexcept
    {
        return { IntType(-m_num), m_den };
    }

    [[nodiscard]] constexpr Rational operator+(const Rational& value) const noexcept
    {
        const auto [lhs, rhs, lcm] = to_common_denominator(value);
        return { IntType(lhs + rhs), lcm };
    }

    constexpr Rational& operator+=(const Rational& value) & noexcept
    {
        return *this = *this + value;
    }

    [[nodiscard]] constexpr Rational operator-(const Rational& value) const noexcept
    {
        return { *this + -value };
    }

    constexpr Rational& operator-=(const Rational& value) & noexcept
    {
        return *this = *this - value;
    }

    [[nodiscard]] constexpr Rational operator*(const Rational& value) const noexcept
    {
        return { IntType(m_num * value.m_num), IntType(m_den * value.m_den) };
    }

    constexpr Rational& operator*=(const Rational& value) & noexcept
    {
        return *this = *this * value;
    }

    [[nodiscard]] constexpr Rational operator/(const Rational& value) const noexcept
    {
        return { IntType(m_num * value.m_den), IntType(m_den * value.m_num) };
    }

    constexpr Rational& operator/=(const Rational& value) & noexcept
    {
        return *this = *this / value;
    }

    [[nodiscard]] constexpr bool operator==(const Rational& value) const noexcept = default;

    [[nodiscard]] constexpr auto operator<=>(const Rational& value) const noexcept
    {
        const auto [lhs, rhs, _] = to_common_denominator(value);
        return lhs <=> rhs;
    }

private:
    struct ScaledNumerators {
        IntType lhs {};
        IntType rhs {};
        IntType lcm {};
    };

    [[nodiscard]] constexpr ScaledNumerators to_common_denominator(const Rational& value) const noexcept
    {
        // Scale each such fraction such that the denominators are equal to their least commmon multiple.
        // This minimizes how much we have to raise the magnitude of the numerator which improves precision.
        const auto lcm = std::lcm(m_den, value.m_den);
        return { IntType(m_num * (lcm / m_den)), IntType(value.m_num * (lcm / value.m_den)), lcm };
    }

    // Required for converting constructor
    template <std::signed_integral>
    friend class Rational;

    template <typename T>
    friend std::ostream& operator<<(std::ostream& out, const Rational<T>& value);

    IntType m_num { 0 };
    IntType m_den { 1 };
};
}

template <typename IntType>
std::ostream& operator<<(std::ostream& out, const nira::Rational<IntType>& value)
{
    return out << "(" << +value.m_num << " / " << +value.m_den << ")";
}

template <typename IntType>
struct std::numeric_limits<nira::Rational<IntType>> : std::numeric_limits<IntType> {
    static constexpr bool is_integer { false };

    static constexpr nira::Rational<IntType> min() noexcept
    {
        return { std::numeric_limits<IntType>::min() };
    }

    static constexpr nira::Rational<IntType> lowest() noexcept
    {
        return { std::numeric_limits<IntType>::lowest() };
    }

    static constexpr nira::Rational<IntType> max() noexcept
    {
        return { std::numeric_limits<IntType>::max() };
    }
};
