#pragma once
#include "java_vm.hpp"

namespace jni
{
    namespace detail
    {
        jni::java_vm jvm = jni::java_vm();

        thread_local jni::environment env = jni::environment();
    }

    inline java_vm &get_jvm()
    {
        if (!detail::jvm)
        {
            detail::jvm = java_vm::get_created();
        }
        return detail::jvm;
    }

    environment &get_env()
    {
        if (!detail::env)
        {
            detail::env = get_jvm().get_env();
        }
        return detail::env;
    }
}