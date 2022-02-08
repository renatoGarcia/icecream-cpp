#include "icecream.hpp"

#include <vector>
#include <list>
#include <map>
#include <sstream>
#include <cstddef>
#include <limits>

#if defined (CPP_17)
#include <optional>
#include <variant>
#endif

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


auto test_empty_ic() -> void
{
    IC();
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

    {
        icecream::ic.disable();
        IC(i0);
        REQUIRE(sstr.str() == "");
        sstr.str("");
        icecream::ic.enable();
    }


    {
        test_empty_ic();
        REQUIRE_THAT(sstr.str(), Catch::StartsWith("ic| test.cpp:21 in"));
        REQUIRE_THAT(sstr.str(), Catch::Contains("test_empty_ic("));
        sstr.str("");
    }

    {
        IC(i0);
        REQUIRE(sstr.str() == "ic| i0: 7\n");
        sstr.str("");
    }

    {
        IC(i0, d0);
        REQUIRE(sstr.str() == "ic| i0: 7, d0: 3.14\n");
        sstr.str("");
    }

    {
        IC(i0, d0, 10, 30);
        REQUIRE(sstr.str() == "ic| i0: 7, d0: 3.14, 10: 10, 30: 30\n");
        sstr.str("");
    }

    {
        IC((sum(40, 2)), i0   , (((sum(3, 5)))));
        REQUIRE(sstr.str() == "ic| sum(40, 2): 42, i0: 7, sum(3, 5): 8\n");
        sstr.str("");
    }

    {
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: [1, 2, 3, 4, 5]\n");
        sstr.str("");
    }

    {
        IC(mc);
        REQUIRE(sstr.str() == "ic| mc: <MyClass 20>\n");
        sstr.str("");
    }
}


// -------------------------------------------------- Test return
TEST_CASE("return")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    using icecream::_;

    {
        // !v0 is a dangling reference!
        auto&& v0 = IC(7);
        REQUIRE(std::is_same<decltype(v0), int&&>::value);

        REQUIRE(IC(7) == 7);
    }

    {
        auto v0 = 'r';
        auto&& v1 = IC(v0);
        REQUIRE(std::is_same<decltype(v1), char&>::value);
        REQUIRE(v1 == 'r');
        REQUIRE(&v1 == &v0);
    }

    {
        auto v0 = double{3.14};
        auto&& v1 = IC_("A", v0);
        REQUIRE(std::is_same<decltype(v1), double&>::value);
        REQUIRE(v1 == 3.14);
        REQUIRE(&v1 == &v0);
    }

    {
        // !v0 is a dangling reference!
        auto&& v0 = IC_("#o", 7);
        REQUIRE(std::is_same<decltype(v0), int&&>::value);
        REQUIRE(IC_("#o", 7) == 7);
    }

    {
        auto const a = int{20};

        // !v0 has a dangling reference to 7 and 3.14!
        auto&& v0 = IC(7, 3.14, a);
        REQUIRE(std::is_same<decltype(v0), std::tuple<int&&, double&&, int const&>&&>::value);
        REQUIRE(&std::get<2>(std::move(v0)) == &a);
        REQUIRE((IC(7, 3.14, a) == std::make_tuple(7, 3.14, 20)));
    }

    {
        auto a = int{30};

        // !v0 has a dangling reference to 7 and 3.14!
        auto&& v0 = IC_("#", 7, a, 3.14);
        REQUIRE(std::is_same<decltype(v0), std::tuple<int&&, int&, double&&>&&>::value);
        REQUIRE(&std::get<1>(std::move(v0)) == &a);
        REQUIRE(IC_("#", 7, a, 3.14) == std::make_tuple(7, 30, 3.14));
    }

    {
        auto a = int{10};
        auto const b = int{20};

        auto&& v0 = IC(_("0v#4x", a, b), 49);
        REQUIRE(std::is_same<decltype(v0), std::tuple<int&, int const&, int&&>&&>::value);
        REQUIRE(IC(_("0v#4x", a, b), 49) == std::make_tuple(10, 20, 49));
    }

    {
        REQUIRE(std::is_same<decltype(IC()), void>::value);
    }

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


#if defined(CPP_17)
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

// -------------------------------------------------- Test variant

#include <boost/variant2/variant.hpp>

