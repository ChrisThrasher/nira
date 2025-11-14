#include <nira/fixed_point.hpp>

#include <catch2/catch_template_test_macros.hpp>
#include <numbers>
#include <type_traits>

using nira::FixedPoint;

TEMPLATE_TEST_CASE("FixedPoint type traits", "", std::int8_t, std::int16_t, std::int32_t, std::int64_t)
{
    STATIC_CHECK(sizeof(FixedPoint<1, TestType>) == sizeof(TestType));
    STATIC_CHECK(alignof(FixedPoint<1, TestType>) == alignof(TestType));
    STATIC_CHECK(std::is_default_constructible_v<FixedPoint<1, TestType>>);
    STATIC_CHECK(std::is_trivially_copy_constructible_v<FixedPoint<1, TestType>>);
    STATIC_CHECK(std::is_trivially_copy_assignable_v<FixedPoint<1, TestType>>);
    STATIC_CHECK(std::is_trivially_move_constructible_v<FixedPoint<1, TestType>>);
    STATIC_CHECK(std::is_trivially_move_assignable_v<FixedPoint<1, TestType>>);
    STATIC_CHECK(std::is_nothrow_swappable_v<FixedPoint<1, TestType>>);
    STATIC_CHECK(std::has_unique_object_representations_v<FixedPoint<1, TestType>>);
    STATIC_CHECK(std::totally_ordered<FixedPoint<1, TestType>>);
    STATIC_CHECK(std::three_way_comparable<FixedPoint<1, TestType>>);
}

TEMPLATE_TEST_CASE("FixedPoint::FixedPoint()", "", std::int8_t, std::int16_t, std::int32_t, std::int64_t)
{
    STATIC_CHECK(FixedPoint<1, TestType>().whole() == 0);
    STATIC_CHECK(FixedPoint<1, TestType>().fractional() == 0);

    STATIC_CHECK(FixedPoint<2, TestType>().whole() == 0);
    STATIC_CHECK(FixedPoint<2, TestType>().fractional() == 0);

    STATIC_CHECK(FixedPoint<3, TestType>().whole() == 0);
    STATIC_CHECK(FixedPoint<3, TestType>().fractional() == 0);

    STATIC_CHECK(FixedPoint<4, TestType>().whole() == 0);
    STATIC_CHECK(FixedPoint<4, TestType>().fractional() == 0);
}

TEST_CASE("FixedPoint::FixedPoint(IntType)")
{
    constexpr FixedPoint<1, std::int8_t> value8(1);
    STATIC_CHECK(value8.whole() == 1);
    STATIC_CHECK(value8.fractional() == 0);

    constexpr FixedPoint<2, std::int16_t> value16(5);
    STATIC_CHECK(value16.whole() == 5);
    STATIC_CHECK(value16.fractional() == 0);

    constexpr FixedPoint<4, std::int32_t> value32(17);
    STATIC_CHECK(value32.whole() == 17);
    STATIC_CHECK(value32.fractional() == 0);

    constexpr FixedPoint<8, std::int64_t> value64(31);
    STATIC_CHECK(value64.whole() == 31);
    STATIC_CHECK(value64.fractional() == 0);
}

