#include <vector>
#include <list>
#include <map>
#include <optional>


#include "icecream.hpp"

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


// -------------------------------------------------- Test optional

#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
auto operator<<(std::ostream& os, std::optional<MyClass> const& value) -> std::ostream&
{
    std::cout << "||opt MyClass|| ";
    if (value.has_value())
    {
        std::cout << *value;
    }
    else
    {
        std::cout << "nullopt";
    }
    return os;
}

auto test_optional() -> void
{
    auto s0 = std::optional<int> {10};
    auto s1 = std::optional<int> {};
    auto s2 = std::optional<MyClass> {1};

    auto b0 = boost::optional<double> {20.0};
    auto b1 = boost::optional<double> {};

    IC(s0);
    IC(s1);
    IC(s2);

    IC(b0);
    IC(b1);
}


// -------------------------------------------------- Test pair

#include <utility>
auto operator<<(std::ostream& os, std::pair<int, double> const& value) -> std::ostream&
{
    std::cout << "{" << value.first << "," << value.second << "}";
    return os;
}

auto test_pair() -> void
{
    auto s0 = std::pair<int, double> {10, 3.14};
    auto s1 = std::make_pair("oi", 'b');

    IC(s0);
    IC(s1);
}


// -------------------------------------------------- Test iterable

template<typename T>
auto operator<<(std::ostream& os, std::vector<T> const&) -> std::ostream&
{
    std::cout << "print vector";
    return os;
}

auto test_iterable() -> void
{
    auto v0 = std::list<int> {10, 20, 30};
    auto v1 = std::vector<float> {1.1, 1.2};

    IC(v0);
    IC(v1);
}


// -------------------------------------------------- Test pointer like
#include <boost/smart_ptr.hpp>

auto test_pointer_like() -> void
{
    auto v0 = std::make_shared<int>(7);
    auto v1 = boost::make_shared<int>(33);
    int* v2 = new int {40};
    float* v3 = nullptr;
    auto v4 = std::unique_ptr<double> {};
    auto v5 = std::weak_ptr<int> {v0};

    using namespace icecream;

    IC(v0);
    IC(v1);
    IC(v2);
    IC(v3);
    IC(v4);
    IC(v5);
    ic.show_pointed_value(false);
    IC(v0);
    IC(v1);
    IC(v2);
    IC(v3);
    IC(v4);
    IC(v5);

    delete v2;
}


// -------------------------------------------------- Test common
auto test_common() -> void
{
    auto i0 = int {7};
    auto d0 = double {3.14};
    auto v0 = std::vector<int> {1, 2, 3, 4, 5};

    IC();
    IC(i0);
    IC(i0, d0);
    IC(i0, d0, 10, 30);
    IC((sum(40, 2)), i0   , (((sum(3, 5)))));
    IC(v0);

    MyClass mc {20};
    IC(mc);
}

// -------------------------------------------------- Test prefix
auto test_prefix() -> void
{
    icecream::ic.prefix("pref -> ");
    IC(1, 2);
    icecream::ic.prefix([](){return "icecream -- ";});
    IC(1, 2);
    icecream::ic.prefix("ic| ");
    IC(1, 2);
}

// -------------------------------------------------- Test char_p
auto test_char_p() -> void
{
    char const* str0 = "Icecream test";
    IC(str0);

    char const* const str1 = "Icecream test";
    IC(str1);

    char str2[] = "string 2";
    IC(str2);

    char* const str3 = str2;
    IC(str3);

    char const*& str4 = str0;
    IC(str4);
    // char const* const str1 = "Icecream test";
}

// --------------------------------------------------

int main(int, char *[])
{
    test_common();
    test_optional();
    test_pair();
    test_iterable();
    test_pointer_like();
    test_prefix();
    test_char_p();

    return 0;
}