TEST_CASE("boost_variant2")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    {
        auto v0 = boost::variant2::variant<int, double, char> {6.28};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: 6.28\n");
        sstr.str("");
    }

}


#if defined(CPP_17)
TEST_CASE("std_variant")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    {
        auto v0 = std::variant<int, double, char> {3.14};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: 3.14\n");
        sstr.str("");
    }
}
#endif


// -------------------------------------------------- Test tuples

#include <utility>
namespace std
{
auto operator<<(std::ostream& os, std::pair<std::string, char> const& value) -> std::ostream&
{
    os << "{" << value.first << ", " << value.second << "}";
    return os;
}
}

TEST_CASE("tuples")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    {
        auto s0 = std::pair<int, char> {5, 'a'};
        IC(s0);
        REQUIRE(sstr.str() == "ic| s0: (5, 'a')\n");
        sstr.str("");
    }

    {
        auto s0 = std::make_pair(std::string{"oi"}, 'b');
        IC(s0);
        REQUIRE(sstr.str() == "ic| s0: {oi, b}\n"); // will use the function above
        sstr.str("");
    }

    {
        auto s0 = std::pair<int, double> {10, 3.14};
        IC(s0);
        REQUIRE(sstr.str() == "ic| s0: (10, 3.14)\n");
        sstr.str("");
    }

    {
        auto s0 = std::make_tuple(1, 2.2, 'b', "bla");
        IC(s0);
        REQUIRE(sstr.str() == "ic| s0: (1, 2.2, 'b', \"bla\")\n");
        sstr.str("");
    }

    {
        auto s0 = std::make_tuple(10, 20);
        IC_("x", s0);
        REQUIRE(sstr.str() == "ic| s0: (a, 14)\n");
        sstr.str("");
    }
}


// -------------------------------------------------- Test arrays
TEST_CASE("arrays")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    {
        int v0[] = {1, 2, 3};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: [1, 2, 3]\n");
        sstr.str("");
    }

    {
        char v0[] = {'1', '2', '3'};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: ['1', '2', '3']\n");
        sstr.str("");
    }

    {
        char v0[] = "abc";
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: ['a', 'b', 'c', '\\0']\n");
        sstr.str("");
    }

    {
        int v0[] = {10, 20, 30};
        IC_("#X", v0);
        REQUIRE(sstr.str() == "ic| v0: [0XA, 0X14, 0X1E]\n");
        sstr.str("");
    }
}


// -------------------------------------------------- Test iterable

auto operator<<(std::ostream& os, MyIterable<NonPrintable> const&) -> std::ostream&
{
     os << "B<A>";
     return os;
}

TEST_CASE("iterable")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    {
        auto v0 = std::list<int> {10, 20, 30};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: [10, 20, 30]\n");
        sstr.str("");
    }

    {
        auto v0 = std::vector<float> {1.1, 1.2};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: [1.1, 1.2]\n");
        sstr.str("");
    }

    {
        int v0[] = {1, 2, 3};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: [1, 2, 3]\n");
        sstr.str("");
    }

    {
        auto v0 = MyIterable<NonPrintable> {};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: B<A>\n");
        sstr.str("");
    }

    {
        auto v0 = std::list<int> {10, 20, 30};
        IC_("0v#5x", v0);
        REQUIRE(sstr.str() == "ic| v0: [0x00a, 0x014, 0x01e]\n");
        sstr.str("");
    }

}


// -------------------------------------------------- Test pointer like
#include <boost/smart_ptr.hpp>

