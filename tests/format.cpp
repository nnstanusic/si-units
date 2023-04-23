#include "si.h"
#include <catch2/catch_test_macros.hpp>

#include <format>


TEST_CASE("Format meter", "[Operations]") {    
    auto length = si::meter{ 25.0f }  / si::ampere{3.5};

    auto x = std::format("{}", length);

    REQUIRE(x == "7.142857 m A^-1");
}
