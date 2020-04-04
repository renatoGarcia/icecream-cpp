#include "icecream.hpp"

#include <vector>
#include <list>
#include <map>
#include <sstream>

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
        REQUIRE_THAT(sstr.str(), Catch::StartsWith("ic| test.cpp:19 in"));
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
        REQUIRE(sstr.str() == "ic| (sum(40, 2)): 42, i0: 7, (((sum(3, 5)))): 8\n");
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

    {
        auto v0 = IC(7);
        REQUIRE(std::is_same<decltype(v0), int>::value);
        REQUIRE(v0 == 7);
    }

    {
        auto v0 = 'r';
        auto v1 = IC(v0);
        REQUIRE(std::is_same<decltype(v1), char>::value);
        REQUIRE(v1 == 'r');
    }

    {
        auto v0 = IC(7, 3.14);
        REQUIRE(std::is_same<decltype(v0), std::tuple<int, double>>::value);
        REQUIRE(v0 == std::make_tuple(7, 3.14));
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
