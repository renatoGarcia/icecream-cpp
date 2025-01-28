#include "icecream.hpp"

#include "common.hpp"

#include <ranges>
#include <vector>
#include <forward_list>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>


template <typename T>
struct Sentinel
{
    T it;

    friend bool operator==(Sentinel const& lho, T const& rho)
    {
        return lho.it == rho;
    }

    friend bool operator==(T const& lho, Sentinel const& rho)
    {
        return lho == rho.it;
    }
};


TEST_CASE("ranges view")
{
    namespace rv = std::views;

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<std::pair<double, int>>{{0.1, 10}, {1.1, 11}};
        auto v0 = arr | rv::transform([](auto i){return i.second;}) | IC_V();
        for (auto i : v0){}
      #if defined(ICECREAM_SOURCE_LOCATION) && defined(__clang__)
        REQUIRE(str == "ic| range_view_40:76[0]: 10\nic| range_view_40:76[1]: 11\n");
      #elif defined(ICECREAM_SOURCE_LOCATION)
        REQUIRE(str == "ic| range_view_40:71[0]: 10\nic| range_view_40:71[1]: 11\n");
      #else
        REQUIRE(str == "ic| range_view_40[0]: 10\nic| range_view_40[1]: 11\n");
      #endif
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<std::pair<double, int>>{{0.1, 10}, {1.1, 11}, {2.1, 12}};
        auto v0 = arr | rv::drop(2) | IC_V([](auto i){return i.second;});
        for (auto i : v0){}
      #if defined(ICECREAM_SOURCE_LOCATION) && defined(__clang__)
        REQUIRE(str == "ic| range_view_57:72[0]: 12\n");
      #elif defined(ICECREAM_SOURCE_LOCATION)
        REQUIRE(str == "ic| range_view_57:39[0]: 12\n");
      #else
        REQUIRE(str == "ic| range_view_57[0]: 12\n");
      #endif
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<std::pair<double, int>>{{0.1, 10}, {1.1, 11}};
        auto v0 = arr | IC_FV(":#x", [](auto i){return i.second;}) | rv::drop(0); //68
        for (auto i : v0){}
      #if defined(ICECREAM_SOURCE_LOCATION) && defined(__clang__)
        auto const result =
            "ic| range_view_74:66[0]: 0xa\n"
            "ic| range_view_74:66[1]: 0xb\n";
      #elif defined(ICECREAM_SOURCE_LOCATION)
        auto const result =
            "ic| range_view_74:25[0]: 0xa\n"
            "ic| range_view_74:25[1]: 0xb\n";
      #else
        auto const result =
            "ic| range_view_74[0]: 0xa\n"
            "ic| range_view_74[1]: 0xb\n";
      #endif
        REQUIRE(str == result);
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<std::pair<double, int>>{{0.1, 10}, {1.1, 11}, {2.1, 12}};
        auto v0 = arr | rv::drop(1) | IC_V("v");
        for (auto i : v0){}
        REQUIRE(str == "ic| v[0]: (1.1, 11)\nic| v[1]: (2.1, 12)\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<std::pair<double, int>>{{0.1, 10}, {1.1, 11}, {2.1, 12}};
        auto v0 = arr | rv::drop(1) | IC_V("v1", [](auto i){return i.first;}) | rv::drop(1) | IC_V("v2");
        for (auto i : v0){}
        REQUIRE(str == "ic| v1[0]: 2.1\nic| v2[0]: (2.1, 12)\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<std::pair<double, int>>{{0.1, 10}, {1.1, 11}, {2.1, 12}};
        auto v0 = arr | IC_V("v") | rv::drop(1);
        for (auto i : v0){}
        REQUIRE(str == "ic| v[0]: (1.1, 11)\nic| v[1]: (2.1, 12)\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<std::pair<double, int>>{{0.1, 10}, {1.1, 11}, {2.1, 12}};
        auto v0 = IC_V("v") | rv::drop(1);
        for (auto i : arr | v0){}
        REQUIRE(str == "ic| v[0]: (1.1, 11)\nic| v[1]: (2.1, 12)\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<std::pair<double, int>>{{0.1, 10}, {1.1, 11}, {2.1, 12}};
        auto v0 = rv::drop(1) | IC_V("v");
        for (auto i : arr | v0){}
        REQUIRE(str == "ic| v[0]: (1.1, 11)\nic| v[1]: (2.1, 12)\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<std::pair<double, int>>{{0.1, 10}, {1.1, 11}, {2.1, 12}};
        auto v0 = arr | IC_FV("[:-1:]", "v1") | rv::drop(0) | IC_V("v2");
        for (auto i : v0){}
        auto const result =
            "ic| v1[0]: (0.1, 10)\n"
            "ic| v2[0]: (0.1, 10)\n"
            "ic| v1[1]: (1.1, 11)\n"
            "ic| v2[1]: (1.1, 11)\n"
            "ic| v2[2]: (2.1, 12)\n";
        REQUIRE(str == result);
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<std::pair<double, int>>{{0.1, 10}, {1.1, 11}, {2.1, 12}, {3.1, 13}, {4.1, 14}};
        auto v0 = arr | IC_FV("[1::2]:#x", "v1", [](auto i){return i.second;}) | rv::drop(0);
        for (auto i : v0){}
        auto const result =
            "ic| v1[1]: 0xb\n"
            "ic| v1[3]: 0xd\n";
        REQUIRE(str == result);
    }
}


TEST_CASE("ranges view erros")
{
    namespace rv = std::views;

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<int>{1, 2, 3};
        auto v0 = arr | rv::drop(2) | IC_FV("[]", "v");
        for (auto i : v0){}
        REQUIRE(str == "ic| v[0]: \"<invalid slice formatting string>\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<int>{1, 2, 3};
        auto v0 = arr | rv::drop(2) | IC_FV("[::-1]", "v");
        for (auto i : v0){}
        REQUIRE(str == "ic| v[0]: \"<slice steps must be strictly positive>\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::forward_list<int>{1, 2, 3};
        auto v0 = arr | rv::drop(2) | IC_FV("[:-1:]", "v");
        for (auto i : v0){}
        REQUIRE(str == "ic| v[0]: \"<this range view supports only non-negative slice indexes>\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<int>{1, 2, 3};
        auto v0 = rv::drop(2) | IC_FV("[:-1:]", "v");
        for (auto i : arr | v0){}
        REQUIRE(str == "ic| v[0]: \"<partial views supports only non-negative slice indexes>\"\n");
    }
}


TEST_CASE("ranges")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<int>{10, 11, 12, 13, 14, 15};
        auto v0 = std::ranges::subrange(arr.begin()+2, arr.end());

        IC_F("[0:3]", v0);
        REQUIRE(str == "ic| v0: [0:3]->[12, 13, 14]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<int>{10, 11, 12, 13, 14, 15};
        auto v0 = std::ranges::subrange(
            arr.begin(), Sentinel<decltype(arr.end())>{arr.end()}
        );

        IC_F("[0:3]", v0);
        REQUIRE(str == "ic| v0: [0:3]->[10, 11, 12]\n");
    }
}


TEST_CASE("STL formatting lib")
{
    {
        // STL formatting lib printing should take precedence over IOStreams
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = IOStreamStlFormattable{42};

        IC(v0);
        REQUIRE(str == "ic| v0: <stl IOStreamStlFormattable 42>\n");
    }

    {
        // With C++20 and earlier a range should be printed by the Icecream-cpp range
        // strategy because there is no other way. Starting from C++23 and the "formatting
        // ranges" feature, the range strategy will be selected when the
        // `force_range_strategy` config option is `true` (the default value).

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
        // With C++20 and earlier a tuple should be printed by the Icecream-cpp tuple
        // strategy because there is no other way. Starting from C++23 and the "formatting
        // ranges" feature, the tuple strategy will be selected when the
        // `force_tuple_strategy` config option is `true` (the default value).

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
}
