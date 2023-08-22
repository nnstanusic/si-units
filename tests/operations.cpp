#include "si.h"
#include <complex>
#include <catch2/catch_test_macros.hpp>
#include "catch2/catch_template_test_macros.hpp"

#define SI_TYPES si::meter, si::second, si::mols, si::ampere, si::kelvin, si::candela, si::gram, si::hertz, si::newton, si::pascal, si::joule, si::watt, si::coulomb, si::volt, si::farad, si::ohm, si::siemens, si::weber, si::tesla, si::henry, si::lux, si::katal

TEMPLATE_PRODUCT_TEST_CASE("Testing construcion of types", "[Operations]",
    (SI_TYPES),
    (int, float, double, std::complex<char>)) 
{
    auto unit = TestType{ };

    using underlying_type = decltype(unit.value);

    REQUIRE(unit.value == underlying_type{});
    REQUIRE(static_cast<underlying_type>(unit) == underlying_type{});
}


TEST_CASE("Addition", "[Operations]") {

    auto length = si::meter{ 250.0f } + si::meter{ 250.0f };

    REQUIRE(length.value == 500.0f);
}


TEST_CASE("Subtraction", "[Operations]") {    
    auto length = si::meter{ 250.0f } - si::meter{ 250.0f };
    
    REQUIRE(length.value == 0.0f);
}
