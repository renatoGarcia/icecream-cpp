#include "icecream.hpp"
#include "common.hpp"

#include <boost/exception/all.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/variant2/variant.hpp>

#include <list>
#include <sstream>
#include <type_traits>
#include <utility>
#include <vector>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


// -------------------------------------------------- Utilities

auto test_empty_ic(std::string& str) -> void
{
    IC_CONFIG_SCOPE();
    IC_CONFIG.output(str);
    IC();
}

template <typename T>
auto id_f(T&& t) -> T
{
    return std::forward<T>(t);
}

struct NonPrintable
{};


template <typename T>
struct MyIterable
{
    std::vector<T> v;

    auto begin() const -> decltype(v.begin())
    {
        return v.begin();
    }

    auto end() const -> decltype(v.end())
    {
        return v.end();
    }
};

auto operator<<(std::ostream& os, MyIterable<NonPrintable> const&) -> std::ostream&
{
     os << "B<A>";
     return os;
}


auto return_7() -> int
{
    return 7;
}


auto sum(int i0, int i1) -> int
{
    return i0 + i1;
}


class exception
    : public virtual std::exception
    , public virtual boost::exception
{
public:
    exception()
        : std::exception()
        , boost::exception()
    {}

    virtual char const* what() const noexcept
    {
        return "what info";
    }
};

using StringInfo = boost::error_info<struct StringTag, std::string>;
using IntInfo = boost::error_info<struct IntTag, int>;


namespace std
{
    auto operator<<(std::ostream& os, std::pair<std::string, char> const& value) -> std::ostream&
    {
        os << "{" << value.first << ", " << value.second << "}";
        return os;
    }
}


// -------------------------------------------------- Tests

TEST_CASE("output")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);
        IC(1, 2);
        REQUIRE(str == "ic| 1: 1, 2: 2\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto sstr = std::ostringstream{};
        IC_CONFIG.output(sstr);
        IC(1, 2);
        REQUIRE(sstr.str() == "ic| 1: 1, 2: 2\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(std::inserter(str, str.end()));
        IC(1, 2);
        REQUIRE(str == "ic| 1: 1, 2: 2\n");
    }
}


