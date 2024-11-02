#include "icecream.hpp"

#include <cstdint>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>


struct Point
{
    double x;
    double y;
};


enum EN : std::uint64_t
{
    UM,
    DOIS,
    TRES
};


TEST_CASE("dump_string")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        int i0 = 7;
        struct
        {
            int* integers[2];
        } v0 = {
            {&i0},
        };

        auto const result = "ic\\| v0: \\{integers: \\[(0x)*[0-9a-fA-F]+, 0\\]\\}\n";

        IC(v0);
        REQUIRE_THAT(str, Catch::Matches(result));
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        struct
        {
            EN en;
            int intege[10];
            Point coords[5];
        } v0 = {
            EN::DOIS,
            {1,2,3,4,5},
            {{1,1}, {2,2}},
        };

        auto const result =
        "ic| \n"
        "    v0: {\n"
        "        en: 1, \n"
        "        intege: [1, 2, 3, 4, 5, 0, 0, 0, 0, 0], \n"
        "        \n"
        "            coords: \n"
        "            [\n"
        "                {x: 1, y: 1}, \n"
        "                {x: 2, y: 2}, \n"
        "                {x: 0, y: 0}, \n"
        "                {x: 0, y: 0}, \n"
        "                {x: 0, y: 0}\n"
        "            ]\n"
        "        \n"
        "    }\n";

        IC(v0);
        REQUIRE(str == result);
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        struct
            : Point
        {
            EN en;
        } v0;

        v0.x = 1;
        v0.y = 2;
        v0.en = EN::DOIS;

        IC(v0);
        REQUIRE(str == "ic| v0: {x: 1, y: 2, en: 1}\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        struct
        {
            Point p;
            int i;
        } v0 = {
            {1, 2},
            7,
        };

        IC(v0);
        REQUIRE(str == "ic| v0: {p: {x: 1, y: 2}, i: 7}\n");
    }

}
