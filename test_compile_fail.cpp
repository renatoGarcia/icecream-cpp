// Header used to see the compiling message errors

#include "icecream.hpp"

#include <vector>
#include <list>
#include <map>
#include <sstream>

#include <optional>
#include <variant>

#include <boost/variant2/variant.hpp>


// Not printable type
struct C{};


int main(int argc, char *argv[])
{
    // auto v = C{};
    // auto v = std::variant<int, float, C> {7};
    // auto v = boost::variant2::variant<int, float, C> {7};

    IC(v);

    return 0;
}
