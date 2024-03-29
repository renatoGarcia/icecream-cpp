#include "icecream.hpp"
#include "common.hpp"

#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <variant>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


auto operator<<(std::ostream& os, std::optional<MyClass> const& value) -> std::ostream&
{
    os << "||opt MyClass|| ";
    if (value.has_value())
    {
        os << *value;
    }
    else
    {
        os << "nullopt";
    }
    return os;
}


TEST_CASE("std_optional")
{
    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto s0 = std::optional<int> {10};
        IC(s0);
        REQUIRE(str == "ic| s0: 10\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto s1 = std::optional<int> {};
        IC(s1);
        REQUIRE(str == "ic| s1: nullopt\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto s2 = std::optional<MyClass> {1};
        IC(s2);
        REQUIRE(str == "ic| s2: ||opt MyClass|| <MyClass 1>\n");
    }
}


TEST_CASE("std_variant")
{
    auto str = std::string{};
    icecream::ic.output(str);

    auto v0 = std::variant<int, double, char> {3.14};
    IC(v0);
    REQUIRE(str == "ic| v0: 3.14\n");
}


#if defined(__cpp_lib_filesystem)
#include <filesystem>
TEST_CASE("std_filesystem")
{
    auto str = std::string{};
    icecream::ic.output(str);

    {
        auto v0 = std::filesystem::path{"/one/two/three"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"/one/two/three\"\n");
        str.clear();
    }
}
#endif


TEST_CASE("std_string_view")
{
    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = std::string_view {"str 1"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"str 1\"\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = std::string {"ABCDE"};
        auto v1 = std::string_view {v0.data() + 1, 3};
        IC(v1);
        REQUIRE(str == "ic| v1: \"BCD\"\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = std::wstring_view {L"wstr 1"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"wstr 1\"\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = std::u16string_view {u"u16str \u03B1"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"u16str \xce\xb1\"\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto v0 = std::u32string_view {U"u32str \U0001F427"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"u32str \xF0\x9F\x90\xA7\"\n");
    }
}
