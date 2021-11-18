#pragma once
#include <jvmti.h>
#include "../../jni/detail/detail.hpp"

namespace jvmti::detail
{
    jvmtiEnv* environment;

    jvmtiEnv* get_environment()
    {
        if(jni::detail::local_jni_env == nullptr)
        {
            jni::detail::get_local_env();
        }

        if(environment == nullptr)
        {
            //TODO: Check error here.
            jni::detail::get_java_vm()->GetEnv((void**)&environment, JVMTI_VERSION_1);
        }

        return environment;
    }   
}