//
// Created by manu343726 on 22/04/15.
//

#ifndef TURBO_BOXED_TYPE_HPP
#define TURBO_BOXED_TYPE_HPP

namespace tml
{
    namespace haskell
    {
        template<typename metatype, typename ctor, typename value = ctor>
        struct boxed_type
        {
            using Type = metatype;
            using Ctor = ctor;
            using Value = value;

            struct noexpand {}; //Prevent evaluation during expression parameter expansion

            struct apply
            {
                using type = Value;
            };
        };

        template<typename Type, typename BoxedValue>
        struct is_a : std::false_type {};

        template<typename Type, typename Ctor, typename Value>
        struct is_a<Type, boxed_type<Type,Ctor,Value>> : std::true_type {};

        template<typename Ctor, typename BoxedValue>
        struct constructed_by : std::false_type {};

        template<typename Type, typename Ctor, typename Value>
        struct constructed_by<Ctor, boxed_type<Type,Ctor,Value>> : std::true_type {};

        struct Maybe
        {
            struct Just
            {
                template<typename T>
                struct apply
                {
                    using type = boxed_type<Maybe, Just, T>;
                };
            };

            struct Nothing
            {
                using type = boxed_type<Maybe, Nothing>;
            };
        };
    }
}

#define TURBO_IS_A(...) TICK_CLASS_REQUIRES(is_a<__VA_ARGS__>())
#define TURBO_MATCHES(...) TICK_CLASS_REQUIRES(constructed_by<__VA_ARGS__>())

#endif //TURBO_BOXED_TYPE_HPP
