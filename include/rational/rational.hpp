#include <cassert>
#include <concepts>
#include <limits>
#include <numeric>
#include <ostream>

template <std::signed_integral IntType, std::floating_point RealType = double>
class Rational {
public:
    constexpr Rational() noexcept = default;

    constexpr Rational(const IntType numerator, const IntType denominator = 1) noexcept
        : m_num(numerator)
        , m_den(denominator)
    {
        assert(m_den != 0);

        // Reduce denominator if numerator is zero
        if (m_num == 0) {
            m_den = 1;
            return;
        }

        // Reduce fraction
        // Flip sign if both halves of fraction are negative
        auto gcd = std::gcd(m_num, m_den);
        if (m_num < 0 && m_den < 0)
            gcd *= -1;
        m_num /= gcd;
        m_den /= gcd;
    }

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

    constexpr Rational& operator+=(const Rational& value) noexcept
    {
        return *this = *this + value;
    }

    [[nodiscard]] constexpr Rational operator-(const Rational& value) const noexcept
    {
        return { *this + -value };
    }

    constexpr Rational& operator-=(const Rational& value) noexcept
    {
        return *this = *this - value;
    }

    [[nodiscard]] constexpr Rational operator*(const Rational& value) const noexcept
    {
        return { IntType(m_num * value.m_num), IntType(m_den * value.m_den) };
    }

    constexpr Rational& operator*=(const Rational& value) noexcept
    {
        return *this = *this * value;
    }

    [[nodiscard]] constexpr Rational operator/(const Rational& value) const noexcept
    {
        return { IntType(m_num * value.m_den), IntType(m_den * value.m_num) };
    }

    constexpr Rational& operator/=(const Rational& value) noexcept
    {
        return *this = *this / value;
    }

    [[nodiscard]] constexpr auto operator<=>(const Rational& value) const noexcept
    {
        const auto [lhs, rhs, _] = to_common_denominator(value);
        return lhs <=> rhs;
    }

    [[nodiscard]] constexpr bool operator==(const Rational& value) const noexcept = default;

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
        const auto lhs_den = m_den;
        const auto rhs_den = value.m_den;
        const auto lcm = std::lcm(lhs_den, rhs_den);
        const auto lhs_factor = lcm / lhs_den;
        const auto rhs_factor = lcm / rhs_den;

        return { IntType(m_num * lhs_factor), IntType(value.m_num * rhs_factor), lcm };
    }

    template <typename T>
    friend std::ostream& operator<<(std::ostream& out, const Rational<T>& value);

    IntType m_num {};
    IntType m_den { 1 };
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const Rational<T>& value)
{
    return out << "(" << +value.m_num << " / " << +value.m_den << ")";
}

template <typename IntType, typename RealType>
struct std::numeric_limits<Rational<IntType, RealType>> : std::numeric_limits<IntType> {
    static constexpr bool is_integer { false };

    static constexpr Rational<IntType, RealType> min() noexcept
    {
        return { std::numeric_limits<IntType>::min() };
    }

    static constexpr Rational<IntType, RealType> lowest() noexcept
    {
        return { std::numeric_limits<IntType>::lowest() };
    }

    static constexpr Rational<IntType, RealType> max() noexcept
    {
        return { std::numeric_limits<IntType>::max() };
    }
};
