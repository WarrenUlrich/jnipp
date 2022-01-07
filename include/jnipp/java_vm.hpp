#pragma once
#include "environment.hpp"
#include "jvmti/environment.hpp"

namespace jni
{
    class environment;

    class java_vm
    {
    public:
        java_vm() noexcept
        : _jvm(nullptr)
        {
        }

        java_vm(JavaVM *jvm) noexcept
        : _jvm(jvm)
        {
        }

        java_vm(java_vm &&jvm) noexcept
        : _jvm(std::exchange(jvm._jvm, nullptr))
        {
        }

        java_vm &operator=(java_vm &&jvm) noexcept
        {
            _jvm = std::exchange(jvm._jvm, nullptr);
            return *this;
        }

        operator bool() const noexcept
        {
            return _jvm != nullptr;
        }

        environment get_env() const
        {
            JNIEnv *env;
            auto res = _jvm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);
            if (res == JNI_EDETACHED)
            {
                _jvm->AttachCurrentThread(reinterpret_cast<void **>(&env), nullptr);
            }
            return environment(env);
        }

        jvmti::environment get_jvmti_env() const
        {
            jvmtiEnv *buf;
            _jvm->GetEnv(reinterpret_cast<void **>(&buf), JVMTI_VERSION_1);
            return jvmti::environment(buf);
        }

        static java_vm get_created()
        {
            JavaVM *jvm;
            jsize count{};
            JNI_GetCreatedJavaVMs(&jvm, 1, &count);
            return java_vm(jvm);
        }

    private:
        JavaVM *_jvm;
    };
}