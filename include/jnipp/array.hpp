#pragma once
#include "object.hpp"
#include <cstddef>

namespace jni
{
    template <typename T, reference_type RefType = reference_type::local>
    class array : public object<RefType>
    {
    public:
        array(object<RefType> &&obj)
            : object<RefType>(std::move(obj))
        {
        }

        class iterator
        {
        public:
        
        private:
        };

    private:
    };
}