#include "icecream.hpp"

#if defined(_MSC_VER)
  #pragma warning(disable: 4571 4868 5045)
#endif

#if defined(__GNUC__) && !defined(__clang__) && (__GNUC__ <= 6)
    // GCC at version 6 and older has a bug when processing a `_Pragma` directive within
    // macro expansions. https://gcc.gnu.org/bugzilla/show_bug.cgi?id=69126
    #pragma GCC diagnostic ignored "-Wshadow"
#endif

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>


TEST_CASE("Hereditary")
{
    {
        auto v0 = icecream::detail::Hereditary<int>{1};
        REQUIRE(v0.value() == 1);
        v0 = 2;
        REQUIRE(v0.value() == 2);
    }

    {
        auto v0 = icecream::detail::Hereditary<double>{2.2};
        auto v1 = icecream::detail::Hereditary<double>(v0);
        REQUIRE(v0.value() == 2.2);
        REQUIRE(v1.value() == 2.2);

        v0 = 4.0;
        REQUIRE(v0.value() == 4.0);
        REQUIRE(v1.value() == 4.0);

        v1 = 4.2;
        REQUIRE(v0.value() == 4.0);
        REQUIRE(v1.value() == 4.2);
    }

    {
        auto v0 = int{7};
        auto v2 = int{8};

        auto v1 = icecream::detail::Hereditary<int>{v0};
        REQUIRE(v1.value() == v0);

        v1 = v2;
        REQUIRE(v1.value() == v2);
    }
}


TEST_CASE("Configure")
{
    IC_CONFIG_SCOPE();

    {
        REQUIRE(IC_CONFIG.is_enabled());
        IC_CONFIG_SCOPE();
        REQUIRE(IC_CONFIG.is_enabled());
        IC_CONFIG.disable();
        REQUIRE(!IC_CONFIG.is_enabled());
        REQUIRE(::IC_CONFIG.is_enabled());
    }
    REQUIRE(IC_CONFIG.is_enabled());
}
