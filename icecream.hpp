#ifndef _ICECREAM_HPP_
#define _ICECREAM_HPP_

#include <iostream>
#include <string>
#include <tuple>
#include <utility>


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

#define IC(...) ::icecream::print{__FILE__, __LINE__, __PRETTY_FUNCTION__, ICECREAM_FOREACH(ICECREAM_EXPAND_ARG, (__VA_ARGS__))}


namespace icecream
{
    template<std::size_t...>
    struct index_sequence
    {};

    template <std::size_t N, std::size_t... Ints>
    struct index_sequence_helper
        : public index_sequence_helper<N-1U, N-1U, Ints...>
    {};

    template <std::size_t... Ints>
    struct index_sequence_helper<0U, Ints...>
    {
        using type = index_sequence<Ints...>;
    };

    template <std::size_t N>
    using make_index_sequence = typename index_sequence_helper<N>::type;


    template<typename... Ts>
    auto head(std::tuple<Ts...> const& tup) -> decltype(std::get<0>(tup))&
    {
        return std::get<0>(tup);
    }

    template<std::size_t... Ns ,typename... Ts>
    auto tail_(index_sequence<Ns...>, std::tuple<Ts...>&& tup) -> decltype(std::forward_as_tuple(std::get<Ns+1u>(tup)...))
    {
        return std::forward_as_tuple(std::get<Ns+1u>(tup)...);
    }

    template<typename... Ts>
    auto tail(std::tuple<Ts...>&& tup) -> decltype(tail_(make_index_sequence<sizeof...(Ts) - 1u>(), std::move(tup)))
    {
        return tail_(make_index_sequence<sizeof...(Ts) - 1u>(), std::move(tup));
    }


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
            std::tuple<Ts...>&& args
        ) -> void
        {
            std::cout << "ic| ";

            // If used an empty IC macro, i.e.: IC().
            if (std::tuple_size<std::tuple<Ts...>>::value == 0)
            {
                std::string::size_type const n = file.rfind('/');
                std::cout << file.substr(n+1) << ':' << line << " in \"" << function << '"';
            }
            else
            {
                this->print_all_args(std::move(args));
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
        template<typename... Ts>
        auto print_all_args(std::tuple<Ts...>&& args) -> void
        {
            auto const& arg_name = head(args);
            auto&& arg_value = head(tail(std::move(args)));
            auto&& args_tail = tail(tail(std::move(args)));

            this->print_arg(arg_name, arg_value);
            if (std::tuple_size<std::tuple<Ts...>>::value > 2)
            {
                std::cout << ", ";
                this->print_all_args(std::move(args_tail));
            }
        }

        auto print_all_args(std::tuple<>&&) -> void {}
    };

    namespace
    {
        Icecream ic {};
    }

    struct print
    {
        // An empty IC() macro will expand to
        // ::icecream::print{__FILE__, __LINE__, __PRETTY_FUNCTION__, "",}
        print(std::string const& file, int line, std::string const& function, char const*)
        {
            ::icecream::ic.print(file, line, function, std::make_tuple());
        }

        // A macro like IC(foo, bar) will expand to
        // ::icecream::print{__FILE__, __LINE__, __PRETTY_FUNCTION__, "foo", foo, "bar", bar}
        template<typename... Ts>
        print(std::string const& file, int line, std::string const& function, Ts&&... args)
        {
            ::icecream::ic.print(file, line, function, std::forward_as_tuple(args...));
        }
    };

} // namespace icecream


#endif // _ICECREAM_HPP_
