#ifndef TESTS_COMMON_HPP_INCLUDED
#define TESTS_COMMON_HPP_INCLUDED

#include <ostream>


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


#endif // TESTS_COMMON_HPP_INCLUDED
