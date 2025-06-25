#define ICECREAM_DISABLE
#include "icecream.hpp"
#include "common.hpp"

#if defined(_MSC_VER)
  #pragma warning(disable: 4365 4435 4626 4820 4571 4710 4711 4868 5045 5262)
#endif

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>


TEST_CASE("printing")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);
      #if (__cplusplus >= 202002L)
        REQUIRE(std::is_same<decltype(IC()), void>::value);
      #else
        REQUIRE(std::is_same<decltype(IC0()), void>::value);
      #endif
        REQUIRE(str.empty());
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);
        auto r = IC(10);
        REQUIRE(str.empty());
        REQUIRE(r == 10);
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);
        REQUIRE(std::is_same<decltype(IC(1, 2)), void>::value);
        REQUIRE(str.empty());
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = int{1};
        auto r = IC_F("#x", v0);
        REQUIRE(str.empty());
        REQUIRE(r == 1);
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = int{1};
        REQUIRE(std::is_same<decltype(IC_F("#x", v0, 2)), void>::value);
        REQUIRE(str.empty());
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);
        auto mc = MyClass{50};

      #if (__cplusplus >= 202002L)
        auto r = IC_A(mc.ret_val);
      #else
        auto r = IC_A0(mc.ret_val);
      #endif
        REQUIRE(str.empty());
        REQUIRE(r == 50);
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);
        auto mc = IC_A(id_f, MyClass{42});
        REQUIRE(str.empty());
        REQUIRE(mc.ret_val() == 42);
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);
        auto r = IC_FA("#x", sum, 10, 20);
        REQUIRE(str == "");
        REQUIRE(r == 30);
    }
}


#if defined(ICECREAM_LIB_RANGES)
TEST_CASE("views")
{
    namespace rv = std::views;

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto arr = std::vector<std::pair<double, int>>{{0.1, 10}, {1.1, 11}};
        auto v0 = arr | rv::transform([](auto i){return i.second;}) | IC_V();
        for (auto i : v0){(void)i;}
        REQUIRE(str.empty());
    }

}
#endif
