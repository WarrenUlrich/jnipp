#pragma once
#include <jni.h>
#include <array>

namespace jni::util
{
    template<class... Args>
    constexpr std::size_t args_count()
    {
        return sizeof...(Args);
    }

    template<class T>
    jvalue to_jvalue(const T& v)
    {
        jvalue result{};

        if constexpr (std::is_same<T, object<::local>>::value)
        {
            result.l = v.obj;
        }
        else if constexpr (std::is_same<T, object<::global>>::value)
        {
            result.l = v.obj;
        }
        else if constexpr (std::is_same<T, bool>::value)
        {
            result.z = (jboolean)v;
        }
        else if constexpr (std::is_same<T, std::byte>::value || std::is_same<T, unsigned char>::value)
        {
            result.b = v;
        }
        else if constexpr (std::is_same<T, short>::value || std::is_same<T, std::int16_t>::value)
        {
            result.s = v;
        }
        else if constexpr (std::is_same<T, int>::value)
        {
            result.i = v;
        }
        else if constexpr (std::is_same<T, std::int64_t>::value)
        {
            result.j = v;
        }
        else if constexpr (std::is_same<T, float>::value)
        {
            result.f = v;
        }
        else if constexpr (std::is_same<T, double>::value)
        {
            result.d = v;
        }
        else
        {
            []<bool flag = false>() {static_assert(flag, "unsupported template type in to_jvalue");}();
        }
        return result;
    }

    template<class... Args>
    const jvalue* args_to_jvalues(Args... args)
    {
        constexpr auto size = args_count<Args...>();
        std::array<jvalue, size> result = std::array<jvalue, size>();
       
        return result.data();
    }
}