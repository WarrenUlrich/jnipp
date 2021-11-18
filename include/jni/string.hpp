#pragma once
#include "object.hpp"

namespace jni
{
    template <reference_type RefType>
    class string : public object<RefType>
    {
    public:
        string(object_ref ref) : object<RefType>(ref)
        {

        }

        std::string get_string() const
        {
            auto cstr = detail::get_local_env()->GetStringUTFChars((jstring)this->ref, nullptr);
            auto str = std::string(cstr);
            detail::get_local_env()->ReleaseStringUTFChars((jstring)this->ref, cstr);
            return str;
        }
    };
}