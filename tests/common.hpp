#ifndef TESTS_COMMON_HPP_INCLUDED
#define TESTS_COMMON_HPP_INCLUDED

#include <ostream>

#if defined(_MSC_VER)
  #pragma warning(push)
  #pragma warning(disable: 4514)
#endif

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

// -------------------------------------------------- MutableStream

class MutableStream
{
public:
    explicit MutableStream(int i_)
        : i {i_}
    {}

    int i;

    friend auto operator<<(std::ostream& os, MutableStream& self) -> std::ostream&
    {
        os << "<MutableStream " << self.i << ">";
        return os;
    }
};

// -------------------------------------------------- NonFormattable

class NonFormattable
{
public:
    explicit NonFormattable(int i_)
        : i {i_}
    {}

    int i;
};


// -------------------------------------------------- IOStreamStdFmtFormattable

#if defined(ICECREAM_STL_FORMAT) && defined(ICECREAM_FMT_ENABLED)
class IOStreamStlFmtFormattable
{
public:
    explicit IOStreamStlFmtFormattable(int i_)
        : i {i_}
    {}

    int i;

    friend auto operator<<(std::ostream& os, IOStreamStlFmtFormattable const& self) -> std::ostream&
    {
        os << "<IOstream IOStreamStlFmtFormattable " << self.i << ">";
        return os;
    }
};

template<>
struct std::formatter<IOStreamStlFmtFormattable>
    : formatter<std::string>
{
    template<class FmtContext>
    auto format(IOStreamStlFmtFormattable v, FmtContext& ctx) const -> FmtContext::iterator
    {
        return std::format_to(ctx.out(), "<stl IOStreamStlFmtFormattable {}>", v.i);
    }
};

template <>
struct fmt::formatter<IOStreamStlFmtFormattable>
    : formatter<std::string>
{
    auto format(IOStreamStlFmtFormattable v, format_context& ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "<fmt IOStreamStlFmtFormattable {}>", v.i);
    }
};
#endif


// -------------------------------------------------- IOStreamStlFormattable

#if defined(ICECREAM_STL_FORMAT)
class IOStreamStlFormattable
{
public:
    explicit IOStreamStlFormattable(int i_)
        : i {i_}
    {}

    int i;

    friend auto operator<<(std::ostream& os, IOStreamStlFormattable const& self) -> std::ostream&
    {
        os << "<IOstream IOStreamStlFormattable " << self.i << ">";
        return os;
    }
};

template<>
struct std::formatter<IOStreamStlFormattable>
    : formatter<std::string>
{
    template<class FmtContext>
    auto format(IOStreamStlFormattable v, FmtContext& ctx) const -> FmtContext::iterator
    {
        return std::format_to(ctx.out(), "<stl IOStreamStlFormattable {}>", v.i);
    }
};

#endif


// -------------------------------------------------- IOStreamFmtFormattable

#if defined(ICECREAM_FMT_ENABLED)
class IOStreamFmtFormattable
{
public:
    explicit IOStreamFmtFormattable(int i_)
        : i {i_}
    {}

    int i;

    friend auto operator<<(std::ostream& os, IOStreamFmtFormattable const& self) -> std::ostream&
    {
        os << "<IOstream IOStreamFmtFormattable " << self.i << ">";
        return os;
    }
};

FMT_BEGIN_NAMESPACE
template <>
struct formatter<IOStreamFmtFormattable>
    : formatter<std::string>
{
    auto format(IOStreamFmtFormattable v, format_context& ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "<fmt IOStreamFmtFormattable {}>", v.i);
    }
};
FMT_END_NAMESPACE

#endif

// --------------------------------------------------------------------------------

#if defined(_MSC_VER)
  #pragma warning(pop)
#endif

#endif // TESTS_COMMON_HPP_INCLUDED