TEST_CASE("apply")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);
        auto mc = MyClass{50};

        auto r = IC_A(mc.ret_val);
        REQUIRE(str == "ic| \n");
        REQUIRE(r == 50);
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);
        IC_A(id_f, MyClass{42});
        REQUIRE(str == "ic| MyClass{42}: <MyClass 42>\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);
        IC_A(sum, 1, 2);
        REQUIRE(str == "ic| 1: 1, 2: 2\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);
        IC_FA("#x", sum, 10, 20);
        REQUIRE(str == "ic| 10: 0xa, 20: 0x14\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto mc = MyClass{3};
        auto r = IC_A(mc.multiply, 10);
        REQUIRE(str == "ic| 10: 10\n");
        REQUIRE(r == 30);
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto mc = MyClass{3};
        auto r = IC_A(mc.add, MyClass{7});
        REQUIRE(str == "ic| MyClass{7}: <MyClass 7>\n");
        REQUIRE(r == 10);
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = int{5};
        #define V1 10
        auto r = IC_A(sum, v0, V1);
        REQUIRE(str == "ic| v0: 5, V1: 10\n");
        REQUIRE(r == 15);
        #undef V1
    }
}


TEST_CASE("base")
{
    auto i0 = int {7};
    auto d0 = double {3.14};
    auto v0 = std::vector<int> {1, 2, 3, 4, 5};
    MyClass mc {20};

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC_CONFIG.disable();
        IC(i0);
        REQUIRE(str == "");
    }

    {
        auto str = std::string{};

        test_empty_ic(str);
        REQUIRE_THAT(str, Catch::StartsWith("ic| test_c++11.cpp:26 in"));
        REQUIRE_THAT(str, Catch::Contains("test_empty_ic("));
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC(i0);
        REQUIRE(str == "ic| i0: 7\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC(i0, d0);
        REQUIRE(str == "ic| i0: 7, d0: 3.14\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC(i0, d0, 10, 30);
        REQUIRE(str == "ic| i0: 7, d0: 3.14, 10: 10, 30: 30\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC(sum(40, 2), i0   , sum(3, 5));
        REQUIRE(str == "ic| sum(40, 2): 42, i0: 7, sum(3, 5): 8\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC(std::is_same<int, int> :: value);
        REQUIRE(str == "ic| std::is_same<int, int> :: value: 1\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto const result =
            "ic| \n"
            "    7: 7,\n"
            "    \"same<int, \\\"int>\": [\n"
            "        's', \n"
            "        'a', \n"
            "        'm', \n"
            "        'e', \n"
            "        '<', \n"
            "        'i', \n"
            "        'n', \n"
            "        't', \n"
            "        ',', \n"
            "        ' ', \n"
            "        '\"', \n"
            "        'i', \n"
            "        'n', \n"
            "        't', \n"
            "        '>', \n"
            "        '\\0'\n"
            "    ],\n"
            "    34: 34\n";
        IC(7, "same<int, \"int>", 34);
        REQUIRE(str == result);
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC(7, ',', 34);
        REQUIRE(str == "ic| 7: 7, ',': ',', 34: 34\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC(v0);
        REQUIRE(str == "ic| v0: [1, 2, 3, 4, 5]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC(mc);
        REQUIRE(str == "ic| mc: <MyClass 20>\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC("Hi");
        REQUIRE(str == "ic| \"Hi\": ['H', 'i', '\\0']\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC('a');
        REQUIRE(str == "ic| 'a': 'a'\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        #define V0 10
        IC(V0);
        REQUIRE(str == "ic| V0: 10\n");
        #undef V0
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto ms = MutableStream{7};
        IC(ms);
        REQUIRE(str == "ic| ms: <MutableStream 7>\n");
    }
}


TEST_CASE("return")
{
    {
        REQUIRE(std::is_same<decltype(IC(7)), int&&>::value);
        REQUIRE(IC(7) == 7);
    }

    {
        auto irval = [](int&&){return 10;};
        REQUIRE(IC_A(irval, 7) == 10);
    }

    {
        auto v0 = 'r';
        REQUIRE(std::is_same<decltype(IC(v0)), char&>::value);

        auto& v1 = IC(v0);
        REQUIRE(v1 == 'r');
        REQUIRE(&v1 == &v0);
    }

    {
        auto const v0 = double{3.14};

        REQUIRE(std::is_same<decltype(IC_F("A", v0)), double const&>::value);

        auto&& v1 = IC_F("A", v0);
        REQUIRE(v1 == 3.14);
        REQUIRE(&v1 == &v0);
    }

    {
        REQUIRE(std::is_same<decltype(IC_F("#o", 7)), int&&>::value);
        REQUIRE(IC_F("#o", 7) == 7);
    }

    {
        auto const a = int{20};

        REQUIRE(
            std::is_same<
                decltype(IC(7, 3.14, a)),
                std::tuple<int&&, double&&, int const&>
            >::value
        );

        // !v0 has a dangling reference to 7 and 3.14!
        auto v0 = IC(7, 3.14, a);
        REQUIRE(&std::get<2>(std::move(v0)) == &a);
        REQUIRE((IC(7, 3.14, a) == std::make_tuple(7, 3.14, 20)));
    }

    {
        auto a = int{30};

        REQUIRE(
            std::is_same<
                decltype(IC_F("#", 7, a, 3.14)),
                std::tuple<int&&, int&, double&&>
            >::value
        );

        auto v0 = IC_F("#", 7, a, 3.14);
        REQUIRE(&std::get<1>(std::move(v0)) == &a);
        REQUIRE(IC_F("#", 7, a, 3.14) == std::make_tuple(7, 30, 3.14));
    }

    {
        REQUIRE(std::is_same<decltype(IC()), std::tuple<>>::value);
    }
}


TEST_CASE("boost_optional")
{
    IC_CONFIG_SCOPE();
    auto str = std::string{};
    IC_CONFIG.output(str);

    auto b0 = boost::optional<double> {20.0};
    auto b1 = boost::optional<double> {};

    IC(b0);
    // The boost implementation of operator<<(ostream, optional<T>) will print an extra
    // space before the optional content.
    REQUIRE(str == "ic| b0:  20\n");
    str.clear();

    IC(b1);
    REQUIRE(str == "ic| b1: --\n");
    str.clear();
}


TEST_CASE("boost_variant2")
{
    IC_CONFIG_SCOPE();
    auto str = std::string{};
    IC_CONFIG.output(str);

    auto v0 = boost::variant2::variant<int, double, char> {6.28};
    IC(v0);
    REQUIRE(str == "ic| v0: 6.28\n");
}


TEST_CASE("tuples")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto s0 = std::pair<int, char> {5, 'a'};
        IC(s0);
        REQUIRE(str == "ic| s0: (5, 'a')\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto s0 = std::make_pair(std::string{"oi"}, 'b');
        IC(s0);
        REQUIRE(str == "ic| s0: {oi, b}\n"); // will use the function above
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto s0 = std::pair<int, double> {10, 3.14};
        IC(s0);
        REQUIRE(str == "ic| s0: (10, 3.14)\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto s0 = std::make_tuple(1, 2.2, 'b', "bla");
        IC(s0);
        REQUIRE(str == "ic| s0: (1, 2.2, 'b', \"bla\")\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto s0 = std::make_tuple(10, 20);
        IC_F("x", s0);
        REQUIRE(str == "ic| s0: (a, 14)\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto s0 = std::make_tuple(10, std::make_tuple(20, 1.1));
        IC(s0);
        REQUIRE(str == "ic| s0: (10, (20, 1.1))\n");
    }
}


TEST_CASE("arrays")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        int v0[] = {1, 2, 3};
        IC(v0);
        REQUIRE(str == "ic| v0: [1, 2, 3]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        char v0[] = {'1', '2', '3'};
        IC(v0);
        REQUIRE(str == "ic| v0: ['1', '2', '3']\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        char v0[] = "abc";
        IC(v0);
        REQUIRE(str == "ic| v0: ['a', 'b', 'c', '\\0']\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        int v0[] = {10, 20, 30};
        IC_F(":#X", v0);
        REQUIRE(str == "ic| v0: [0XA, 0X14, 0X1E]\n");
    }
}


TEST_CASE("range")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::list<int> {10, 20, 30};
        IC(v0);
        REQUIRE(str == "ic| v0: [10, 20, 30]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<float> {1.1, 1.2};
        IC(v0);
        REQUIRE(str == "ic| v0: [1.1, 1.2]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        int v0[] = {1, 2, 3};
        IC(v0);
        REQUIRE(str == "ic| v0: [1, 2, 3]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = MyIterable<NonPrintable> {};
        IC(v0);
        REQUIRE(str == "ic| v0: B<A>\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::list<int> {10, 20, 30};
        IC_F(":0v#5x", v0);
        REQUIRE(str == "ic| v0: [0x00a, 0x014, 0x01e]\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<std::tuple<int, double>> {{1, 1.1}, {2, 1.2}};
        IC(v0);
        REQUIRE(str == "ic| v0: [(1, 1.1), (2, 1.2)]\n");
    }
}


TEST_CASE("pointer_like")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::unique_ptr<int> {new int {10}};
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::make_shared<int>(7);
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = boost::make_shared<int>(33);
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        boost::scoped_ptr<int> v0 {new int {33}};
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        int* v0 = new int {40};
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
        delete v0;
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        float* v0 = nullptr;
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*0+\n"));
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::unique_ptr<double> {};
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*0+\n"));
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::make_shared<int>(7);
        auto v1 = std::weak_ptr<int> {v0};
        IC(v1);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v1: (0x)*[0-9a-fA-F]+\n"));

        str.clear();

        v0.reset();
        IC(v1);
        REQUIRE(str == "ic| v1: expired\n");
    }

}


TEST_CASE("prefix")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC_CONFIG.prefix("pref -> ");
        IC(1, 2);
        REQUIRE(str == "pref -> 1: 1, 2: 2\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC_CONFIG.prefix([](){return "icecream -- ";});
        IC(1, 2);
        REQUIRE(str == "icecream -- 1: 1, 2: 2\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC_CONFIG.prefix("ic ", []{return 5;}, "| ");
        IC(1, 2);
        REQUIRE(str == "ic 5| 1: 1, 2: 2\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC_CONFIG.prefix("id ", return_7, " | ");
        IC(1, 2);
        REQUIRE(str == "id 7 | 1: 1, 2: 2\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC_CONFIG.prefix("ic| ");
        IC(1, 2);
        REQUIRE(str == "ic| 1: 1, 2: 2\n");
    }
}


TEST_CASE("line_wrap")
{
    IC_CONFIG_SCOPE();
    IC_CONFIG.line_wrap_width(20);

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<float> {1.1, 1.2};
        IC(v0);
        REQUIRE(str == "ic| v0: [1.1, 1.2]\n");
    }

    {
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::list<int> {10, 20, 30, 40, 50, 60};
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
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<std::vector<int>> {{1, 2}, {10, 20, 30, 40, 50, 60}, {3, 4}};
        auto const result =
            "ic| \n"
            "    v0: [\n"
            "        [1, 2], \n"
            "        [\n"
            "            10, \n"
            "            20, \n"
            "            30, \n"
            "            40, \n"
            "            50, \n"
            "            60\n"
            "        ], \n"
            "        [3, 4]\n"
            "    ]\n";
        IC(v0);
        REQUIRE(str == result);
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC_CONFIG.prefix("pref -> ");
        auto v0 = std::vector<float> {1.1, 1.2};
        auto v1 = std::vector<int> {11, 12};
        auto const result =
            "pref -> \n"
            "    v0: [\n"
            "        1.1, \n"
            "        1.2\n"
            "    ],\n"
            "    v1: [\n"
            "        11, \n"
            "        12\n"
            "    ]\n";
        IC(v0, v1);
        REQUIRE(str == result);
    }
}


TEST_CASE("exception")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::runtime_error("ble");
        IC(v0);
        REQUIRE(str == "ic| v0: ble\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        try
        {
            BOOST_THROW_EXCEPTION(exception {} << IntInfo {10});
        }
        catch (exception& e)
        {
            e << StringInfo {"bla_string"};
            IC(e);
            REQUIRE_THAT(str,
                Catch::Contains("IntTag")
                && Catch::Contains("] = 10")
                && Catch::Contains("StringTag")
                && Catch::Contains("] = bla_string")
                && Catch::Contains("e: what info")
            );
        }
    }
}


TEST_CASE("formatting")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = int{42};
        IC_F("#o", v0, 7);
        REQUIRE(str == "ic| v0: 052, 7: 07\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = 12.3456789f;
        IC_F("#A", v0);
        REQUIRE(
            ((str == "ic| v0: 0X1.8B0FCEP+3\n") ||
             (str == "ic| v0: 0X1.8B0FCE0000000P+3\n")) // Visualstudio 2019 output
        );
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = int{42};
        IC_F("oA", v0);
        REQUIRE(str == "ic| v0: *Error* on formatting string\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = int{42};
        IC_F("x", v0, IC_("*<",5,"d", v0));
        REQUIRE(str == "ic| v0: 2a, v0: 42***\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = int{77};
        IC(v0, IC_("X", v0));
        REQUIRE(str == "ic| v0: 77, v0: 4D\n");
    }

    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        auto v0 = std::vector<int>{10,12,13};
        IC(IC_(":x", v0));
        REQUIRE(str == "ic| v0: [a, c, d]\n");
    }
}


TEST_CASE("Tree char count")
{
    {
        auto v0 = std::string {"str"};
        auto tree = icecream::detail::make_printing_branch(v0, "", IC_CONFIG);
        REQUIRE(tree.code_point_length() == 5); // 3 chars (str) plus 2 char (quotes)
    }

    {
        auto v0 = std::string {"\xce\xb1"}; // Greek small letter alpha
        auto tree = icecream::detail::make_printing_branch(v0, "", IC_CONFIG);
        REQUIRE(tree.code_point_length() == 3); // 1 chars plus 2 char (quotes)
    }

    {
        auto v0 = std::string {"\xF0\x9F\x90\xA7"}; // Penguin
        auto tree = icecream::detail::make_printing_branch(v0, "", IC_CONFIG);
        REQUIRE(tree.code_point_length() == 3); // 1 chars plus 2 char (quotes)
    }

    {
        auto v0 = std::wstring {L"wstr"};
        auto tree = icecream::detail::make_printing_branch(v0, "", IC_CONFIG);
        REQUIRE(tree.code_point_length() == 6);
    }

    {
        auto v0 = std::u16string {u"u16\u03B1"}; // Greek small letter alpha
        auto tree = icecream::detail::make_printing_branch(v0, "", IC_CONFIG);
        REQUIRE(tree.code_point_length() == 6);
    }

    {
        auto v0 = std::u32string {U"abcd\U0001F427"}; // Penguin
        auto tree = icecream::detail::make_printing_branch(v0, "", IC_CONFIG);
        REQUIRE(tree.code_point_length() == 7);
    }
}

TEST_CASE("output transcoding")
{
    {
        IC_CONFIG_SCOPE();
        auto str = std::string{};
        IC_CONFIG.output(str);

        IC_CONFIG.output_transcoder(
            [](char const* str, std::size_t count) -> std::string
            {
                return std::string(str, count) + "|";
            }
        );

        auto v0 = int{12345};
        IC(v0);
        REQUIRE(str == "ic| v0: 12345|\n|");
    }
}
