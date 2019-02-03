#include "icecream.hpp"


auto sum(int i0, int i1) -> int
{
    return i0 + i1;
}


int main(int argc, char *argv[])
{
    auto i0 = int {7};
    auto d0 = double {3.14};

    IC();
    IC(i0);
    IC(i0, d0);
    IC((sum(40, 2)));

    return 0;
}
