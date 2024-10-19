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
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto s0 = std::optional<int> {10};
        IC(s0);
        REQUIRE(str == "ic| s0: 10\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto s1 = std::optional<int> {};
        IC(s1);
        REQUIRE(str == "ic| s1: nullopt\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto s2 = std::optional<MyClass> {1};
        IC(s2);
        REQUIRE(str == "ic| s2: ||opt MyClass|| <MyClass 1>\n");
    }
}


TEST_CASE("std_variant")
{
    IC_CONFIG_SCOPE();
    auto str = std::string{};
    IC_CONFIG.output(str);

    auto v0 = std::variant<int, double, char> {3.14};
    IC(v0);
    REQUIRE(str == "ic| v0: 3.14\n");
}


#if defined(__cpp_lib_filesystem)
#include <filesystem>
TEST_CASE("std_filesystem")
{
    IC_CONFIG_SCOPE();
    auto str = std::string{};
    IC_CONFIG.output(str);

    {
        auto v0 = std::filesystem::path{"/one/two/three"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"/one/two/three\"\n");
        str.clear();
    }
}
#endif


TEST_CASE("output transcoding")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC_CONFIG.output_transcoder(
            [](std::string_view str) -> std::string
            {
                return std::string(str) + ";";
            }
        );

        auto v0 = 3.14f;
        IC(v0);
        REQUIRE(str == "ic| v0: 3.14;\n;");
    }
}
