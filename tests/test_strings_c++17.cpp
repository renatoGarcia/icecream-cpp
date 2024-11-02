#include "icecream.hpp"

#include <string>
#include <string_view>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>


TEST_CASE("std_string_view")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::string_view {"str 1"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"str 1\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::string {"ABCDE"};
        auto v1 = std::string_view {v0.data() + 1, 3};
        IC(v1);
        REQUIRE(str == "ic| v1: \"BCD\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::wstring_view {L"wstr 1"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"wstr 1\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::u16string_view {u"u16str \u03B1"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"u16str \xce\xb1\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::u32string_view {U"u32str \U0001F427"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"u32str \xF0\x9F\x90\xA7\"\n");
    }
}


TEST_CASE("transcode functions")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC_CONFIG.unicode_transcoder(
            [](std::u32string_view str) -> std::string
            {
                REQUIRE(str == U"char16_t test \u03B1");
                return "foo";
            }
        );

        char16_t const* v0 = u"char16_t test \u03B1";
        IC(v0);
        REQUIRE(str == "ic| v0: \"foo\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC_CONFIG.wide_string_transcoder(
            [](std::wstring_view str) -> std::string
            {
                REQUIRE(str == L"wide string test");
                return "qux";
            }
        );

        auto v0 = std::wstring_view(L"wide string test");
        IC(v0);
        REQUIRE(str == "ic| v0: \"qux\"\n");
    }
}
