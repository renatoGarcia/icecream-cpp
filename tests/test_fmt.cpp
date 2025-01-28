#include <fmt/format.h>
#include <fmt/ranges.h>
#include "icecream.hpp"

#include "common.hpp"

#include <string>
#include <tuple>
#include <vector>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>


struct FmtFormatAs
{
    int i;
};

auto format_as(FmtFormatAs f) -> int
{
    return f.i;
}


TEST_CASE("fmt lib")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = IOStreamFmtFormattable{42};

        IC(v0);
        REQUIRE(str == "ic| v0: <fmt IOStreamFmtFormattable 42>\n");
    }

  #if defined(ICECREAM_STL_FORMAT)
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = IOStreamStlFmtFormattable{7};

        IC(v0);
        REQUIRE(str == "ic| v0: <fmt IOStreamStlFmtFormattable 7>\n");
    }
  #endif

    {
        // A range should be printed by the Icecream-cpp range strategy, instead of {fmt}
        // range printing support.

        IC_CONFIG_SCOPE();
        IC_CONFIG.line_wrap_width(20);
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 20, 30, 40, 50, 60};
        auto const result =
            "ic| \n"
            "    v0: [\n"
            "        10, \n"
            "        20, \n"
            "        30, \n"
            "        40, \n"
            "        50, \n"
            "        60\n"
            "    ]\n";

        IC(v0);
        REQUIRE(str == result);
    }

    {
        // The same range as above, but now is should use the {fmt} range printing
        // support.

        IC_CONFIG_SCOPE();
        IC_CONFIG.line_wrap_width(20);
        IC_CONFIG.force_range_strategy(false);
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 20, 30, 40, 50, 60};
        auto const result =
            "ic| \n"
            "    v0: [10, 20, 30, 40, 50, 60]\n";
        IC(v0);
        REQUIRE(str == result);
    }

    {
        // A tuple should be printed by the Icecream-cpp tuple strategy, instead of {fmt}
        // tuple printing support.

        IC_CONFIG_SCOPE();
        IC_CONFIG.line_wrap_width(20);
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::make_tuple(10, 20, 30, 40, 50, 60);
        auto const result =
            "ic| \n"
            "    v0: (\n"
            "        10, \n"
            "        20, \n"
            "        30, \n"
            "        40, \n"
            "        50, \n"
            "        60\n"
            "    )\n";

        IC(v0);
        REQUIRE(str == result);
    }

    {
        // Same tuple as above, but now it should use the {fmt} tuple printing support.
        IC_CONFIG_SCOPE();
        IC_CONFIG.line_wrap_width(20);
        IC_CONFIG.force_tuple_strategy(false);
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::make_tuple(10, 20, 30, 40, 50, 60);
        auto const result =
            "ic| \n"
            "    v0: (10, 20, 30, 40, 50, 60)\n";
        IC(v0);
        REQUIRE(str == result);
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = FmtFormatAs{5};
        IC(v0);
        REQUIRE(str == "ic| v0: 5\n");
    }
}
