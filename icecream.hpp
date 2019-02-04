/*
 * Copyright (c) 2019 The IceCream-Cpp Developers. See the AUTHORS file at the
 * top-level directory of this distribution and at
 * https://github.com/renatoGarcia/icecream-cpp
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef _ICECREAM_HPP_
#define _ICECREAM_HPP_

#include <iostream>
#include <string>
#include <utility>


#define ICECREAM_MAJOR_VERSION 0
#define ICECREAM_MINOR_VERSION 1
#define ICECREAM_PATCH_VERSION 0
#define ICECREAM_LIB_VERSION "0.1.0"


#if defined(__GNUC__)
    #define ICECREAM_FUNCTION __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
    #define ICECREAM_FUNCTION __FUNCSIG__
#else
    #define ICECREAM_FUNCTION __func__
#endif


#define ICECREAM_APPLY(macro, a0) macro(a0)

#define ICECREAM_LIST_SIZE(...) ICECREAM_LIST_SIZE_(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define ICECREAM_LIST_SIZE_(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...) N

#define ICECREAM_HEAD(a0, ...) a0
#define ICECREAM_TAIL(a0, ...) (__VA_ARGS__)

#define ICECREAM_FOREACH(macro, list) ICECREAM_FOREACH_(ICECREAM_LIST_SIZE list, macro, list)
#define ICECREAM_FOREACH_(N, macro, list) ICECREAM_FOREACH__(N, macro, list)
#define ICECREAM_FOREACH__(N, macro, list) ICECREAM_FOREACH_##N(macro, list)
#define ICECREAM_FOREACH_1(macro, list) macro list
#define ICECREAM_FOREACH_2(macro, list) ICECREAM_APPLY(macro, ICECREAM_HEAD list), ICECREAM_FOREACH_1(macro, ICECREAM_TAIL list)
#define ICECREAM_FOREACH_3(macro, list) ICECREAM_APPLY(macro, ICECREAM_HEAD list), ICECREAM_FOREACH_2(macro, ICECREAM_TAIL list)
#define ICECREAM_FOREACH_4(macro, list) ICECREAM_APPLY(macro, ICECREAM_HEAD list), ICECREAM_FOREACH_3(macro, ICECREAM_TAIL list)
#define ICECREAM_FOREACH_5(macro, list) ICECREAM_APPLY(macro, ICECREAM_HEAD list), ICECREAM_FOREACH_4(macro, ICECREAM_TAIL list)
#define ICECREAM_FOREACH_6(macro, list) ICECREAM_APPLY(macro, ICECREAM_HEAD list), ICECREAM_FOREACH_5(macro, ICECREAM_TAIL list)
#define ICECREAM_FOREACH_7(macro, list) ICECREAM_APPLY(macro, ICECREAM_HEAD list), ICECREAM_FOREACH_6(macro, ICECREAM_TAIL list)
#define ICECREAM_FOREACH_8(macro, list) ICECREAM_APPLY(macro, ICECREAM_HEAD list), ICECREAM_FOREACH_7(macro, ICECREAM_TAIL list)
#define ICECREAM_FOREACH_9(macro, list) ICECREAM_APPLY(macro, ICECREAM_HEAD list), ICECREAM_FOREACH_8(macro, ICECREAM_TAIL list)
#define ICECREAM_FOREACH_10(macro, list) ICECREAM_APPLY(macro, ICECREAM_HEAD list), ICECREAM_FOREACH_9(macro, ICECREAM_TAIL list)
#define ICECREAM_FOREACH_11(macro, list) ICECREAM_APPLY(macro, ICECREAM_HEAD list), ICECREAM_FOREACH_10(macro, ICECREAM_TAIL list)
#define ICECREAM_FOREACH_12(macro, list) ICECREAM_APPLY(macro, ICECREAM_HEAD list), ICECREAM_FOREACH_11(macro, ICECREAM_TAIL list)
#define ICECREAM_FOREACH_13(macro, list) ICECREAM_APPLY(macro, ICECREAM_HEAD list), ICECREAM_FOREACH_12(macro, ICECREAM_TAIL list)
#define ICECREAM_FOREACH_14(macro, list) ICECREAM_APPLY(macro, ICECREAM_HEAD list), ICECREAM_FOREACH_13(macro, ICECREAM_TAIL list)
#define ICECREAM_FOREACH_15(macro, list) ICECREAM_APPLY(macro, ICECREAM_HEAD list), ICECREAM_FOREACH_14(macro, ICECREAM_TAIL list)
#define ICECREAM_FOREACH_16(macro, list) ICECREAM_APPLY(macro, ICECREAM_HEAD list), ICECREAM_FOREACH_15(macro, ICECREAM_TAIL list)

#define ICECREAM_EXPAND_ARG(a0) #a0, a0

#if defined(ICECREAM_LONG_NAME)
    #define ICECREAM(...) ::icecream::print{__FILE__, __LINE__, ICECREAM_FUNCTION, ICECREAM_FOREACH(ICECREAM_EXPAND_ARG, (__VA_ARGS__))}
#else
    #define IC(...) ::icecream::print{__FILE__, __LINE__, ICECREAM_FUNCTION, ICECREAM_FOREACH(ICECREAM_EXPAND_ARG, (__VA_ARGS__))}
#endif


namespace icecream
{
    class Icecream
    {
    public:
        Icecream() = default;
        Icecream(Icecream const&) = delete;
        Icecream(Icecream&&) = delete;
        Icecream& operator=(Icecream const&) = delete;
        Icecream& operator=(Icecream&&) = delete;

        template<typename... Ts>
        auto print(
            std::string const& file,
            int line,
            std::string const& function,
            Ts&&... args
        ) -> void
        {
            std::cout << "ic| ";

            // If used an empty IC macro, i.e.: IC().
            if (sizeof...(Ts) == 0)
            {
                std::string::size_type const n = file.rfind('/');
                std::cout << file.substr(n+1) << ':' << line << " in \"" << function << '"';
            }
            else
            {
                this->print_all_args(std::forward<Ts>(args)...);
            }

            std::cout << std::endl;
        }

    private:

        template<typename T>
        auto print_arg(std::string const& name, T&& value) -> void
        {
            std::cout << name << ": " << value;
        }

        // Print all elements inside tupe, where the element at index `i` is a string with
        // the argument name, and the index `i+1` is that argument value.
        template<typename TArg, typename... Ts>
        auto print_all_args(std::string const& arg_name, TArg&& arg_value, Ts&&... args_tail) -> void
        {
            this->print_arg(arg_name, arg_value);
            if (sizeof...(Ts) > 0)
            {
                std::cout << ", ";
                this->print_all_args(std::forward<Ts>(args_tail)...);
            }
        }

        auto print_all_args() -> void {}
    };

    namespace
    {
        Icecream ic {};
    }

    struct print
    {
        // An empty IC() macro will expand to
        // ::icecream::print{__FILE__, __LINE__, ICECREAM_FUNCTION, "",}
        print(std::string const& file, int line, std::string const& function, char const*)
        {
            ::icecream::ic.print(file, line, function);
        }

        // A macro like IC(foo, bar) will expand to
        // ::icecream::print{__FILE__, __LINE__, ICECREAM_FUNCTION, "foo", foo, "bar", bar}
        template<typename... Ts>
        print(std::string const& file, int line, std::string const& function, Ts&&... args)
        {
            ::icecream::ic.print(file, line, function, std::forward<Ts>(args)...);
        }
    };

} // namespace icecream


#endif // _ICECREAM_HPP_
