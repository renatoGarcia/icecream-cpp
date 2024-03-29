#include "icecream.hpp"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <string>
#include <string_view>


TEST_CASE("char8_t")
{
    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = char8_t {u8'a'};
        IC(v0);
        REQUIRE(str == "ic| v0: 'a'\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = std::u8string {u8"u8str \uAB8C"}; // Cherokee Small Letter MO
        IC(v0);
        REQUIRE(str == "ic| v0: \"u8str \xEA\xAE\x8C\"\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        char8_t const* const v0 = u8"char8_t test \uAB8C";
        IC(v0);
        REQUIRE(str == "ic| v0: \"char8_t test \xEA\xAE\x8C\"\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = std::u8string_view {u8"u8str \uAB8C"}; // Cherokee Small Letter MO
        IC(v0);
        REQUIRE(str == "ic| v0: \"u8str \xEA\xAE\x8C\"\n");
    }
}
