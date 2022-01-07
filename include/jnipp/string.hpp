#pragma once
#include "object.hpp"

namespace jni
{
    template <reference_type RefType>
    class string : public object<RefType>
    {
    public:
        string(object<RefType> &&obj) : object<RefType>(std::move(obj))
        {
        }
    };
}