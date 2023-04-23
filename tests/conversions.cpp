#include "si.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Kilometer to meter", "[Conversions]") {

    auto length = si::kilo(si::meter{ 500.0f });

    REQUIRE(length.value == 0.5f);
}



TEST_CASE("minute to second", "[Conversions]") {
    
    

    constexpr auto ct_seconds = si::second{ 0.0f } + si::minute{ 1.0f };
    static_assert(ct_seconds.value == 60.0f, "Minutes not converting to seconds as expected");
    auto min = si::minute{ 1.0f };
    
    auto seconds = si::second{ 0.0f } + min;
    REQUIRE(seconds.value == 60.0f);
}


TEST_CASE("Generating newtons", "[Conversions]") {
    using namespace si::literals;

    constexpr auto force = m / s / s * si::kilo_gram{ 1.0f };

    using newton = std::remove_cv_t<decltype(force)>;

    constexpr auto check = std::same_as<newton, si::newton>;

    static_assert(check, "Generated type is not equal to newton");

    REQUIRE(check);
}