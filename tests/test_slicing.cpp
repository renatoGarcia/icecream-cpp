#include "icecream.hpp"

#include <forward_list>

#if defined(_MSC_VER)
  #pragma warning(disable: 4571 4868)
#endif

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

class SizedForwardList
    : public std::forward_list<int>
{
public:
    using std::forward_list<int>::forward_list;

    auto size() const -> size_t
    {
        auto count = size_t{0};
        for (auto const& i : *this)
        {
            (void)i;
            ++count;
        }
        return count;
    }
};


TEST_CASE("bidirectional positive step")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[0:5:1]", v0);
        REQUIRE(str == "ic| v0: [0:5:1]->[10, 11, 12, 13, 14]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[0:5:]", v0);
        REQUIRE(str == "ic| v0: [0:5:]->[10, 11, 12, 13, 14]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[-40:15:]", v0);
        REQUIRE(str == "ic| v0: [-40:15:]->[10, 11, 12, 13, 14]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[1:-2:]", v0);
        REQUIRE(str == "ic| v0: [1:-2:]->[11, 12]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[2:0:]", v0);
        REQUIRE(str == "ic| v0: [2:0:]->[]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[0:3:2]", v0);
        REQUIRE(str == "ic| v0: [0:3:2]->[10, 12]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[:]", v0);
        REQUIRE(str == "ic| v0: [:]->[10, 11, 12, 13, 14]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[::]", v0);
        REQUIRE(str == "ic| v0: [::]->[10, 11, 12, 13, 14]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[3:]", v0);
        REQUIRE(str == "ic| v0: [3:]->[13, 14]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[:3]", v0);
        REQUIRE(str == "ic| v0: [:3]->[10, 11, 12]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[:-1]", v0);
        REQUIRE(str == "ic| v0: [:-1]->[10, 11, 12, 13]\n");
    }
}


TEST_CASE("bidirectional negative step")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[4:0:-1]", v0);
        REQUIRE(str == "ic| v0: [4:0:-1]->[14, 13, 12, 11]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[4:-5:-1]", v0);
        REQUIRE(str == "ic| v0: [4:-5:-1]->[14, 13, 12, 11]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[-1:-6:-1]", v0);
        REQUIRE(str == "ic| v0: [-1:-6:-1]->[14, 13, 12, 11, 10]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[20:-15:-1]", v0);
        REQUIRE(str == "ic| v0: [20:-15:-1]->[14, 13, 12, 11, 10]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[0:2:-1]", v0);
        REQUIRE(str == "ic| v0: [0:2:-1]->[]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[0:-2:-1]", v0);
        REQUIRE(str == "ic| v0: [0:-2:-1]->[]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[-1:0:-2]", v0);
        REQUIRE(str == "ic| v0: [-1:0:-2]->[14, 12]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[::-1]", v0);
        REQUIRE(str == "ic| v0: [::-1]->[14, 13, 12, 11, 10]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[2::-1]", v0);
        REQUIRE(str == "ic| v0: [2::-1]->[12, 11, 10]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[:-3:-1]", v0);
        REQUIRE(str == "ic| v0: [:-3:-1]->[14, 13]\n");
    }
}


TEST_CASE("bidirectional - wrong formatting")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[a:5:1]", v0);
        REQUIRE(str == "ic| v0: <invalid range slicing>\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[]", v0);
        REQUIRE(str == "ic| v0: <invalid range slicing>\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[:::]", v0);
        REQUIRE(str == "ic| v0: <invalid range slicing>\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10, 11, 12, 13, 14};
        IC_F("[::0]", v0);
        REQUIRE(str == "ic| v0: <slice step cannot be zero>\n");
    }
}


