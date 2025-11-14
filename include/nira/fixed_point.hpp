#pragma once

#include <concepts>
#include <cstdint>
#include <iomanip>
#include <ostream>

namespace nira {
template <std::uint8_t scale, std::signed_integral IntType = int>
class FixedPoint {
    static_assert(scale > 0, "Scale factor must be greater than zero");

public:
    constexpr FixedPoint() noexcept = default;

    explicit constexpr FixedPoint(const IntType whole, const IntType fractional = 0) noexcept
        : m_value(IntType(whole * factor + fractional * sign(whole)))
    {
    }

    template <std::floating_point T>
    explicit constexpr FixedPoint(const T value) noexcept
        : m_value(IntType(value * factor))
    {
    }

    [[nodiscard]] constexpr IntType whole() const noexcept
    {
        return m_value / factor;
    }

    [[nodiscard]] constexpr IntType fractional() const noexcept
    {
        return abs(m_value - IntType(whole() * factor));
    }

    [[nodiscard]] constexpr FixedPoint operator-() const noexcept
    {
        auto fixed = *this;
        fixed.m_value *= -1;
        return fixed;
    }

    [[nodiscard]] constexpr FixedPoint operator+(FixedPoint fixed) const noexcept
    {
        fixed.m_value += m_value;
        return fixed;
    }

    constexpr FixedPoint& operator+=(const FixedPoint& fixed) & noexcept
    {
        m_value += fixed.m_value;
        return *this;
    }

    [[nodiscard]] constexpr FixedPoint operator-(FixedPoint fixed) const noexcept
    {
        fixed.m_value = m_value - fixed.m_value;
        return fixed;
    }

    constexpr FixedPoint& operator-=(const FixedPoint& fixed) & noexcept
    {
        m_value -= fixed.m_value;
        return *this;
    }

    [[nodiscard]] constexpr FixedPoint operator*(FixedPoint fixed) const noexcept
    {
        fixed.m_value *= m_value;
        fixed.m_value /= factor;
        return fixed;
    }

    constexpr FixedPoint& operator*=(const FixedPoint& fixed) & noexcept
    {
        m_value *= fixed.m_value;
        m_value /= factor;
        return *this;
    }

    [[nodiscard]] constexpr FixedPoint operator/(FixedPoint fixed) const noexcept
    {
        fixed.m_value = m_value * factor / fixed.m_value;
        return fixed;
    }

    constexpr FixedPoint& operator/=(const FixedPoint& fixed) & noexcept
    {
        m_value *= factor;
        m_value /= fixed.m_value;
        return *this;
    }

    [[nodiscard]] constexpr auto operator<=>(const FixedPoint&) const noexcept = default;

private:
    [[nodiscard]] static constexpr IntType abs(const IntType value) noexcept
    {
        if (value >= 0)
            return value;
        return IntType(-value);
    }

    [[nodiscard]] static constexpr IntType sign(const IntType value) noexcept
    {
        if (value >= 0)
            return 1;
        return -1;
    }

    [[nodiscard]] static consteval IntType power_of_ten() noexcept
    {
        auto exponent = scale;
        IntType value = 1;
        while (exponent-- > 0)
            value *= 10;
        return value;
    }

    static constexpr IntType factor = power_of_ten();

    IntType m_value { 0 };
};
}

template <std::uint8_t scale, typename IntType>
std::ostream& operator<<(std::ostream& out, const nira::FixedPoint<scale, IntType>& fixed)
{
    return out << +fixed.whole() << '.' << std::setfill('0') << std::setw(scale) << +fixed.fractional();
}
