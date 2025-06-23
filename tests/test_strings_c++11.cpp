#if defined(_MSC_VER)
  #pragma warning(disable: 4571 4868 5045)
#endif

#include "icecream.hpp"

#include <string>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>


TEST_CASE("character")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = char {'a'};
        IC(v0);
        REQUIRE(str == "ic| v0: 'a'\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = char{'a'};
        IC_F("x", v0);
        REQUIRE(str == "ic| v0: 61\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = wchar_t {L'a'};
        IC(v0);
        REQUIRE(str == "ic| v0: 'a'\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = char16_t {u'a'};
        IC(v0);
        REQUIRE(str == "ic| v0: 'a'\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = char16_t {u'\u03B1'}; // Greek Small Letter Alpha
        IC(v0);
        REQUIRE(str == "ic| v0: '\xce\xb1'\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = char32_t {U'a'};
        IC(v0);
        REQUIRE(str == "ic| v0: 'a'\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = char32_t {U'\U0001F427'}; // Penguin
        IC(v0);
        REQUIRE(str == "ic| v0: '\xF0\x9F\x90\xA7'\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = char16_t {u'\0'};
        IC(v0);
        REQUIRE(str == "ic| v0: '\\u{0}'\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = char32_t {u'\t'};
        IC(v0);
        REQUIRE(str == "ic| v0: '\\t'\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = char{'\t'};
        IC_F("c", v0);
        REQUIRE(str == "ic| v0: \t\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = 126;
        IC_F("c", v0);
        REQUIRE(str == "ic| v0: ~\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = char16_t{u'a'};
        IC_F("c", v0);
        REQUIRE(str == "ic| v0: a\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = char16_t{0xD801};  // invalid UTF-16 code unit
        IC(v0);
        REQUIRE(str == "ic| v0: '\\x{d801}'\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = char16_t{0xD801};  // invalid UTF-16 code unit
        IC_F("c", v0);
        REQUIRE(str == "ic| v0: �\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = char32_t{0xA001F427};  // invalid unicode codepoint
        IC(v0);
        REQUIRE(str == "ic| v0: '\\x{a001f427}'\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = char32_t{0xA001F427};  // invalid unicode codepoint
        IC_F("c", v0);
        REQUIRE(str == "ic| v0: �\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = char32_t {U'\U0001F427'};
        IC_F("d", v0);
        REQUIRE(str == "ic| v0: 128039\n");
    }
}


TEST_CASE("std_string")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::string {"str 1"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"str 1\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::wstring {L"wstr 1"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"wstr 1\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::u16string {u"u16str \u03B1"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"u16str \xce\xb1\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::u16string {u"u16str \U0001D11E"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"u16str \xf0\x9d\x84\x9e\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::u32string {U"u32str \U0001F427"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"u32str \xF0\x9F\x90\xA7\"\n");
    }

    {  // Test an invalid unicode string
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::u32string {U"abc"};
        v0[1] = 0x300000;
        IC_F("s", v0);
        REQUIRE(str == "ic| v0: a\xEF\xBF\xBD" "c\n");
    }

    {  // Test an invalid unicode string
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::u32string {U"abc"};
        v0[1] = 0x300000;
        IC(v0);
        REQUIRE(str == "ic| v0: \"a\\x{300000}c\"\n");
    }
}


TEST_CASE("c_string")
{
    IC_CONFIG.show_c_string(true);

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        char const* v0 = "Icecream test";
        IC(v0);
        REQUIRE(str == "ic| v0: \"Icecream test\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        char const* const v0 = "Icecream test";
        IC(v0);
        REQUIRE(str == "ic| v0: \"Icecream test\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        char v0[] = "string 2";
        char* const v1 = v0;
        IC(v1);
        REQUIRE(str == "ic| v1: \"string 2\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        char const* v0 = "Icecream test";
        char const*& v1 = v0;
        IC(v1);
        REQUIRE(str == "ic| v1: \"Icecream test\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC_CONFIG.show_c_string(false);
        char const* const v0 = "string 5";
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        wchar_t const* v0 = L"wchar_t test";
        IC(v0);
        REQUIRE(str == "ic| v0: \"wchar_t test\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        char16_t const* v0 = u"char16_t test \u03B1";
        IC(v0);
        REQUIRE(str == "ic| v0: \"char16_t test \xce\xb1\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC_CONFIG.show_c_string(false);
        char16_t const* v0 = u"char16_t test";
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        char32_t const* const v0 = U"char32_t test \U0001F427";
        IC(v0);
        REQUIRE(str == "ic| v0: \"char32_t test \xF0\x9F\x90\xA7\"\n");
    }
}


TEST_CASE("transcode functions")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC_CONFIG.unicode_transcoder(
            [](char32_t const* strg, std::size_t count) -> std::string
            {
                REQUIRE(count == 15);
                REQUIRE(std::char_traits<char32_t>::compare(strg, U"char16_t test \u03B1", 15) == 0);
                return "foo";
            }
        );

        char16_t const* v0 = u"char16_t test \u03B1";
        IC_F("s", v0);
        REQUIRE(str == "ic| v0: foo\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC_CONFIG.wide_string_transcoder(
            [](wchar_t const* strg, std::size_t count) -> std::string
            {
                REQUIRE(count == 16);
                REQUIRE(std::char_traits<wchar_t>::compare(strg, L"wide string test", 16) == 0);
                return "qux";
            }
        );

        wchar_t const* v0 = L"wide string test";
        IC_F("s", v0);
        REQUIRE(str == "ic| v0: qux\n");
    }
}
