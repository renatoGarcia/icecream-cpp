#include "icecream.hpp"

#include <vector>
#include <list>
#include <map>
#include <sstream>
#include <cstddef>
#include <limits>
#include <type_traits>

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

auto return_7() -> int
{
    return 7;
}

class MyClass
{
public:
    MyClass() = delete;
    MyClass(const MyClass &other) = delete;
    MyClass(MyClass &&other) = default;
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

    auto ret_val() -> int
    {
        return this->i;
    }

    auto multiply(int p) -> int
    {
        return this->i * p;
    }

    auto add(MyClass&& mc) -> int
    {
        return this->i + mc.i;
    }
};

auto sum(int i0, int i1) -> int
{
    return i0 + i1;
}

// -------------------------------------------------- Test output
TEST_CASE("output")
{
    {
        auto str = std::string{};
        icecream::ic.output(str);
        IC(1, 2);
        REQUIRE(str == "ic| 1: 1, 2: 2\n");
    }

    {
        auto sstr = std::ostringstream{};
        icecream::ic.output(sstr);
        IC(1, 2);
        REQUIRE(sstr.str() == "ic| 1: 1, 2: 2\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(std::inserter(str, str.end()));
        IC(1, 2);
        REQUIRE(str == "ic| 1: 1, 2: 2\n");
    }
}

// -------------------------------------------------- Test apply
TEST_CASE("apply")
{
    {
        auto str = std::string{};
        icecream::ic.output(str);
        auto mc = MyClass{50};

        auto r = IC_A(mc.ret_val);
        REQUIRE(str == "");
        REQUIRE(r == 50);
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);
        IC_A(id_f, MyClass{42});
        REQUIRE(str == "ic| MyClass{42}: <MyClass 42>\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);
        IC_A(sum, 1, 2);
        REQUIRE(str == "ic| 1: 1, 2: 2\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);
        IC_A_("#x", sum, 10, 20);
        REQUIRE(str == "ic| 10: 0xa, 20: 0x14\n");
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto mc = MyClass{3};
        auto r = IC_A(mc.multiply, 10);
        REQUIRE(str == "ic| 10: 10\n");
        REQUIRE(r == 30);
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

        auto mc = MyClass{3};
        auto r = IC_A(mc.add, MyClass{7});
        REQUIRE(str == "ic| MyClass{7}: <MyClass 7>\n");
        REQUIRE(r == 10);
    }
}

// -------------------------------------------------- Test base
TEST_CASE("base")
{
    auto str = std::string{};
    icecream::ic.output(str);

    auto i0 = int {7};
    auto d0 = double {3.14};
    auto v0 = std::vector<int> {1, 2, 3, 4, 5};
    MyClass mc {20};

    {
        icecream::ic.disable();
        IC(i0);
        REQUIRE(str == "");
        str.clear();
        icecream::ic.enable();
    }


    {
        test_empty_ic();
        REQUIRE_THAT(str, Catch::StartsWith("ic| test.cpp:22 in"));
        REQUIRE_THAT(str, Catch::Contains("test_empty_ic("));
        str.clear();
    }

    {
        IC(i0);
        REQUIRE(str == "ic| i0: 7\n");
        str.clear();
    }

    {
        IC(i0, d0);
        REQUIRE(str == "ic| i0: 7, d0: 3.14\n");
        str.clear();
    }

    {
        IC(i0, d0, 10, 30);
        REQUIRE(str == "ic| i0: 7, d0: 3.14, 10: 10, 30: 30\n");
        str.clear();
    }

    {
        IC((sum(40, 2)), i0   , (((sum(3, 5)))));
        REQUIRE(str == "ic| sum(40, 2): 42, i0: 7, sum(3, 5): 8\n");
        str.clear();
    }

    {
        IC(std::is_same<int, int> :: value);
        REQUIRE(str == "ic| std::is_same<int, int> :: value: 1\n");
        str.clear();
    }

    {
        IC(v0);
        REQUIRE(str == "ic| v0: [1, 2, 3, 4, 5]\n");
        str.clear();
    }

    {
        IC(mc);
        REQUIRE(str == "ic| mc: <MyClass 20>\n");
        str.clear();
    }
}


// -------------------------------------------------- Test return
TEST_CASE("return")
{
    auto str = std::string{};
    icecream::ic.output(str);

    using icecream::f_;

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

        auto&& v0 = IC(f_("0v#4x", a, b), 49);
        REQUIRE(std::is_same<decltype(v0), std::tuple<int&, int const&, int&&>&&>::value);
        REQUIRE(IC(f_("0v#4x", a, b), 49) == std::make_tuple(10, 20, 49));
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
    auto str = std::string{};
    icecream::ic.output(str);

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
    auto str = std::string{};
    icecream::ic.output(str);

    auto s0 = std::optional<int> {10};
    auto s1 = std::optional<int> {};
    auto s2 = std::optional<MyClass> {1};

    IC(s0);
    REQUIRE(str == "ic| s0: 10\n");
    str.clear();

    IC(s1);
    REQUIRE(str == "ic| s1: nullopt\n");
    str.clear();

    IC(s2);
    REQUIRE(str == "ic| s2: ||opt MyClass|| <MyClass 1>\n");
    str.clear();
}
#endif

// -------------------------------------------------- Test variant

#include <boost/variant2/variant.hpp>

TEST_CASE("boost_variant2")
{
    auto str = std::string{};
    icecream::ic.output(str);

    {
        auto v0 = boost::variant2::variant<int, double, char> {6.28};
        IC(v0);
        REQUIRE(str == "ic| v0: 6.28\n");
        str.clear();
    }

}


#if defined(CPP_17)
TEST_CASE("std_variant")
{
    auto str = std::string{};
    icecream::ic.output(str);

    {
        auto v0 = std::variant<int, double, char> {3.14};
        IC(v0);
        REQUIRE(str == "ic| v0: 3.14\n");
        str.clear();
    }
}
#endif

// -------------------------------------------------- Test filesystem

#if defined(CPP_17) && defined(__cpp_lib_filesystem)
#include <filesystem>
TEST_CASE("std_filesystem")
{
    auto str = std::string{};
    icecream::ic.output(str);

    {
        auto v0 = std::filesystem::path{"/one/two/three"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"/one/two/three\"\n");
        str.clear();
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
    auto str = std::string{};
    icecream::ic.output(str);

    {
        auto s0 = std::pair<int, char> {5, 'a'};
        IC(s0);
        REQUIRE(str == "ic| s0: (5, 'a')\n");
        str.clear();
    }

    {
        auto s0 = std::make_pair(std::string{"oi"}, 'b');
        IC(s0);
        REQUIRE(str == "ic| s0: {oi, b}\n"); // will use the function above
        str.clear();
    }

    {
        auto s0 = std::pair<int, double> {10, 3.14};
        IC(s0);
        REQUIRE(str == "ic| s0: (10, 3.14)\n");
        str.clear();
    }

    {
        auto s0 = std::make_tuple(1, 2.2, 'b', "bla");
        IC(s0);
        REQUIRE(str == "ic| s0: (1, 2.2, 'b', \"bla\")\n");
        str.clear();
    }

    {
        auto s0 = std::make_tuple(10, 20);
        IC_("x", s0);
        REQUIRE(str == "ic| s0: (a, 14)\n");
        str.clear();
    }
}


// -------------------------------------------------- Test arrays
TEST_CASE("arrays")
{
    auto str = std::string{};
    icecream::ic.output(str);

    {
        int v0[] = {1, 2, 3};
        IC(v0);
        REQUIRE(str == "ic| v0: [1, 2, 3]\n");
        str.clear();
    }

    {
        char v0[] = {'1', '2', '3'};
        IC(v0);
        REQUIRE(str == "ic| v0: ['1', '2', '3']\n");
        str.clear();
    }

    {
        char v0[] = "abc";
        IC(v0);
        REQUIRE(str == "ic| v0: ['a', 'b', 'c', '\\0']\n");
        str.clear();
    }

    {
        int v0[] = {10, 20, 30};
        IC_("#X", v0);
        REQUIRE(str == "ic| v0: [0XA, 0X14, 0X1E]\n");
        str.clear();
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
    auto str = std::string{};
    icecream::ic.output(str);

    {
        auto v0 = std::list<int> {10, 20, 30};
        IC(v0);
        REQUIRE(str == "ic| v0: [10, 20, 30]\n");
        str.clear();
    }

    {
        auto v0 = std::vector<float> {1.1, 1.2};
        IC(v0);
        REQUIRE(str == "ic| v0: [1.1, 1.2]\n");
        str.clear();
    }

    {
        int v0[] = {1, 2, 3};
        IC(v0);
        REQUIRE(str == "ic| v0: [1, 2, 3]\n");
        str.clear();
    }

    {
        auto v0 = MyIterable<NonPrintable> {};
        IC(v0);
        REQUIRE(str == "ic| v0: B<A>\n");
        str.clear();
    }

    {
        auto v0 = std::list<int> {10, 20, 30};
        IC_("0v#5x", v0);
        REQUIRE(str == "ic| v0: [0x00a, 0x014, 0x01e]\n");
        str.clear();
    }

}


// -------------------------------------------------- Test pointer like
#include <boost/smart_ptr.hpp>

TEST_CASE("pointer_like")
{
    auto str = std::string{};
    icecream::ic.output(str);

    {
        auto v0 = std::unique_ptr<int> {new int {10}};
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
        str.clear();
    }

    {
        auto v0 = std::make_shared<int>(7);
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
        str.clear();
    }

    {
        auto v0 = boost::make_shared<int>(33);
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
        str.clear();
    }

    {
        boost::scoped_ptr<int> v0 {new int {33}};
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
        str.clear();
    }

    {
        int* v0 = new int {40};
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
        str.clear();
        delete v0;
    }

    {
        float* v0 = nullptr;
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*0+\n"));
        str.clear();
    }

    {
        auto v0 = std::unique_ptr<double> {};
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*0+\n"));
        str.clear();
    }

    {
        auto v0 = std::make_shared<int>(7);
        auto v1 = std::weak_ptr<int> {v0};
        IC(v1);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v1: (0x)*[0-9a-fA-F]+\n"));
        str.clear();

        v0.reset();
        IC(v1);
        REQUIRE(str == "ic| v1: expired\n");
        str.clear();
    }

}


// -------------------------------------------------- Test prefix
TEST_CASE("prefix")
{
    auto str = std::string{};
    icecream::ic.output(str);

    {
        icecream::ic.prefix("pref -> ");
        IC(1, 2);
        REQUIRE(str == "pref -> 1: 1, 2: 2\n");
        str.clear();
    }

    {
        icecream::ic.prefix([](){return "icecream -- ";});
        IC(1, 2);
        REQUIRE(str == "icecream -- 1: 1, 2: 2\n");
        str.clear();
    }

    {
        icecream::ic.prefix("ic ", []{return 5;}, "| ");
        IC(1, 2);
        REQUIRE(str == "ic 5| 1: 1, 2: 2\n");
        str.clear();
    }

    {
        icecream::ic.prefix("id ", return_7, " | ");
        IC(1, 2);
        REQUIRE(str == "id 7 | 1: 1, 2: 2\n");
        str.clear();
    }

    {
        icecream::ic.prefix("ic| ");
        IC(1, 2);
        REQUIRE(str == "ic| 1: 1, 2: 2\n");
        str.clear();
    }
}

// -------------------------------------------------- Test character
TEST_CASE("character")
{
    auto str = std::string{};
    icecream::ic.output(str);

    {
        auto v0 = char {'a'};
        IC(v0);
        REQUIRE(str == "ic| v0: 'a'\n");
        str.clear();
    }

    {
        auto v0 = wchar_t {L'a'};
        IC(v0);
        REQUIRE(str == "ic| v0: 'a'\n");
        str.clear();
    }

#if defined(__cpp_char8_t)
    {
        auto v0 = char8_t {u8'a'};
        IC(v0);
        REQUIRE(str == "ic| v0: 'a'\n");
        str.clear();
    }
#endif

    {
        auto v0 = char16_t {u'a'};
        IC(v0);
        REQUIRE(str == "ic| v0: 'a'\n");
        str.clear();
    }

    {
        auto v0 = char16_t {u'\u03B1'}; // Greek Small Letter Alpha
        IC(v0);
        REQUIRE(str == "ic| v0: '\xce\xb1'\n");
        str.clear();
    }

    {
        auto v0 = char32_t {U'a'};
        IC(v0);
        REQUIRE(str == "ic| v0: 'a'\n");
        str.clear();
    }

    {
        auto v0 = char32_t {U'\U0001F427'}; // Penguin
        IC(v0);
        REQUIRE(str == "ic| v0: '\xF0\x9F\x90\xA7'\n");
        str.clear();
    }

    {
        auto v0 = char16_t {u'\0'};
        IC(v0);
        REQUIRE(str == "ic| v0: '\\0'\n");
        str.clear();
    }

    {
        auto v0 = char32_t {u'\t'};
        IC(v0);
        REQUIRE(str == "ic| v0: '\\t'\n");
        str.clear();
    }
}


// -------------------------------------------------- Test std::string
TEST_CASE("std_string")
{
    auto str = std::string{};
    icecream::ic.output(str);

    {
        auto v0 = std::string {"str 1"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"str 1\"\n");
        str.clear();
    }

    {
        auto v0 = std::wstring {L"wstr 1"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"wstr 1\"\n");
        str.clear();
    }

    {
        auto v0 = std::u16string {u"u16str \u03B1"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"u16str \xce\xb1\"\n");
        str.clear();
    }

    {
        auto v0 = std::u32string {U"u32str \U0001F427"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"u32str \xF0\x9F\x90\xA7\"\n");
        str.clear();
    }
}

// -------------------------------------------------- Test std::string_view
#if defined (CPP_17)
TEST_CASE("std_string_view")
{
    auto str = std::string{};
    icecream::ic.output(str);

    {
        auto v0 = std::string_view {"str 1"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"str 1\"\n");
        str.clear();
    }

    {
        auto v0 = std::string {"ABCDE"};
        auto v1 = std::string_view {v0.data() + 1, 3};
        IC(v1);
        REQUIRE(str == "ic| v1: \"BCD\"\n");
        str.clear();
    }

    {
        auto v0 = std::wstring_view {L"wstr 1"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"wstr 1\"\n");
        str.clear();
    }

    {
        auto v0 = std::u16string_view {u"u16str \u03B1"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"u16str \xce\xb1\"\n");
        str.clear();
    }

    {
        auto v0 = std::u32string_view {U"u32str \U0001F427"};
        IC(v0);
        REQUIRE(str == "ic| v0: \"u32str \xF0\x9F\x90\xA7\"\n");
        str.clear();
    }
}
#endif

// -------------------------------------------------- Test C string
TEST_CASE("c_string")
{
    auto str = std::string{};
    icecream::ic.output(str);

    icecream::ic.show_c_string(true);

    {
        char const* v0 = "Icecream test";
        IC(v0);
        REQUIRE(str == "ic| v0: \"Icecream test\"\n");
        str.clear();
    }

    {
        char const* const v0 = "Icecream test";
        IC(v0);
        REQUIRE(str == "ic| v0: \"Icecream test\"\n");
        str.clear();
    }

    {
        char v0[] = "string 2";
        char* const v1 = v0;
        IC(v1);
        REQUIRE(str == "ic| v1: \"string 2\"\n");
        str.clear();
    }

    {
        char const* v0 = "Icecream test";
        char const*& v1 = v0;
        IC(v1);
        REQUIRE(str == "ic| v1: \"Icecream test\"\n");
        str.clear();
    }

    {
        char const* const v0 = "string 5";
        icecream::ic.show_c_string(false);
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
        str.clear();
        icecream::ic.show_c_string(true);
    }

    {
        wchar_t const* v0 = L"wchar_t test";
        IC(v0);
        REQUIRE(str == "ic| v0: \"wchar_t test\"\n");
        str.clear();
    }

    {
        char16_t const* v0 = u"char16_t test \u03B1";
        IC(v0);
        REQUIRE(str == "ic| v0: \"char16_t test \xce\xb1\"\n");
        str.clear();
    }

    {
        char16_t const* v0 = u"char16_t test";
        icecream::ic.show_c_string(false);
        IC(v0);
        REQUIRE_THAT(str, Catch::Matches("ic\\| v0: (0x)*[0-9a-fA-F]+\n"));
        str.clear();
        icecream::ic.show_c_string(true);
    }

    {
        char32_t const* const v0 = U"char32_t test \U0001F427";
        IC(v0);
        REQUIRE(str == "ic| v0: \"char32_t test \xF0\x9F\x90\xA7\"\n");
        str.clear();
    }
}

// -------------------------------------------------- Test line wrap

TEST_CASE("line_wrap")
{
    auto str = std::string{};
    icecream::ic.output(str);

    icecream::ic.line_wrap_width(20);

    {
        auto v0 = std::vector<float> {1.1, 1.2};
        IC(v0);
        REQUIRE(str == "ic| v0: [1.1, 1.2]\n");
        str.clear();
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
        REQUIRE(str == result);
        str.clear();
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
        REQUIRE(str == result);
        str.clear();
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
        REQUIRE(str == result);
        str.clear();
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
    auto str = std::string{};
    icecream::ic.output(str);

    {
        auto v0 = std::runtime_error("ble");
        IC(v0);
        REQUIRE(str == "ic| v0: ble\n");
        str.clear();
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
            REQUIRE_THAT(str,
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
    auto str = std::string{};
    icecream::ic.output(str);

    using icecream::f_;

    {
        auto v0 = int{42};
        IC(f_("#x", v0), 7);
        REQUIRE(str == "ic| v0: 0x2a, 7: 7\n");
        str.clear();
    }

    {
        auto v0 = int{42};
        IC_("#o", v0, 7);
        REQUIRE(str == "ic| v0: 052, 7: 07\n");
        str.clear();
    }

    {
        auto v0 = int{42};
        IC( ((f_("#", (v0) )   )), 7);
        REQUIRE(str == "ic| v0: 42, 7: 7\n");
        str.clear();
    }

    {
        auto v0 = float{12.3456789};
        IC((f_("#A", v0)));
        REQUIRE(
            ((str == "ic| v0: 0X1.8B0FCEP+3\n") ||
             (str == "ic| v0: 0X1.8B0FCE0000000P+3\n")) // Visualstudio 2019 output
        );
        str.clear();
    }

    {
        auto v0 = int{42};
        IC_("oA", v0);
        REQUIRE(str == "ic| v0: *Error* on formatting string\n");
        str.clear();
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
    auto str = std::string{};
    icecream::ic.output(str);

    {
        struct S0
        {
            bool v_bool0;
            bool v_bool1;
            char v_char;
            signed char v_schar;
            unsigned char v_uchar;
            wchar_t v_wchart;
            wchar_t const* v_pwchar;
            char16_t v_char16_t;
            char16_t const* v_pchar16_t;
            char32_t v_char32_t;
            char32_t const* v_pchar32_t;
            char const* v_pchar;
        };
        auto v0 = S0 {
            false,         // v_bool0
            true,          // v_bool1
            'z',           // v_char
            -20,           // v_schar;
            200,           // v_uchar;
            L'w',          // v_wchart
            L"wchar test", // v_pwchart
            u'r',          // v_char16_t
            u"pchar16",    // v_pchar16_t
            U'R',          // v_char32_t
            U"pchar32",    // v_pchar32_t
            "test",        // v_pchar
        };
        auto const result =
            "ic| v0: {\n"
            "        v_bool0: 0,\n"
            "        v_bool1: 1,\n"
            "        v_char: 'z',\n"
            "        v_schar: -20,\n"
            "        v_uchar: 200,\n"
            "        v_wchart: 'w',\n"
            "        v_pwchar: \"wchar test\",\n"
            "        v_char16_t: 'r',\n"
            "        v_pchar16_t: \"pchar16\",\n"
            "        v_char32_t: 'R',\n"
            "        v_pchar32_t: \"pchar32\",\n"
            "        v_pchar: \"test\"\n"
            "    }\n";
        IC(v0);
        REQUIRE(str == result);
        str.clear();
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
        REQUIRE(str == result);
        str.clear();
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
        REQUIRE(str == result);
        str.clear();
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
        REQUIRE(str == result);
        str.clear();
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
        REQUIRE_THAT(str, Catch::Matches(result));
        str.clear();
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
            wchar_t v_wchar[6];
            char16_t v_char16[3];
            char32_t v_char32[4];

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
            L"wchar",                                                          // v_wchar
            {u'1', u'2', u'3'},                                                // v_char16
            {U'3', U'a', U'b', U'c'},                                          // v_char32

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
            "        v_wchar: ['w', 'c', 'h', 'a', 'r', '\\0'],\n"
            "        v_char16: ['1', '2', '3'],\n"
            "        v_char32: ['3', 'a', 'b', 'c'],\n"
            "        v_float: [1.1, 1.2],\n"
            "        mfloat: <type aliased array, see issue #7 on github>,\n"
            "        v_double: [3.1, 3.2, 3.3],\n"
            "        v_ldouble: [103.1, 103.2, 103.3],\n"
            "        v_pint: [0x70, 0x71, 0x72]\n"
            "    }\n";
        REQUIRE(str == result);
        str.clear();
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
        REQUIRE_THAT(str, Catch::Matches(result));
        str.clear();
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
        REQUIRE(str == result);
        str.clear();
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

        REQUIRE(str == result);
        str.clear();
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
        REQUIRE(str == result);
        str.clear();
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
        REQUIRE(str == result);
        str.clear();
    }
}
#endif


// -------------------------------------------------- Test Tree char count
TEST_CASE("Tree char count")
{
    {
        auto v0 = std::string {"str"};
        auto tree = icecream::detail::Tree{v0, std::ostringstream{}};
        REQUIRE(tree.count_chars() == 5); // 3 chars (str) plus 2 char (quotes)
    }

    {
        auto v0 = std::string {"\xce\xb1"}; // Greek small letter alpha
        auto tree = icecream::detail::Tree{v0, std::ostringstream{}};
        REQUIRE(tree.count_chars() == 3); // 1 chars plus 2 char (quotes)
    }

    {
        auto v0 = std::string {"\xF0\x9F\x90\xA7"}; // Penguin
        auto tree = icecream::detail::Tree{v0, std::ostringstream{}};
        REQUIRE(tree.count_chars() == 3); // 1 chars plus 2 char (quotes)
    }

    {
        auto v0 = std::wstring {L"wstr"};
        auto tree = icecream::detail::Tree{v0, std::ostringstream{}};
        REQUIRE(tree.count_chars() == 6);
    }

    {
        auto v0 = std::u16string {u"u16\u03B1"}; // Greek small letter alpha
        auto tree = icecream::detail::Tree{v0, std::ostringstream{}};
        REQUIRE(tree.count_chars() == 6);
    }

    {
        auto v0 = std::u32string {U"abcd\U0001F427"}; // Penguin
        auto tree = icecream::detail::Tree{v0, std::ostringstream{}};
        REQUIRE(tree.count_chars() == 7);
    }
}
