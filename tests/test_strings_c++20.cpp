#include "icecream.hpp"

#include <string>
#include <string_view>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>


TEST_CASE("char8_t")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = char8_t {u8'a'};
        IC(v0);
        REQUIRE(str == "ic| v0: 'a'\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = char8_t {u8'a'};
        IC_F("#o", v0);
        REQUIRE(str == "ic| v0: 0141\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = char8_t{0x80};  // invalid UTF-8 code unit
        IC(v0);
        REQUIRE(str == "ic| v0: '\\x{80}'\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = char8_t{0x80};  // invalid UTF-8 code unit
        IC_F("c", v0);
        REQUIRE(str == "ic| v0: �\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::u8string {u8"u8str \uAB8C"}; // Cherokee Small Letter MO
        IC(v0);
        REQUIRE(str == "ic| v0: \"u8str \xEA\xAE\x8C\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        char8_t const* const v0 = u8"char8_t test \uAB8C";
        IC(v0);
        REQUIRE(str == "ic| v0: \"char8_t test \xEA\xAE\x8C\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::u8string_view {u8"u8str \uAB8C"}; // Cherokee Small Letter MO
        IC(v0);
        REQUIRE(str == "ic| v0: \"u8str \xEA\xAE\x8C\"\n");
    }
}


TEST_CASE("transcode functions")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC_CONFIG.unicode_transcoder(
            [](char32_t const* str, std::size_t count) -> std::string
            {
                REQUIRE(count == 14);
                REQUIRE(std::char_traits<char32_t>::compare(str, U"char8_t test A", count) == 0);
                return "foo";
            }
        );

        char8_t const* v0 = u8"char8_t test A";
        IC(v0);
        REQUIRE(str == "ic| v0: \"foo\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC_CONFIG.unicode_transcoder(
            [](std::u32string_view str) -> std::string
            {
                REQUIRE(str == U"string B");
                return "foo";
            }
        );

        auto v0 = std::u8string_view{u8"string B"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"foo\"\n");
    }
}
