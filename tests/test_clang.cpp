#include "icecream.hpp"

#include <cstdint>
#include <string>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


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
    {
        auto str = std::string{};
        icecream::ic.output(str);

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
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

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
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

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
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

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
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

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
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

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
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

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
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

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
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

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
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

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
    }

    {
        auto str = std::string{};
        icecream::ic.output(str);

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
    }
}
