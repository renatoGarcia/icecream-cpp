#include <cstdint>
#include <tuple>
#include <string>
#include <string_view>
#include <iostream>
#include <list>
#include <algorithm>
#include <forward_list>
#include <ranges>
#include <format>
#include <source_location>

#include <fmt/format.h>

#include "tests/common.hpp"

template<typename... Args> void whatis();

// #define BLA(...) __VA_ARGS__

namespace rv = std::views;
namespace vws = std::views;


// Computes approximate display width of a UTF-8 string.
// FMT_CONSTEXPR inline auto compute_width(string_view s) -> size_t {
//   size_t num_code_points = 0;
//   // It is not a lambda for compatibility with C++14.
//   struct count_code_points {
//     size_t* count;
//     FMT_CONSTEXPR auto operator()(uint32_t cp, string_view) const -> bool {
//       *count += detail::to_unsigned(
//           1 +
//           (cp >= 0x1100 &&
//            (cp <= 0x115f ||  // Hangul Jamo init. consonants
//             cp == 0x2329 ||  // LEFT-POINTING ANGLE BRACKET
//             cp == 0x232a ||  // RIGHT-POINTING ANGLE BRACKET
//             // CJK ... Yi except IDEOGRAPHIC HALF FILL SPACE:
//             (cp >= 0x2e80 && cp <= 0xa4cf && cp != 0x303f) ||
//             (cp >= 0xac00 && cp <= 0xd7a3) ||    // Hangul Syllables
//             (cp >= 0xf900 && cp <= 0xfaff) ||    // CJK Compatibility Ideographs
//             (cp >= 0xfe10 && cp <= 0xfe19) ||    // Vertical Forms
//             (cp >= 0xfe30 && cp <= 0xfe6f) ||    // CJK Compatibility Forms
//             (cp >= 0xff00 && cp <= 0xff60) ||    // Fullwidth Forms
//             (cp >= 0xffe0 && cp <= 0xffe6) ||    // Fullwidth Forms
//             (cp >= 0x20000 && cp <= 0x2fffd) ||  // CJK
//             (cp >= 0x30000 && cp <= 0x3fffd) ||
//             // Miscellaneous Symbols and Pictographs + Emoticons:
//             (cp >= 0x1f300 && cp <= 0x1f64f) ||
//             // Supplemental Symbols and Pictographs:
//             (cp >= 0x1f900 && cp <= 0x1f9ff))));
//       return true;
//     }
//   };
//   }

auto my_function(int i, int j){}

struct BLA
{};

template<>
struct std::formatter<BLA, char>
{
    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<class FmtContext>
    FmtContext::iterator format(BLA s, FmtContext& ctx) const
    {
        return std::format_to(ctx.out(), "{} {}", "bla", 15);
    }
};

template <>
struct fmt::formatter<BLA>
    : formatter<std::string_view>
{
  auto format(BLA c, format_context& ctx) const
    -> format_context::iterator
    {
        return formatter<std::string_view>::format("fmtlib bla", ctx);
    }
};

#include "icecream.hpp"
namespace idt = icecream::detail;

int main(int argc, char *argv[])
{
    using std::operator""s;

    // auto v0 = std::vector<int>{1, 0, 2, 3, 0, 4, 5};
    // int v0[] = {1, 0, 2, 3, 0, 4, 5};
    // auto v0  = {1, 0, 2, 3, 0, 4, 5};
    // auto v0 = "one,two,three"s;

    auto v0 = BLA{};
    // auto v0 = 16;
    IC(v0);
    // IC_F("", v0);

    // auto v1 = std::format("{}", MyClass{4});
    // IC(idt::is_stl_formattable<BLA>::value);


    // auto rv0 = rv::iota('a') | rv::enumerate | IC_V() | rv::take(3);
    // auto rv0 = vws::iota(0) | IC_FV("[::2]:#x", "bar") | vws::take(5);
    // auto rv0 = vws::iota('a') | vws::enumerate | IC_V(&std::get<1>) | vws::take(2);

    // auto rv0 = std::vector<int>{1, 0, 2, 3, 0, 4, 5}
    //     | vws::split(0)
    //     | IC_V()
    //     | vws::enumerate;

    // IC_FA("#x", my_function, 10, 20);
    // IC_F("[0:5:1]", bla);
    // IC(bla);
    // for (auto i : rv0)
    {
        // auto a = std::get<1>(i);
        // whatis<decltye(i)>();
        // IC(i);
    }
    // std::tuple<long int, std::tuple<double, int>, std::vector<int>> const aaa = {3, {1.0, 10}, {1,2,3}};
    // std::tuple<long int, double, int> aaa = {3, 1.0, 10};
    // IC(std::get<1>(aaa));
    // IC(aaa);

}
