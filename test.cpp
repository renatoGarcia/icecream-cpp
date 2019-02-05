#include "icecream.hpp"

class MyClass
{
public:
    MyClass() = delete;
    MyClass(const MyClass &other) = delete;
    MyClass(MyClass &&other) = delete;
    MyClass& operator=(const MyClass &other) = delete;
    MyClass& operator=(MyClass &&other) = delete;

    MyClass(int i_)
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


int main(int, char *[])
{
    auto i0 = int {7};
    auto d0 = double {3.14};

    IC();
    IC(i0);
    IC(i0, d0);
    IC(i0, d0, 10, 30);
    IC((sum(40, 2)), i0   , (((sum(3, 5)))));

    MyClass mc {20};
    IC(mc);

    return 0;
}
