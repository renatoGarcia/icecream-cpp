#include <vector>
#include <list>
#include <map>
#include <optional>
#include <sstream>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "icecream.hpp"

auto test_empty_ic() -> void
{
    IC();
}


class MyClass
{
public:
    MyClass() = delete;
    MyClass(const MyClass &other) = delete;
    MyClass(MyClass &&other) = delete;
    MyClass& operator=(const MyClass &other) = delete;
    MyClass& operator=(MyClass &&other) = delete;

    explicit MyClass(int i_)
        : i {i_}
    {}

    int i;

    friend auto operator<<(std::ostream& os, MyClass const& self) -> std::ostream&
    {
     os << "<MyClass " << self.i << ">";
     return os;
    }
};


auto sum(int i0, int i1) -> int
{
    return i0 + i1;
}


// -------------------------------------------------- Test base
TEST_CASE("base")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    auto i0 = int {7};
    auto d0 = double {3.14};
    auto v0 = std::vector<int> {1, 2, 3, 4, 5};
    MyClass mc {20};

    test_empty_ic();
    REQUIRE(sstr.str() == "ic| test.cpp:14 in \"void test_empty_ic()\"\n");
    sstr.str("");

    IC(i0);
    REQUIRE(sstr.str() == "ic| i0: 7\n");
    sstr.str("");

    IC(i0, d0);
    REQUIRE(sstr.str() == "ic| i0: 7, d0: 3.14\n");
    sstr.str("");

    IC(i0, d0, 10, 30);
    REQUIRE(sstr.str() == "ic| i0: 7, d0: 3.14, 10: 10, 30: 30\n");
    sstr.str("");

    IC((sum(40, 2)), i0   , (((sum(3, 5)))));
    REQUIRE(sstr.str() == "ic| (sum(40, 2)): 42, i0: 7, (((sum(3, 5)))): 8\n");
    sstr.str("");

    IC(v0);
    REQUIRE(sstr.str() == "ic| v0: [1, 2, 3, 4, 5]\n");
    sstr.str("");

    IC(mc);
    REQUIRE(sstr.str() == "ic| mc: <MyClass 20>\n");
    sstr.str("");
}


// -------------------------------------------------- Test optional

#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>

TEST_CASE("boost_optional")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    auto b0 = boost::optional<double> {20.0};
    auto b1 = boost::optional<double> {};

    IC(b0);
    // The boost implementation of operator<<(ostream, optional<T>) will print an extra
    // space before the optional content.
    REQUIRE(sstr.str() == "ic| b0:  20\n");
    sstr.str("");

    IC(b1);
    REQUIRE(sstr.str() == "ic| b1: --\n");
    sstr.str("");
}


#ifdef CPP_17
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
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    auto s0 = std::optional<int> {10};
    auto s1 = std::optional<int> {};
    auto s2 = std::optional<MyClass> {1};

    IC(s0);
    REQUIRE(sstr.str() == "ic| s0: 10\n");
    sstr.str("");

    IC(s1);
    REQUIRE(sstr.str() == "ic| s1: nullopt\n");
    sstr.str("");

    IC(s2);
    REQUIRE(sstr.str() == "ic| s2: ||opt MyClass|| <MyClass 1>\n");
    sstr.str("");
}
#endif


// -------------------------------------------------- Test pair

#include <utility>
auto operator<<(std::ostream& os, std::pair<std::string, char> const& value) -> std::ostream&
{
    os << "{" << value.first << ", " << value.second << "}";
    return os;
}

TEST_CASE("pair")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    auto s0 = std::pair<int, char> {5, 'a'};
    auto s1 = std::make_pair("oi", 'b');
    auto s2 = std::pair<int, double> {10, 3.14};

    IC(s0);
    REQUIRE(sstr.str() == "ic| s0: [5, a]\n");
    sstr.str("");

    IC(s1);
    REQUIRE(sstr.str() == "ic| s1: {oi, b}\n");
    sstr.str("");

    IC(s2);
    REQUIRE(sstr.str() == "ic| s2: [10, 3.14]\n");
    sstr.str("");
}


// -------------------------------------------------- Test iterable

TEST_CASE("iterable")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    auto v0 = std::list<int> {10, 20, 30};
    auto v1 = std::vector<float> {1.1, 1.2};

    IC(v0);
    REQUIRE(sstr.str() == "ic| v0: [10, 20, 30]\n");
    sstr.str("");

    IC(v1);
    REQUIRE(sstr.str() == "ic| v1: [1.1, 1.2]\n");
    sstr.str("");
}