TEST_CASE("pointer_like")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    {
        auto v0 = std::unique_ptr<int> {new int {10}};
        IC(v0);
        REQUIRE_THAT(sstr.str(), Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
        sstr.str("");
    }

    {
        auto v0 = std::make_shared<int>(7);
        IC(v0);
        REQUIRE_THAT(sstr.str(), Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
        sstr.str("");
    }

    {
        auto v0 = boost::make_shared<int>(33);
        IC(v0);
        REQUIRE_THAT(sstr.str(), Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
        sstr.str("");
    }

    {
        boost::scoped_ptr<int> v0 {new int {33}};
        IC(v0);
        REQUIRE_THAT(sstr.str(), Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
        sstr.str("");
    }

    {
        int* v0 = new int {40};
        IC(v0);
        REQUIRE_THAT(sstr.str(), Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
        sstr.str("");
        delete v0;
    }

    {
        float* v0 = nullptr;
        IC(v0);
        REQUIRE_THAT(sstr.str(), Catch::Matches("ic\\| v0: (0x)*0+\n"));
        sstr.str("");
    }

    {
        auto v0 = std::unique_ptr<double> {};
        IC(v0);
        REQUIRE_THAT(sstr.str(), Catch::Matches("ic\\| v0: (0x)*0+\n"));
        sstr.str("");
    }

    {
        auto v0 = std::make_shared<int>(7);
        auto v1 = std::weak_ptr<int> {v0};
        IC(v1);
        REQUIRE_THAT(sstr.str(), Catch::Matches("ic\\| v1: (0x)*[0-9a-fA-F]+\n"));
        sstr.str("");

        v0.reset();
        IC(v1);
        REQUIRE(sstr.str() == "ic| v1: expired\n");
        sstr.str("");
    }

}


// -------------------------------------------------- Test prefix
TEST_CASE("prefix")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    {
        icecream::ic.prefix("pref -> ");
        IC(1, 2);
        REQUIRE(sstr.str() == "pref -> 1: 1, 2: 2\n");
        sstr.str("");
    }

    {
        icecream::ic.prefix([](){return "icecream -- ";});
        IC(1, 2);
        REQUIRE(sstr.str() == "icecream -- 1: 1, 2: 2\n");
        sstr.str("");
    }

    {
        icecream::ic.prefix("ic ", []{return 5;}, "| ");
        IC(1, 2);
        REQUIRE(sstr.str() == "ic 5| 1: 1, 2: 2\n");
        sstr.str("");
    }

    {
        icecream::ic.prefix("ic| ");
        IC(1, 2);
        REQUIRE(sstr.str() == "ic| 1: 1, 2: 2\n");
        sstr.str("");
    }
}

// -------------------------------------------------- Test character
TEST_CASE("character")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    {
        auto v0 = char {'a'};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: 'a'\n");
        sstr.str("");
    }
#if !defined(__APPLE__)
    {
        auto v0 = wchar_t {L'a'};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: 'a'\n");
        sstr.str("");
    }

#if defined(__cpp_char8_t)
    {
        auto v0 = char8_t {u8'a'};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: 'a'\n");
        sstr.str("");
    }
#endif

    {
        auto v0 = char16_t {u'a'};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: 'a'\n");
        sstr.str("");
    }

    {
        auto v0 = char32_t {U'a'};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: 'a'\n");
        sstr.str("");
    }

    {
        auto v0 = char16_t {u'\0'};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: '\\0'\n");
        sstr.str("");
    }

    {
        auto v0 = char32_t {u'\t'};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: '\\t'\n");
        sstr.str("");
    }
#endif
}


// -------------------------------------------------- Test std::string
TEST_CASE("std_string")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    {
        auto v0 = std::string {"str 1"};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: \"str 1\"\n");
        sstr.str("");
    }
#if !defined(__APPLE__)
    {
        auto v0 = std::wstring {L"wstr 1"};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: \"wstr 1\"\n");
        sstr.str("");
    }

    {
        auto v0 = std::u16string {u"u16str 1"};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: \"u16str 1\"\n");
        sstr.str("");
    }

    {
        auto v0 = std::u32string {U"u32str 1"};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: \"u32str 1\"\n");
        sstr.str("");
    }
#endif
}

// -------------------------------------------------- Test std::string_view
#if defined (CPP_17)
TEST_CASE("std_string_view")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    {
        auto v0 = std::string_view {"str 1"};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: \"str 1\"\n");
        sstr.str("");
    }
#if !defined(__APPLE__)
    {
        auto v0 = std::wstring_view {L"wstr 1"};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: \"wstr 1\"\n");
        sstr.str("");
    }

    {
        auto v0 = std::u16string_view {u"u16str 1"};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: \"u16str 1\"\n");
        sstr.str("");
    }

    {
        auto v0 = std::u32string_view {U"u32str 1"};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: \"u32str 1\"\n");
        sstr.str("");
    }
#endif
}
#endif

