#include "si.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Construction", "[Operations]") {
    
    auto length = si::Meter{ 500.0f };

    REQUIRE(length.value == 500.0f);
}


TEST_CASE("Addition", "[Operations]") {

    auto length = si::Meter{ 500.0f };

    REQUIRE(length.value == 500.0f);
}