// -------------------------------------------------- Test pointer like
#include <boost/smart_ptr.hpp>

TEST_CASE("pointer_like")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    auto v0 = std::make_shared<int>(7);
    auto v1 = boost::make_shared<int>(33);
    int* v2 = new int {40};
    float* v3 = nullptr;
    auto v4 = std::unique_ptr<double> {};
    auto v5 = std::weak_ptr<int> {v0};

    IC(v0);
    REQUIRE_THAT(sstr.str(), Catch::Matches("ic\\| v0: 0x[0-9a-f]+\n"));
    sstr.str("");

    IC(v1);
    REQUIRE_THAT(sstr.str(), Catch::Matches("ic\\| v1: 0x[0-9a-f]+\n"));
    sstr.str("");

    IC(v2);
    REQUIRE_THAT(sstr.str(), Catch::Matches("ic\\| v2: 0x[0-9a-f]+\n"));
    sstr.str("");

    IC(v3);
    REQUIRE(sstr.str() == "ic| v3: 0\n");
    sstr.str("");

    IC(v4);
    REQUIRE(sstr.str() == "ic| v4: 0\n");
    sstr.str("");

    IC(v5);
    REQUIRE_THAT(sstr.str(), Catch::Matches("ic\\| v5: valid weak_ptr\n"));
    sstr.str("");

    v0.reset();
    IC(v5);
    REQUIRE_THAT(sstr.str(), Catch::Matches("ic\\| v5: expired weak_ptr\n"));
    sstr.str("");

    delete v2;
}


// -------------------------------------------------- Test prefix
TEST_CASE("prefix")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    icecream::ic.prefix("pref -> ");
    IC(1, 2);
    REQUIRE(sstr.str() == "pref -> 1: 1, 2: 2\n");
    sstr.str("");


    icecream::ic.prefix([](){return "icecream -- ";});
    IC(1, 2);
    REQUIRE(sstr.str() == "icecream -- 1: 1, 2: 2\n");
    sstr.str("");

    icecream::ic.prefix("ic| ");
    IC(1, 2);
    REQUIRE(sstr.str() == "ic| 1: 1, 2: 2\n");
    sstr.str("");
}

// -------------------------------------------------- Test char_p
TEST_CASE("char_p")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    char const* str0 = "Icecream test";
    IC(str0);
    REQUIRE(sstr.str() == "ic| str0: Icecream test\n");
    sstr.str("");

    char const* const str1 = "Icecream test";
    IC(str1);
    REQUIRE(sstr.str() == "ic| str1: Icecream test\n");
    sstr.str("");

    char str2[] = "string 2";
    IC(str2);
    REQUIRE(sstr.str() == "ic| str2: string 2\n");
    sstr.str("");

    char* const str3 = str2;
    IC(str3);
    REQUIRE(sstr.str() == "ic| str3: string 2\n");
    sstr.str("");

    char const*& str4 = str0;
    IC(str4);
    REQUIRE(sstr.str() == "ic| str4: Icecream test\n");
    sstr.str("");

    char const* const str5 = "string 5";
    icecream::ic.show_c_string(false);
    IC(str5);
    REQUIRE_THAT(sstr.str(), Catch::Matches("ic\\| str5: 0x[0-9a-f]+\n"));
    sstr.str("");
    icecream::ic.show_c_string(true);
}

// -------------------------------------------------- Test line wrap

TEST_CASE("line_wrap")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    icecream::ic.lineWrapWidth(15);

    {
        auto v0 = std::vector<float> {1.1, 1.2};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: [1.1, 1.2]\n");
        sstr.str("");
    }

    {
        auto v0 = std::list<int> {10, 20, 30, 40, 50, 60};
        auto const result =
            "ic| v0: [10,\n"
            "         20,\n"
            "         30,\n"
            "         40,\n"
            "         50,\n"
            "         60]\n";
        IC(v0);
        REQUIRE(sstr.str() == result);
        sstr.str("");
    }

    {
        auto v0 = std::vector<int> {10, 20, 30};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: [10, 20, 30]\n");
        sstr.str("");
    }

    icecream::ic.lineWrapWidth(70);
}
