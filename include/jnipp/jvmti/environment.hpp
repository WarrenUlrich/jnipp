#pragma once
#include <vector>
#include <jvmti.h>
#include "../environment.hpp"

namespace jni::jvmti
{
    class environment
    {
    public:
        environment() noexcept
            : _env(nullptr)
        {
        }

        environment(jvmtiEnv *env) noexcept
            : _env(env)
        {
        }

        environment(environment &&other) noexcept
            : _env(std::exchange(other._env, nullptr))
        {
        }

        environment &operator=(environment &&other) noexcept
        {
            _env = std::exchange(other._env, nullptr);
            return *this;
        }

        std::vector<jni::type<>> get_loaded_classes(const jni::environment& jni_env) const
        {
            std::vector<jni::type<>> result;
            jint count;
            jclass *buf;
            _env->GetLoadedClasses(&count, &buf);
            for (jint i = 0; i < count; ++i)
            {
                auto temp = jni::object<jni::reference_type::local>(buf[i]);
                result.emplace_back(jni_env.new_global_ref(temp));
            }
            return result;
        }

        operator bool() const noexcept
        {
            return _env != nullptr;
        }

    private:
        jvmtiEnv *_env;
    };
}