// -------------------------------------------------- Test C string
TEST_CASE("c_string")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    icecream::ic.show_c_string(true);

    {
        char const* v0 = "Icecream test";
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: \"Icecream test\"\n");
        sstr.str("");
    }

    {
        char const* const v0 = "Icecream test";
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: \"Icecream test\"\n");
        sstr.str("");
    }

    {
        char v0[] = "string 2";
        char* const v1 = v0;
        IC(v1);
        REQUIRE(sstr.str() == "ic| v1: \"string 2\"\n");
        sstr.str("");
    }

    {
        char const* v0 = "Icecream test";
        char const*& v1 = v0;
        IC(v1);
        REQUIRE(sstr.str() == "ic| v1: \"Icecream test\"\n");
        sstr.str("");
    }

    {
        char const* const v0 = "string 5";
        icecream::ic.show_c_string(false);
        IC(v0);
        REQUIRE_THAT(sstr.str(), Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
        sstr.str("");
        icecream::ic.show_c_string(true);
    }
#if !defined(__APPLE__)
    {
        wchar_t const* v0 = L"wchar_t test";
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: \"wchar_t test\"\n");
        sstr.str("");
    }

    {
        char16_t const* v0 = u"char16_t test";
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: \"char16_t test\"\n");
        sstr.str("");
    }

    {
        char16_t const* v0 = u"char16_t test";
        icecream::ic.show_c_string(false);
        IC(v0);
        REQUIRE_THAT(sstr.str(), Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
        sstr.str("");
        icecream::ic.show_c_string(true);
    }

    {
        char32_t const* const v0 = U"char32_t test";
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: \"char32_t test\"\n");
        sstr.str("");
    }
#endif
}

// -------------------------------------------------- Test line wrap

TEST_CASE("line_wrap")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    icecream::ic.line_wrap_width(20);

    {
        auto v0 = std::vector<float> {1.1, 1.2};
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: [1.1, 1.2]\n");
        sstr.str("");
    }

    {
        auto v0 = std::list<int> {10, 20, 30, 40, 50, 60};
        auto const result =
            "ic| v0: [\n"
            "        10,\n"
            "        20,\n"
            "        30,\n"
            "        40,\n"
            "        50,\n"
            "        60\n"
            "    ]\n";
        IC(v0);
        REQUIRE(sstr.str() == result);
        sstr.str("");
    }

    {
        auto v0 = std::vector<std::vector<int>> {{1, 2}, {10, 20, 30, 40, 50, 60}, {3, 4}};
        auto const result =
            "ic| v0: [\n"
            "        [1, 2],\n"
            "        [\n"
            "            10,\n"
            "            20,\n"
            "            30,\n"
            "            40,\n"
            "            50,\n"
            "            60\n"
            "        ],\n"
            "        [3, 4]\n"
            "    ]\n";
        IC(v0);
        REQUIRE(sstr.str() == result);
        sstr.str("");
    }

    {
        icecream::ic.prefix("pref -> ");
        auto v0 = std::vector<float> {1.1, 1.2};
        auto v1 = std::vector<int> {11, 12};
        auto const result =
            "pref -> v0: [\n"
            "        1.1,\n"
            "        1.2\n"
            "    ]\n"
            "    v1: [11, 12]\n";
        IC(v0, v1);
        REQUIRE(sstr.str() == result);
        sstr.str("");
        icecream::ic.prefix("ic| ");
    }

    icecream::ic.line_wrap_width(70);
}

// -------------------------------------------------- Test exception

#include <boost/exception/all.hpp>

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

TEST_CASE("exception")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    {
        auto v0 = std::runtime_error("ble");
        IC(v0);
        REQUIRE(sstr.str() == "ic| v0: ble\n");
        sstr.str("");
    }

    {
        try
        {
            BOOST_THROW_EXCEPTION(exception {} << IntInfo {10});
        }
        catch (exception& e)
        {
            e << StringInfo {"bla_string"};
            IC(e);
            REQUIRE_THAT(sstr.str(),
                Catch::Contains("IntTag")
                && Catch::Contains("] = 10")
                && Catch::Contains("StringTag")
                && Catch::Contains("] = bla_string")
                && Catch::Contains("ic| e: what info")
            );
        }
    }
}

