#include "icecream.hpp"

#if defined(_MSC_VER)
  #pragma warning(disable: 4571 4868)
#endif

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>


TEST_CASE("STL formatting lib")
{
  #if defined(ICECREAM_STL_FORMAT_RANGES)
    // Starting from C++23 and the "formatting ranges" feature, the STL formatting lib has
    // support to printing ranges and tuples. Icecream-cpp will used them instead of the
    // range and tuple strategies when the respective force config option is `false`.

    {
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
  #endif
}