TEST_CASE("forward unknown size")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::forward_list<int>{10, 11, 12, 13, 14};
        IC_F("[0:5:1]", v0);
        REQUIRE(str == "ic| v0: [0:5:1]->[10, 11, 12, 13, 14]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::forward_list<int>{10, 11, 12, 13, 14};
        IC_F("[0:3:1]", v0);
        REQUIRE(str == "ic| v0: [0:3:1]->[10, 11, 12]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::forward_list<int>{10, 11, 12, 13, 14};
        IC_F("[:2]", v0);
        REQUIRE(str == "ic| v0: [:2]->[10, 11]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::forward_list<int>{10, 11, 12, 13, 14};
        IC_F("[:]", v0);
        REQUIRE(str == "ic| v0: [:]->[10, 11, 12, 13, 14]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::forward_list<int>{10, 11, 12, 13, 14};
        IC_F("[::2]", v0);
        REQUIRE(str == "ic| v0: [::2]->[10, 12, 14]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::forward_list<int>{10, 11, 12, 13, 14};
        IC_F("[0:15:1]", v0);
        REQUIRE(str == "ic| v0: [0:15:1]->[10, 11, 12, 13, 14]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::forward_list<int>{10, 11, 12, 13, 14};
        IC_F("[3]", v0);
        REQUIRE(str == "ic| v0: [3]->[13]\n");
    }
}


TEST_CASE("forward unknown size - wrong formatting")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::forward_list<int>{10, 11, 12, 13, 14};
        IC_F("[-1]", v0);
        REQUIRE(str == "ic| \n    v0: <this range supports only non-negative start and stop slice indexes>\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::forward_list<int>{10, 11, 12, 13, 14};
        IC_F("[::-1]", v0);
        REQUIRE(str == "ic| v0: <this range supports only strictly positive slice steps>\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::forward_list<int>{10, 11, 12, 13, 14};
        IC_F("[::0]", v0);
        REQUIRE(str == "ic| v0: <slice step cannot be zero>\n");
    }
}


TEST_CASE("forward known size")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = SizedForwardList{10, 11, 12, 13, 14};
        IC_F("[0:5:1]", v0);
        REQUIRE(str == "ic| v0: [0:5:1]->[10, 11, 12, 13, 14]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = SizedForwardList{10, 11, 12, 13, 14};
        IC_F("[0:3:1]", v0);
        REQUIRE(str == "ic| v0: [0:3:1]->[10, 11, 12]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = SizedForwardList{10, 11, 12, 13, 14};
        IC_F("[0::3]", v0);
        REQUIRE(str == "ic| v0: [0::3]->[10, 13]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = SizedForwardList{10, 11, 12, 13, 14};
        IC_F("[0:-3]", v0);
        REQUIRE(str == "ic| v0: [0:-3]->[10, 11]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = SizedForwardList{10, 11, 12, 13, 14};
        IC_F("[-3:-1]", v0);
        REQUIRE(str == "ic| v0: [-3:-1]->[12, 13]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = SizedForwardList{10, 11, 12, 13, 14};
        IC_F("[:2]", v0);
        REQUIRE(str == "ic| v0: [:2]->[10, 11]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = SizedForwardList{10, 11, 12, 13, 14};
        IC_F("[:]", v0);
        REQUIRE(str == "ic| v0: [:]->[10, 11, 12, 13, 14]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = SizedForwardList{10, 11, 12, 13, 14};
        IC_F("[::2]", v0);
        REQUIRE(str == "ic| v0: [::2]->[10, 12, 14]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = SizedForwardList{10, 11, 12, 13, 14};
        IC_F("[0:15:1]", v0);
        REQUIRE(str == "ic| v0: [0:15:1]->[10, 11, 12, 13, 14]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = SizedForwardList{10, 11, 12, 13, 14};
        IC_F("[3]", v0);
        REQUIRE(str == "ic| v0: [3]->[13]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = SizedForwardList{10, 11, 12, 13, 14};
        IC_F("[-3]", v0);
        REQUIRE(str == "ic| v0: [-3]->[12]\n");
    }
}


TEST_CASE("forward known size - wrong formatting")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = SizedForwardList{10, 11, 12, 13, 14};
        IC_F("[::-1]", v0);
        REQUIRE(str == "ic| v0: <this range supports only strictly positive slice steps>\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = SizedForwardList{10, 11, 12, 13, 14};
        IC_F("[::0]", v0);
        REQUIRE(str == "ic| v0: <slice step cannot be zero>\n");
    }
}
