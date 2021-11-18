#pragma once
#include "internal/internal.hpp"
#include "util/util.hpp"
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

    template <reference_type RefType2>
    class object
    {
    public:
        object_ref ref;

        object(object_ref ref)
            : ref(ref)
        {

        }

        object(object &object) = delete;

        object(object &&object)
            : ref(object.ref)
        {
            object.ref = nullptr;
        }

        virtual operator bool() const
        {
            return !null_ref();
        }

        bool null_ref() const
        {
            return ref == nullptr;
        }

        
        virtual ~object()
        {
            if (this->ref == nullptr)
                return;

            if constexpr (RefType2 == local)
            {
                detail::get_local_env()->DeleteLocalRef(this->ref);
            }
            else if constexpr (RefType2 == global)
            {
                detail::get_local_env()->DeleteGlobalRef(this->ref);
            }
        }
    };
}