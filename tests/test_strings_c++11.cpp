#include "icecream.hpp"

#include <string>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


TEST_CASE("character")
{
    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = char {'a'};
        IC(v0);
        REQUIRE(str == "ic| v0: 'a'\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = wchar_t {L'a'};
        IC(v0);
        REQUIRE(str == "ic| v0: 'a'\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = char16_t {u'a'};
        IC(v0);
        REQUIRE(str == "ic| v0: 'a'\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = char16_t {u'\u03B1'}; // Greek Small Letter Alpha
        IC(v0);
        REQUIRE(str == "ic| v0: '\xce\xb1'\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = char32_t {U'a'};
        IC(v0);
        REQUIRE(str == "ic| v0: 'a'\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = char32_t {U'\U0001F427'}; // Penguin
        IC(v0);
        REQUIRE(str == "ic| v0: '\xF0\x9F\x90\xA7'\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = char16_t {u'\0'};
        IC(v0);
        REQUIRE(str == "ic| v0: '\\0'\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = char32_t {u'\t'};
        IC(v0);
        REQUIRE(str == "ic| v0: '\\t'\n");
    }
}


TEST_CASE("std_string")
{
    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = std::string {"str 1"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"str 1\"\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = std::wstring {L"wstr 1"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"wstr 1\"\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = std::u16string {u"u16str \u03B1"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"u16str \xce\xb1\"\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = std::u16string {u"u16str \U0001D11E"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"u16str \xf0\x9d\x84\x9e\"\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = std::u32string {U"u32str \U0001F427"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"u32str \xF0\x9F\x90\xA7\"\n");
    }

    {  // Test an invalid unicode string
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = std::u32string {U"abc"};
        v0[1] = 0x300000;
        IC(v0);
        REQUIRE(str == "ic| v0: \"a\xEF\xBF\xBD" "c\"\n");
    }
}


TEST_CASE("c_string")
{
    icecream::ic.show_c_string(true);

    {
        auto str = std::string{};
        icecream::ic.output(str);

        char const* v0 = "Icecream test";
        IC(v0);
        REQUIRE(str == "ic| v0: \"Icecream test\"\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        char const* const v0 = "Icecream test";
        IC(v0);
        REQUIRE(str == "ic| v0: \"Icecream test\"\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        char v0[] = "string 2";
        char* const v1 = v0;
        IC(v1);
        REQUIRE(str == "ic| v1: \"string 2\"\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        char const* v0 = "Icecream test";
        char const*& v1 = v0;
        IC(v1);
        REQUIRE(str == "ic| v1: \"Icecream test\"\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        char const* const v0 = "string 5";
        icecream::ic.show_c_string(false);
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
        icecream::ic.show_c_string(true);
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        wchar_t const* v0 = L"wchar_t test";
        IC(v0);
        REQUIRE(str == "ic| v0: \"wchar_t test\"\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        char16_t const* v0 = u"char16_t test \u03B1";
        IC(v0);
        REQUIRE(str == "ic| v0: \"char16_t test \xce\xb1\"\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        char16_t const* v0 = u"char16_t test";
        icecream::ic.show_c_string(false);
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
        icecream::ic.show_c_string(true);
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        char32_t const* const v0 = U"char32_t test \U0001F427";
        IC(v0);
        REQUIRE(str == "ic| v0: \"char32_t test \xF0\x9F\x90\xA7\"\n");
    }
}


TEST_CASE("transcode functions")
{
    {
        auto str = std::string{};
        icecream::ic.output(str);

        icecream::ic.unicode_transcoder(
            [](char32_t const* str, std::size_t count) -> std::string
            {
                REQUIRE(count == 15);
                REQUIRE(std::char_traits<char32_t>::compare(str, U"char16_t test \u03B1", 15) == 0);
                return "foo";
            }
        );

        char16_t const* v0 = u"char16_t test \u03B1";
        IC(v0);
        REQUIRE(str == "ic| v0: \"foo\"\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        icecream::ic.wide_string_transcoder(
            [](wchar_t const* str, std::size_t count) -> std::string
            {
                REQUIRE(count == 16);
                REQUIRE(std::char_traits<wchar_t>::compare(str, L"wide string test", 16) == 0);
                return "qux";
            }
        );

        wchar_t const* v0 = L"wide string test";
        IC(v0);
        REQUIRE(str == "ic| v0: \"qux\"\n");
    }
}
