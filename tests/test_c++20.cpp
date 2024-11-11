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


template<>
struct std::formatter<NonStreamable, char>
{
    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<class FmtContext>
    FmtContext::iterator format(NonStreamable v, FmtContext& ctx) const
    {
        return std::format_to(ctx.out(), "fmt {}", v.i);
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
      #if defined(__clang__)
        REQUIRE(str == "ic| range_view_58:76[0]: 10\nic| range_view_58:76[1]: 11\n");
      #else
        REQUIRE(str == "ic| range_view_58:71[0]: 10\nic| range_view_58:71[1]: 11\n");
      #endif
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<std::pair<double, int>>{{0.1, 10}, {1.1, 11}, {2.1, 12}};
        auto v0 = arr | rv::drop(2) | IC_V([](auto i){return i.second;});
        for (auto i : v0){}
      #if defined(__clang__)
        REQUIRE(str == "ic| range_view_73:72[0]: 12\n");
      #else
        REQUIRE(str == "ic| range_view_73:39[0]: 12\n");
      #endif
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<std::pair<double, int>>{{0.1, 10}, {1.1, 11}};
        auto v0 = arr | IC_FV(":#x", [](auto i){return i.second;}) | rv::drop(0);
        for (auto i : v0){}
      #if defined(__clang__)
        auto const result =
            "ic| range_view_88:66[0]: 0xa\n"
            "ic| range_view_88:66[1]: 0xb\n";
      #else
        auto const result =
            "ic| range_view_88:25[0]: 0xa\n"
            "ic| range_view_88:25[1]: 0xb\n";
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


template<>
struct std::formatter<MyClass, char>
{
    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<class FmtContext>
    FmtContext::iterator format(MyClass const& v, FmtContext& ctx) const
    {
        return std::format_to(ctx.out(), "MyClass fmt {}", v.i);
    }
};



TEST_CASE("STL formatting lib")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = NonStreamable{42};

        IC(v0);
        REQUIRE(str == "ic| v0: fmt 42\n");
    }

    // Test the precedence of std::formatter specialization
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = MyClass{1};

        IC(v0);
        REQUIRE(str == "ic| v0: MyClass fmt 1\n");
    }
}
