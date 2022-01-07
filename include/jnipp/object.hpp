#pragma once
#include <jni.h>
#include <stdexcept>

namespace jni
{
    enum reference_type
    {
        local,
        global
    };

    using object_ref = jobject;

    template <reference_type RefType>
    class object
    {
    public:
        object() noexcept
            : _ref(nullptr)
        {
        }

        object(jobject ref) noexcept
            : _ref(ref)
        {
        }

        object(object &&object) noexcept
            : _ref(std::exchange(object._ref, nullptr))
        {
        }

        object<RefType> &operator=(object &&object) noexcept
        {
            _ref = std::exchange(object._ref, nullptr);
            return *this;
        }

        object_ref unsafe_ref() const noexcept
        {
            return _ref;
        }

    private:
        object_ref _ref;
    };
}