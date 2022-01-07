#pragma once
#include "object.hpp"
#include <map>
#include <string_view>
#include <memory>

namespace jni
{
    template <reference_type RefType = reference_type::global>
    class type : public object<RefType>
    {
    public:
        type() = default;

        type(object_ref ref) : object<RefType>(ref)
        {
        }

        type(type &other) = delete;

        type(type &&type) : object<RefType>(std::move(type))
        {
        }
    };
}