// -------------------------------------------------- Test formating string
TEST_CASE("formatting")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    using icecream::_;

    {
        auto v0 = int{42};
        IC(_("#x", v0), 7);
        REQUIRE(sstr.str() == "ic| v0: 0x2a, 7: 7\n");
        sstr.str("");
    }

    {
        auto v0 = int{42};
        IC_("#o", v0, 7);
        REQUIRE(sstr.str() == "ic| v0: 052, 7: 07\n");
        sstr.str("");
    }

    {
        auto v0 = int{42};
        IC( ((_("#", (v0) )   )), 7);
        REQUIRE(sstr.str() == "ic| v0: 42, 7: 7\n");
        sstr.str("");
    }

    {
        auto v0 = float{12.3456789};
        IC((_("#A", v0)));
        REQUIRE(
            ((sstr.str() == "ic| v0: 0X1.8B0FCEP+3\n") ||
             (sstr.str() == "ic| v0: 0X1.8B0FCE0000000P+3\n")) // Visualstudio 2019 output
        );
        sstr.str("");
    }

    {
        auto v0 = int{42};
        IC_("oA", v0);
        REQUIRE(sstr.str() == "ic| v0: *Error* on formatting string\n");
        sstr.str("");
    }
}

// -------------------------------------------------- Test dump struct clang

#if defined(ICECREAM_DUMP_STRUCT_CLANG)
#include <cwctype>
#include <cstdint>

template <typename T>
auto lowest() -> T
{
    return std::numeric_limits<T>::lowest();
}

template <typename T>
auto str_lowest() -> std::string
{
    return std::to_string(std::numeric_limits<T>::lowest());
}

template <typename T>
auto max() -> T
{
    return std::numeric_limits<T>::max();
}

template <typename T>
auto str_max() -> std::string
{
    return std::to_string(std::numeric_limits<T>::max());
}


