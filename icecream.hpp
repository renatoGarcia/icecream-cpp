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
#ifndef ICECREAM_HPP_INCLUDED
#define ICECREAM_HPP_INCLUDED

#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>


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


#if defined(ICECREAM_LONG_NAME)
    #define ICECREAM(...) ::icecream::print{__FILE__, __LINE__, ICECREAM_FUNCTION, #__VA_ARGS__, __VA_ARGS__}
#else
    #define IC(...) ::icecream::print{__FILE__, __LINE__, ICECREAM_FUNCTION, #__VA_ARGS__, __VA_ARGS__}
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
            std::vector<std::string> const& arg_names,
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
                this->print_all_args(std::begin(arg_names), std::forward<Ts>(args)...);
            }

            std::cout << std::endl;
        }

    private:

        template<typename T>
        auto print_arg(std::string const& name, T&& value) -> void
        {
            std::cout << name << ": " << value;
        }

        template<typename TArg, typename... Ts>
        auto print_all_args(std::vector<std::string>::const_iterator arg_name, TArg&& arg_value, Ts&&... args_tail) -> void
        {
            this->print_arg(*arg_name, arg_value);
            if (sizeof...(Ts) > 0)
            {
                std::cout << ", ";
                this->print_all_args(++arg_name, std::forward<Ts>(args_tail)...);
            }
        }

        auto print_all_args(std::vector<std::string>::const_iterator) -> void {}
    };

    namespace
    {
        Icecream ic {};
    }

    struct print
    {
        // An empty IC() macro will expand to
        // ::icecream::print{__FILE__, __LINE__, ICECREAM_FUNCTION, "",}
        // A macro like IC(foo, bar) will expand to
        // ::icecream::print{__FILE__, __LINE__, ICECREAM_FUNCTION, "foo, bar", foo, bar}
        template<typename... Ts>
        print(std::string const& file, int line, std::string const& function, std::string const& arg_names, Ts&&... args)
        {
            auto split_names = std::vector<std::string> {};
            auto b_it = std::begin(arg_names);
            auto it = std::begin(arg_names);
            int par_count = 0;

            // Split the the arg_names
            while (true)
            {
                if (it == std::end(arg_names) || (*it == ',' && par_count == 0))
                {
                    // Remove the trailing spaces
                    auto e_it = std::prev(it);
                    while (*e_it == ' ') --e_it;
                    ++e_it;

                    // Remove the leading spaces
                    while (*b_it == ' ') ++b_it;

                    split_names.emplace_back(b_it, e_it);
                    b_it = std::next(it);
                }
                else if (*it == '(')
                {
                    ++par_count;
                }
                else if (*it == ')')
                {
                    --par_count;
                }

                if (it == std::end(arg_names))
                {
                    break;
                }
                else
                {
                    ++it;
                }
            }

            ::icecream::ic.print(file, line, function, split_names, std::forward<Ts>(args)...);
        }
    };

} // namespace icecream


#endif // ICECREAM_HPP_INCLUDED
