#include "icecream.hpp"

#include <ranges>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


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