TEST_CASE("dump_string")
{
    auto sstr = std::stringstream {};
    icecream::ic.stream().rdbuf(sstr.rdbuf());

    {
        struct S0
        {
            bool v_bool0;
            bool v_bool1;
            char v_char;
            signed char v_schar;
            unsigned char v_uchar;
        #if !defined(__APPLE__)
            wchar_t v_wchart;
            wchar_t const* v_pwchar;
            char16_t v_char16_t;
            char16_t const* v_pchar16_t;
            char32_t v_char32_t;
            char32_t const* v_pchar32_t;
        #endif
            char const* v_pchar;
        };
        auto v0 = S0 {
            false,         // v_bool0
            true,          // v_bool1
            'z',           // v_char
            -20,           // v_schar;
            200,           // v_uchar;
        #if !defined(__APPLE__)
            L'w',          // v_wchart
            L"wchar test", // v_pwchart
            u'r',          // v_char16_t
            u"pchar16",    // v_pchar16_t
            U'R',          // v_char32_t
            U"pchar32",    // v_pchar32_t
        #endif
            "test",        // v_pchar
        };
        auto const result =
            "ic| v0: {\n"
            "        v_bool0: 0,\n"
            "        v_bool1: 1,\n"
            "        v_char: 'z',\n"
            "        v_schar: -20,\n"
            "        v_uchar: 200,\n"
        #if !defined(__APPLE__)
            "        v_wchart: 'w',\n"
            "        v_pwchar: \"wchar test\",\n"
            "        v_char16_t: 'r',\n"
            "        v_pchar16_t: \"pchar16\",\n"
            "        v_char32_t: 'R',\n"
            "        v_pchar32_t: \"pchar32\",\n"
        #endif
            "        v_pchar: \"test\"\n"
            "    }\n";
        IC(v0);
        REQUIRE(sstr.str() == result);
        sstr.str("");
    }

    {
        struct S0
        {
            signed char v_sigchar;
            short v_short;
            int v_int;
            long v_long;
            long long v_longlong;
            std::intmax_t v_intmax_t;
            std::ptrdiff_t v_ptrdiff_t;
        };
        auto v0 = S0 {
            lowest<signed char>(),    // v_sigchar
            lowest<short>(),          // v_short
            lowest<int>(),            // v_int
            lowest<long>(),           // v_long
            lowest<long long>(),      // v_longlong
            lowest<std::intmax_t>(),  // v_intmax_t
            lowest<std::ptrdiff_t>(), // v_ptrdiff_t
        };
        auto const result =
            "ic| v0: {\n"
            "        v_sigchar: " + str_lowest<signed char>() + ",\n"
            "        v_short: " + str_lowest<short>() + ",\n"
            "        v_int: " + str_lowest<int>() + ",\n"
            "        v_long: " + str_lowest<long>() + ",\n"
            "        v_longlong: " + str_lowest<long long>() + ",\n"
            "        v_intmax_t: " + str_lowest<std::intmax_t>() + ",\n"
            "        v_ptrdiff_t: " + str_lowest<std::ptrdiff_t>() + "\n"
            "    }\n";
        IC(v0);
        REQUIRE(sstr.str() == result);
        sstr.str("");
    }

    {
        struct S0
        {
            unsigned char v_unsigchar;
            unsigned short v_unsigshort;
            unsigned int v_unsigint;
            unsigned long v_unsiglong;
            unsigned long long v_unsiglonglong;
            std::uintmax_t v_uintmax_t;
            std::size_t v_size_t;
        };
        auto v0 = S0 {
            max<unsigned char>(),      // v_unsigchar
            max<unsigned short>(),     // v_unsigshort
            max<unsigned int>(),       // v_unsigint
            max<unsigned long>(),      // v_unsiglong
            max<unsigned long long>(), // v_unsiglonglong
            max<std::uintmax_t>(),     // v_uintmax_t
            max<std::size_t>(),        // v_size_t
        };
        auto const result =
            "ic| v0: {\n"
            "        v_unsigchar: " + str_max<unsigned char>() + ",\n"
            "        v_unsigshort: " + str_max<unsigned short>() + ",\n"
            "        v_unsigint: " + str_max<unsigned int>() + ",\n"
            "        v_unsiglong: " + str_max<unsigned long>() + ",\n"
            "        v_unsiglonglong: " + str_max<unsigned long long>() + ",\n"
            "        v_uintmax_t: " + str_max<std::uintmax_t>() + ",\n"
            "        v_size_t: " + str_max<std::size_t>() + "\n"
            "    }\n";
        IC(v0);
        REQUIRE(sstr.str() == result);
        sstr.str("");
    }

    {
        struct S0
        {
            float v_float;
            double v_double;
            long double v_longdouble;
        };
        auto v0 = S0 {
            -60.6,  // v_float
            -70.7,  // v_double
            100.1   // v_longdouble
        };
        auto const result = "ic| v0: {v_float: -60.6, v_double: -70.7, v_longdouble: 100.1}\n";
        IC(v0);
        REQUIRE(sstr.str() == result);
        sstr.str("");
    }

    {
        using MyFloat = float;
        using MyDouble = double;
        struct S0
        {
            MyFloat v_float0;
            double v_double0;
            MyFloat v_float1;
            MyDouble v_double1;
            double v_double2;
            float v_float2;
            long double v_longdouble;
        };
        auto v0 = S0 {
            -60.6,  // v_float0
            -70.7,  // v_double0
            60.6,   // v_float1
            70.7,   // v_double1
            3.14,   // v_double2
            10.01,  // v_float2
            200.002 // v_longdouble
        };
        auto const result =
            "ic\\| v0: \\{\n"
            "        v_float0: [0-9e\\-+.]+ <NOT RELIABLE, see issue #6 on github>,\n"
            "        v_double0: -70\\.7,\n"
            "        v_float1: [0-9e\\-+.]+ <NOT RELIABLE, see issue #6 on github>,\n"
            "        v_double1: 70\\.7 <NOT RELIABLE, see issue #6 on github>,\n"
            "        v_double2: 3\\.14,\n"
            "        v_float2: 10\\.01,\n"
            "        v_longdouble: 200\\.002\n"
            "    \\}\n";

        IC(v0);
        REQUIRE_THAT(sstr.str(), Catch::Matches(result));
        sstr.str("");
    }

    {
        using MyFloat = float;
        struct S0
        {
            bool v_bool[10];

            short const v_sint[2];
            short unsigned v_usint[2];
            int const v_int[3];
            unsigned int v_uint[2];
            long int v_lint[2];
            unsigned long int v_ulint[2];
            long long int v_llint[2];
            unsigned long const long int v_ullint[2];
            signed char v_schar[2];
            unsigned char v_uchar[2];

            char v_char[4];
        #if !defined(__APPLE__)
            wchar_t v_wchar[6];
            char16_t v_char16[3];
            char32_t v_char32[4];
        #endif

            float v_float[2];
            MyFloat mfloat[2];
            double v_double[3];
            long double v_ldouble[3];

            int* v_pint[3];
        };
        auto v0 = S0 {
            {true, true, false, true, false, false, false, true, true, true},  // v_sint
            {lowest<short int>(), max<short int>()},                           // v_sint
            {lowest<unsigned short int>(), max<unsigned short int>()},         // v_usint
            {lowest<int>(), max<int>(), 3},                                    // v_int
            {lowest<unsigned int>(), max<unsigned int>()},                     // v_uint
            {lowest<long int>(), max<long int>()},                             // v_lint
            {lowest<unsigned long int>(), max<unsigned long int>()},           // v_ulint
            {lowest<long long int>(), max<long long int>()},                   // v_llint
            {lowest<unsigned long long int>(), max<unsigned long long int>()}, // v_ullint
            {lowest<signed char>(), max<signed char>()},                       // v_schar
            {lowest<unsigned char>(), max<unsigned char>()},                   // v_uchar

            {'c', 'h', 'a', 'r'},                                              // v_char
        #if !defined(__APPLE__)
            L"wchar",                                                          // v_wchar
            {u'1', u'2', u'3'},                                                // v_char16
            {U'3', U'a', U'b', U'c'},                                          // v_char32
        #endif

            {1.1, 1.2},                                                        // v_float
            {2.1, 2.2},                                                        // mfloat
            {3.1, 3.2, 3.3},                                                   // v_double
            {103.1, 103.2, 103.3},                                             // v_ldouble

            {(int*)0x70, (int*)0x71, (int*)0x72},                              // v_pint
        };
        IC(v0);
        auto const result =
            "ic| v0: {\n"
            "        v_bool: [1, 1, 0, 1, 0, 0, 0, 1, 1, 1],\n"
            "        v_sint: [" + str_lowest<short int>() + ", " + str_max<short int>() + "],\n"
            "        v_usint: [" + str_lowest<unsigned short int>() + ", " + str_max<unsigned short int>() + "],\n"
            "        v_int: [" + str_lowest<int>() + ", " + str_max<int>() + ", 3],\n"
            "        v_uint: [" + str_lowest<unsigned int>() + ", " + str_max<unsigned int>() + "],\n"
            "        v_lint: [" + str_lowest<long int>() + ", " + str_max<long int>() + "],\n"
            "        v_ulint: [" + str_lowest<unsigned long int>() + ", " + str_max<unsigned long int>() + "],\n"
            "        v_llint: [" + str_lowest<long long int>() + ", " + str_max<long long int>() + "],\n"
            "        v_ullint: [" + str_lowest<unsigned long long int>() + ", " + str_max<unsigned long long int>() + "],\n"
            "        v_schar: [" + str_lowest<signed char>() + ", " + str_max<signed char>() + "],\n"
            "        v_uchar: [" + str_lowest<unsigned char>() + ", " + str_max<unsigned char>() + "],\n"
            "        v_char: ['c', 'h', 'a', 'r'],\n"
        #if !defined(__APPLE__)
            "        v_wchar: ['w', 'c', 'h', 'a', 'r', '\\0'],\n"
            "        v_char16: ['1', '2', '3'],\n"
            "        v_char32: ['3', 'a', 'b', 'c'],\n"
        #endif
            "        v_float: [1.1, 1.2],\n"
            "        mfloat: <type aliased array, see issue #7 on github>,\n"
            "        v_double: [3.1, 3.2, 3.3],\n"
            "        v_ldouble: [103.1, 103.2, 103.3],\n"
            "        v_pint: [0x70, 0x71, 0x72]\n"
            "    }\n";
        REQUIRE(sstr.str() == result);
        sstr.str("");
    }

    {
        struct S0
        {
            int * const* ** *const* *const * i;
            const double* d;
        };
        auto v0 = S0 {
            nullptr,
            (double*)0x56,
        };
        IC(v0);
        auto const result = "ic\\| v0: \\{i: (0x)*0+, d: (0x)*56\\}\n";
        REQUIRE_THAT(sstr.str(), Catch::Matches(result));
        sstr.str("");
    }

    {
        struct S0
        {
            double double0;

            struct S1
            {
                char ch1;
                int i1;
            } s1;

            struct S2
            {
                char ch2;

                struct S3
                {
                    float float3;
                    int int3;
                } s1;

                int i2;
            } s2;
        };

        auto v0 = S0 {60.6, {'a', 1}, {'b', {3.14, 7}, 9}};
        IC(v0);
        auto const result =
            "ic| v0: {\n"
            "        double0: 60.6,\n"
            "        s1: {ch1: 'a', i1: 1},\n"
            "        s2: {ch2: 'b', s1: {float3: 3.14, int3: 7}, i2: 9}\n"
            "    }\n";
        REQUIRE(sstr.str() == result);
        sstr.str("");
    }

    {
        enum EN
        {
            ZERO,
            UM,
            DOIS,
            TRES
        };

        struct S0
        {
            EN en;
        };

        auto v0 = S0 {DOIS};
        IC(v0);
        auto const result =
            "ic| v0: {en: <enum member, see issue #7 on github>}\n";

        REQUIRE(sstr.str() == result);
        sstr.str("");
    }

    {
        enum EN
        {
            ZERO,
            UM,
            DOIS,
            TRES
        };

        struct S1
        {
            int i0;
            int i1;
        };

        struct S0
        {
            S1 v_s1[2];
            EN v_en[3];
        };
        auto v0 = S0 {{{10, 11}, {21, 22}}, {TRES, ZERO, UM}};
        IC(v0);
        auto const result =
            "ic| v0: {\n"
            "        v_s1: <array of structs, see issue #7 on github>,\n"
            "        v_en: <array of enums, see issue #7 on github>\n"
            "    }\n";
        REQUIRE(sstr.str() == result);
        sstr.str("");
    }

    {
        struct S0
        {
            std::int8_t v_int8[2];
            std::int16_t v_int16[2];
            std::int32_t v_int32[2];
            std::int64_t v_int64[2];

            std::uint8_t v_uint8[2];
            std::uint16_t v_uint16[2];
            std::uint32_t v_uint32[2];
            std::uint64_t v_uint64[2];

            std::size_t v_size[2];
            std::ptrdiff_t v_ptrdiff[2];
        };
        auto v0 = S0 {
            {lowest<std::int8_t>(), max<std::int8_t>()},        // v_int8
            {lowest<std::int16_t>(), max<std::int16_t>()},      // v_int16
            {lowest<std::int32_t>(), max<std::int32_t>()},      // v_int32
            {lowest<std::int64_t>(), max<std::int64_t>()},      // v_int64
            {lowest<std::uint8_t>(), max<std::uint8_t>()},      // v_uint8
            {lowest<std::uint16_t>(), max<std::uint16_t>()},    // v_uint16
            {lowest<std::uint32_t>(), max<std::uint32_t>()},    // v_uint32
            {lowest<std::uint64_t>(), max<std::uint64_t>()},    // v_uint64
            {lowest<std::size_t>(), max<std::size_t>()},        // v_size
            {lowest<std::ptrdiff_t>(), max<std::ptrdiff_t>()},  // v_ptrdiff
        };
        IC(v0);
        auto const result =
            "ic| v0: {\n"
            "        v_int8: [" + str_lowest<std::int8_t>() + ", " + str_max<std::int8_t>() + "],\n"
            "        v_int16: [" + str_lowest<std::int16_t>() + ", " + str_max<std::int16_t>() + "],\n"
            "        v_int32: [" + str_lowest<std::int32_t>() + ", " + str_max<std::int32_t>() + "],\n"
            "        v_int64: [" + str_lowest<std::int64_t>() + ", " + str_max<std::int64_t>() + "],\n"
            "        v_uint8: [" + str_lowest<std::uint8_t>() + ", " + str_max<std::uint8_t>() + "],\n"
            "        v_uint16: [" + str_lowest<std::uint16_t>() + ", " + str_max<std::uint16_t>() + "],\n"
            "        v_uint32: [" + str_lowest<std::uint32_t>() + ", " + str_max<std::uint32_t>() + "],\n"
            "        v_uint64: [" + str_lowest<std::uint64_t>() + ", " + str_max<std::uint64_t>() + "],\n"
            "        v_size: [" + str_lowest<std::size_t>() + ", " + str_max<std::size_t>() + "],\n"
            "        v_ptrdiff: [" + str_lowest<std::ptrdiff_t>() + ", " + str_max<std::ptrdiff_t>() + "]\n"
            "    }\n";
        REQUIRE(sstr.str() == result);
        sstr.str("");
    }
}
#endif