TEST_CASE("FixedPoint::FixedPoint(std::floating_point)", )
{
    constexpr FixedPoint<1, std::int8_t> pi1(std::numbers::pi);
    STATIC_CHECK(pi1.whole() == 3);
    STATIC_CHECK(pi1.fractional() == 1);

    constexpr FixedPoint<2, std::int16_t> pi2(std::numbers::pi);
    STATIC_CHECK(pi2.whole() == 3);
    STATIC_CHECK(pi2.fractional() == 14);

    constexpr FixedPoint<4, std::int16_t> pi4(std::numbers::pi);
    STATIC_CHECK(pi4.whole() == 3);
    STATIC_CHECK(pi4.fractional() == 1'415);

    constexpr FixedPoint<8, std::int32_t> pi8(std::numbers::pi);
    STATIC_CHECK(pi8.whole() == 3);
    STATIC_CHECK(pi8.fractional() == 14'159'265);

    constexpr FixedPoint<12, std::int64_t> pi12(std::numbers::pi);
    STATIC_CHECK(pi12.whole() == 3);
    STATIC_CHECK(pi12.fractional() == 141'592'653'589);

    constexpr FixedPoint<2> debt(-12'345.56);
    STATIC_CHECK(debt.whole() == -12'345);
    STATIC_CHECK(debt.fractional() == 56);
}

TEMPLATE_TEST_CASE("FixedPoint::operator-()", "", std::int8_t, std::int16_t, std::int32_t, std::int64_t)
{
    STATIC_CHECK(-FixedPoint<1, TestType>(2) == FixedPoint<1, TestType>(-2));
    STATIC_CHECK(-FixedPoint<1, TestType>(-42) == FixedPoint<1, TestType>(42));

    constexpr auto fixed1 = -FixedPoint<2>(91);
    STATIC_CHECK(fixed1.whole() == -91);
    STATIC_CHECK(fixed1.fractional() == 0);

    constexpr auto fixed2 = -FixedPoint<2>(-3.14);
    STATIC_CHECK(fixed2.whole() == 3);
    STATIC_CHECK(fixed2.fractional() == 14);
}

TEMPLATE_TEST_CASE("FixedPoint::operator+(FixedPoint)", "", std::int16_t, std::int32_t, std::int64_t)
{
    STATIC_CHECK(FixedPoint<1>() + FixedPoint<1>() == FixedPoint<1>());
    STATIC_CHECK(FixedPoint<2>(2, 3) + FixedPoint<2>(5, 9) == FixedPoint<2>(7, 12));
}

TEMPLATE_TEST_CASE("FixedPoint::operator+=(FixedPoint)", "", std::int16_t, std::int32_t, std::int64_t)
{
    FixedPoint<2> fixed(7.9);
    fixed += FixedPoint<2>(11.3);
    CHECK(fixed == FixedPoint<2>(19, 20));
}

TEMPLATE_TEST_CASE("FixedPoint::operator-(FixedPoint)", "", std::int16_t, std::int32_t, std::int64_t)
{
    STATIC_CHECK(FixedPoint<1>() - FixedPoint<1>() == FixedPoint<1>());
    STATIC_CHECK(FixedPoint<2>(2, 3) - FixedPoint<2>(5, 9) == FixedPoint<2>(-3, 6));
}

TEMPLATE_TEST_CASE("FixedPoint::operator-=(FixedPoint)", "", std::int16_t, std::int32_t, std::int64_t)
{
    FixedPoint<2> fixed(7.9);
    fixed -= FixedPoint<2>(11.3);
    CHECK(fixed == FixedPoint<2>(-3, 40));
}

TEMPLATE_TEST_CASE("FixedPoint::operator*(FixedPoint)", "", std::int16_t, std::int32_t, std::int64_t)
{
    STATIC_CHECK(FixedPoint<1>() * FixedPoint<1>() == FixedPoint<1>());
    STATIC_CHECK(FixedPoint<2>(2, 3) * FixedPoint<2>(5, 9) == FixedPoint<2>(10, 33));
}

TEMPLATE_TEST_CASE("FixedPoint::operator*=(FixedPoint)", "", std::int16_t, std::int32_t, std::int64_t)
{
    FixedPoint<2> fixed(7.9);
    fixed *= FixedPoint<2>(11.3);
    CHECK(fixed == FixedPoint<2>(89, 27));
}

TEMPLATE_TEST_CASE("FixedPoint::operator/(FixedPoint)", "", std::int16_t, std::int32_t, std::int64_t)
{
    STATIC_CHECK(FixedPoint<1>() / FixedPoint<1>(1) == FixedPoint<1>());
    STATIC_CHECK(FixedPoint<1>(2) / FixedPoint<1>(1) == FixedPoint<1>(2));
    STATIC_CHECK(FixedPoint<1>(10) / FixedPoint<1>(2) == FixedPoint<1>(5));
    STATIC_CHECK(FixedPoint<1>(10) / FixedPoint<1>(4) == FixedPoint<1>(2, 5));
    STATIC_CHECK(FixedPoint<1>(10) / FixedPoint<1>(9) == FixedPoint<1>(1, 1));
    STATIC_CHECK(FixedPoint<2>(2, 3) / FixedPoint<2>(5, 9) == FixedPoint<2>(0, 39));
}

TEMPLATE_TEST_CASE("FixedPoint::operator/=(FixedPoint)", "", std::int8_t, std::int16_t, std::int32_t, std::int64_t)
{
    FixedPoint<1> fixed(7.9);
    fixed /= FixedPoint<1>(11.3);
    CHECK(fixed == FixedPoint<1>(0, 6));
}

TEMPLATE_TEST_CASE(
    "FixedPoint::operator==(const FixedPoint&)", "", std::int8_t, std::int16_t, std::int32_t, std::int64_t)
{
    STATIC_CHECK(FixedPoint<1, TestType>() == FixedPoint<1, TestType>());
    STATIC_CHECK(FixedPoint<2, TestType>() == FixedPoint<2, TestType>());
    STATIC_CHECK(FixedPoint<2, TestType>(1) == FixedPoint<2, TestType>(1));
    STATIC_CHECK(FixedPoint<2, TestType>(1, 2) == FixedPoint<2, TestType>(1, 2));
    STATIC_CHECK(FixedPoint<2, TestType>(-1, 3) == FixedPoint<2, TestType>(-1, 3));

    STATIC_CHECK_FALSE(FixedPoint<1, TestType>() == FixedPoint<1, TestType>(1));
    STATIC_CHECK_FALSE(FixedPoint<1, TestType>(1) == FixedPoint<1, TestType>(1, 1));
    STATIC_CHECK_FALSE(FixedPoint<1, TestType>(-1) == FixedPoint<1, TestType>(1));
}

TEMPLATE_TEST_CASE(
    "FixedPoint::operator!=(const FixedPoint&)", "", std::int8_t, std::int16_t, std::int32_t, std::int64_t)
{
    STATIC_CHECK(FixedPoint<1, TestType>() != FixedPoint<1, TestType>(1));
    STATIC_CHECK(FixedPoint<1, TestType>(1) != FixedPoint<1, TestType>(1, 1));
    STATIC_CHECK(FixedPoint<1, TestType>(-1) != FixedPoint<1, TestType>(1));

    STATIC_CHECK_FALSE(FixedPoint<1, TestType>() != FixedPoint<1, TestType>());
    STATIC_CHECK_FALSE(FixedPoint<2, TestType>() != FixedPoint<2, TestType>());
    STATIC_CHECK_FALSE(FixedPoint<2, TestType>(1) != FixedPoint<2, TestType>(1));
    STATIC_CHECK_FALSE(FixedPoint<2, TestType>(1, 2) != FixedPoint<2, TestType>(1, 2));
    STATIC_CHECK_FALSE(FixedPoint<2, TestType>(-1, 3) != FixedPoint<2, TestType>(-1, 3));
}

TEMPLATE_TEST_CASE(
    "FixedPoint::operator<(const FixedPoint&)", "", std::int8_t, std::int16_t, std::int32_t, std::int64_t)
{
    STATIC_CHECK(FixedPoint<1, TestType>() < FixedPoint<1, TestType>(0, 1));
    STATIC_CHECK(FixedPoint<1, TestType>() < FixedPoint<1, TestType>(1));
    STATIC_CHECK(FixedPoint<1, TestType>() < FixedPoint<1, TestType>(1.5));

    STATIC_CHECK_FALSE(FixedPoint<1, TestType>() < FixedPoint<1, TestType>());
    STATIC_CHECK_FALSE(FixedPoint<1, TestType>(0, 1) < FixedPoint<1, TestType>());
    STATIC_CHECK_FALSE(FixedPoint<1, TestType>(1.2) < FixedPoint<1, TestType>(1, 1));
}

TEMPLATE_TEST_CASE(
    "FixedPoint::operator<=(const FixedPoint&)", "", std::int8_t, std::int16_t, std::int32_t, std::int64_t)
{
    STATIC_CHECK(FixedPoint<1, TestType>() <= FixedPoint<1, TestType>(0, 1));
    STATIC_CHECK(FixedPoint<1, TestType>() <= FixedPoint<1, TestType>(1));
    STATIC_CHECK(FixedPoint<1, TestType>() <= FixedPoint<1, TestType>(1.5));
    STATIC_CHECK(FixedPoint<1, TestType>() <= FixedPoint<1, TestType>());

    STATIC_CHECK_FALSE(FixedPoint<1, TestType>(0, 1) <= FixedPoint<1, TestType>());
    STATIC_CHECK_FALSE(FixedPoint<1, TestType>(1.2) <= FixedPoint<1, TestType>(1, 1));
}

TEMPLATE_TEST_CASE(
    "FixedPoint::operator>(const FixedPoint&)", "", std::int8_t, std::int16_t, std::int32_t, std::int64_t)
{
    STATIC_CHECK(FixedPoint<1, TestType>(0, 1) > FixedPoint<1, TestType>());
    STATIC_CHECK(FixedPoint<1, TestType>(1.2) > FixedPoint<1, TestType>(1, 1));

    STATIC_CHECK_FALSE(FixedPoint<1, TestType>() > FixedPoint<1, TestType>());
    STATIC_CHECK_FALSE(FixedPoint<1, TestType>() > FixedPoint<1, TestType>(0, 1));
    STATIC_CHECK_FALSE(FixedPoint<1, TestType>() > FixedPoint<1, TestType>(1));
    STATIC_CHECK_FALSE(FixedPoint<1, TestType>() > FixedPoint<1, TestType>(1.5));
}

TEMPLATE_TEST_CASE(
    "FixedPoint::operator>=(const FixedPoint&)", "", std::int8_t, std::int16_t, std::int32_t, std::int64_t)
{
    STATIC_CHECK(FixedPoint<1, TestType>(0, 1) >= FixedPoint<1, TestType>());
    STATIC_CHECK(FixedPoint<1, TestType>(1.2) >= FixedPoint<1, TestType>(1, 1));
    STATIC_CHECK(FixedPoint<1, TestType>() >= FixedPoint<1, TestType>());

    STATIC_CHECK_FALSE(FixedPoint<1, TestType>() >= FixedPoint<1, TestType>(0, 1));
    STATIC_CHECK_FALSE(FixedPoint<1, TestType>() >= FixedPoint<1, TestType>(1));
    STATIC_CHECK_FALSE(FixedPoint<1, TestType>() >= FixedPoint<1, TestType>(1.5));
}
