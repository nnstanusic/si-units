#include "si_format.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>



using base_units = std::tuple<si::meter<float>, si::second<float>, si::mols<float>, si::ampere<float>, si::kelvin<float>, si::candela<float>, si::gram<float>>;
constexpr auto unit_symbols = std::array<const char*, 7>{"m", "s", "mol", "A", "K", "cd", "g"};

namespace util {

    template <class X, class Tuple>
    class Idx;

    template <class X, class... T>
    class Idx<X, std::tuple<T...>> {
        template <std::size_t... idx>
        static constexpr size_t find_idx(std::index_sequence<idx...>) {
            return -1 + ((std::is_same<X, T>::value ? idx + 1 : 0) + ...);
        }
    public:
        static constexpr size_t value = find_idx(std::index_sequence_for<T...>{});
    };

    template<typename X, class Tuple>
    inline constexpr size_t Idx_v = Idx<X, Tuple>::value;
}

TEMPLATE_LIST_TEST_CASE("Base formats", "[Format]", base_units)
{
    using namespace std::string_literals;
    

    auto unit = TestType{ 25.2f };

    auto x = std::format("{}", unit);

    auto expoected_output = "25.2 "s + unit_symbols[ util::Idx_v<TestType, base_units> ];

    REQUIRE(x == expoected_output);
}


TEST_CASE("Base format test", "[Format]") {
    auto length = si::meter{ 25.0f }  / si::ampere{3.5f};

    auto x = std::format("{}", length);

    REQUIRE(x == "7.142857 m A^-1");
}