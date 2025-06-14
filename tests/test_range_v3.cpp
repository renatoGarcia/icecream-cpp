#if defined(_MSC_VER)
  #pragma warning(disable: 4571 4868 5045)
#endif

#include <range/v3/view.hpp>
#include <range/v3/version.hpp>
#include <vector>
#include <forward_list>

#include "icecream.hpp"

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>


TEST_CASE("ranges view")
{
  #if RANGE_V3_VERSION <= 500
    namespace rv = ranges::view;
  #else
    namespace rv = ranges::views;
  #endif

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        using Pair = std::pair<double, int>;
        auto arr = std::vector<Pair>{{0.1, 10}, {1.1, 11}};
        auto v0 = arr | rv::transform([](Pair const& i){return i.second;}) | IC_V();
        for (auto i : v0){(void)i;}
      #if defined(ICECREAM_SOURCE_LOCATION) && defined(__clang__)
        auto const result =
            "ic| range_view_31:83[0]: 10\n"
            "ic| range_view_31:83[1]: 11\n";
      #elif defined(ICECREAM_SOURCE_LOCATION)
        auto const result =
            "ic| range_view_31:78[0]: 10\n"
            "ic| range_view_31:78[1]: 11\n";
      #else
        auto const result =
            "ic| range_view_31[0]: 10\n"
            "ic| range_view_31[1]: 11\n";
      #endif
        REQUIRE(str == result);
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        using Pair = std::pair<double, int>;
        auto arr = std::vector<Pair>{{0.1, 10}, {1.1, 11}, {2.1, 12}};
        auto v0 = arr | rv::drop(2) | IC_V([](Pair const& i){return i.second;});
        for (auto i : v0){(void)i;}
      #if defined(ICECREAM_SOURCE_LOCATION) && defined(__clang__)
        auto const result = "ic| range_view_56:79[0]: 12\n";
      #elif defined(ICECREAM_SOURCE_LOCATION)
        auto const result = "ic| range_view_56:39[0]: 12\n";
      #else
        auto const result = "ic| range_view_56[0]: 12\n";
      #endif
        REQUIRE(str == result);
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        using Pair = std::pair<double, int>;
        auto arr = std::vector<Pair>{{0.1, 10}, {1.1, 11}};
        auto v0 = arr | IC_FV(":#x", [](Pair const& i){return i.second;}) | rv::drop(0);
        for (auto i : v0){(void)i;}
      #if defined(ICECREAM_SOURCE_LOCATION) && defined(__clang__)
        auto const result =
            "ic| range_view_75:73[0]: 0xa\n"
            "ic| range_view_75:73[1]: 0xb\n";
      #elif defined(ICECREAM_SOURCE_LOCATION)
        auto const result =
            "ic| range_view_75:25[0]: 0xa\n"
            "ic| range_view_75:25[1]: 0xb\n";
      #else
        auto const result =
            "ic| range_view_75[0]: 0xa\n"
            "ic| range_view_75[1]: 0xb\n";
      #endif
        REQUIRE(str == result);
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<std::pair<double, int>>{{0.1, 10}, {1.1, 11}, {2.1, 12}};
        auto v0 = arr | rv::drop(1) | IC_V("v");
        for (auto i : v0){(void)i;}
        REQUIRE(str == "ic| v[0]: (1.1, 11)\nic| v[1]: (2.1, 12)\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        using Pair = std::pair<double, int>;
        auto arr = std::vector<Pair>{{0.1, 10}, {1.1, 11}, {2.1, 12}};
        auto v0 = arr | rv::drop(1) | IC_V("v1", [](Pair const& i){return i.first;}) | rv::drop(1) | IC_V("v2");
        for (auto i : v0){(void)i;}
        REQUIRE(str == "ic| v1[0]: 2.1\nic| v2[0]: (2.1, 12)\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<std::pair<double, int>>{{0.1, 10}, {1.1, 11}, {2.1, 12}};
        auto v0 = arr | IC_V("v") | rv::drop(1);
        for (auto i : v0){(void)i;}
        REQUIRE(str == "ic| v[0]: (1.1, 11)\nic| v[1]: (2.1, 12)\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<std::pair<double, int>>{{0.1, 10}, {1.1, 11}, {2.1, 12}};
        auto v0 = IC_V("v") | rv::drop(1);
        for (auto i : arr | v0){(void)i;}
        REQUIRE(str == "ic| v[0]: (1.1, 11)\nic| v[1]: (2.1, 12)\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<std::pair<double, int>>{{0.1, 10}, {1.1, 11}, {2.1, 12}};
        auto v0 = rv::drop(1) | IC_V("v");
        for (auto i : arr | v0){(void)i;}
        REQUIRE(str == "ic| v[0]: (1.1, 11)\nic| v[1]: (2.1, 12)\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<std::pair<double, int>>{{0.1, 10}, {1.1, 11}, {2.1, 12}};
        auto v0 = arr | IC_FV("[:-1:]", "v1") | rv::drop(0) | IC_V("v2");
        for (auto i : v0){(void)i;}
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

        using Pair = std::pair<double, int>;
        auto arr = std::vector<Pair>{{0.1, 10}, {1.1, 11}, {2.1, 12}, {3.1, 13}, {4.1, 14}};
        auto v0 = arr | IC_FV("[1::2]:#x", "v1", [](Pair const& i){return i.second;}) | rv::drop(0);
        for (auto i : v0){(void)i;}
        auto const result =
            "ic| v1[1]: 0xb\n"
            "ic| v1[3]: 0xd\n";
        REQUIRE(str == result);
    }
}


TEST_CASE("ranges view erros")
{
  #if RANGE_V3_VERSION <= 500
    namespace rv = ranges::view;
  #else
    namespace rv = ranges::views;
  #endif

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<int>{1, 2, 3};
        auto v0 = arr | rv::drop(2) | IC_FV("[]", "v");
        for (auto i : v0){(void)i;}
        REQUIRE(str == "ic| v[0]: \"<invalid slice formatting string>\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<int>{1, 2, 3};
        auto v0 = arr | rv::drop(2) | IC_FV("[::-1]", "v");
        for (auto i : v0){(void)i;}
        REQUIRE(str == "ic| v[0]: \"<slice steps must be strictly positive>\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::forward_list<int>{1, 2, 3};
        auto v0 = arr | rv::drop(2) | IC_FV("[:-1:]", "v");
        for (auto i : v0){(void)i;}
        REQUIRE(str == "ic| v[0]: \"<this range view supports only non-negative slice indexes>\"\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<int>{1, 2, 3};
        auto v0 = rv::drop(2) | IC_FV("[:-1:]", "v");
        for (auto i : arr | v0){(void)i;}
        REQUIRE(str == "ic| v[0]: \"<partial views supports only non-negative slice indexes>\"\n");
    }
}
