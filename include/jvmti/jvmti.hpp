#pragma once
#include "../jni/type.hpp"
#include "detail/detail.hpp"
#include <vector>
#include <utility>
#include <format>

namespace jvmti
{
    std::map<std::string_view, jni::type<>> loaded_type_cache = std::map<std::string_view, jni::type<>>();

    std::vector<jni::type<>> get_loaded_types()
    {
        jint count = 0;
        jclass *buf;
        //TODO: error check here
        detail::get_environment()->GetLoadedClasses(&count, &buf);

        std::vector<jni::type<>> result{};
        result.reserve(count);
        const auto jni_env = jni::detail::get_local_env();
        for (int i = 0; i < count; i++)
        {
            auto cls = buf[i];
            if (cls != nullptr)
            {
                const auto global_cls = jni_env->NewGlobalRef(cls);
                jni_env->DeleteLocalRef(cls);
                result.emplace_back(jni::type<>((jclass)global_cls));
            }
        }
        detail::get_environment()->Deallocate(reinterpret_cast<unsigned char *>(buf));
        return result;
    }

    template<jni::reference_type RefType>
    std::pair<std::string, std::string> get_type_signature(const jni::type<RefType> &type)
    {
        char *sig;
        char *generic;

        const auto env = detail::get_environment();

        env->GetClassSignature(static_cast<jclass>(type.ref), &sig, &generic);

        std::pair<std::string, std::string> result{};

        if (sig)
        {
            result.first = sig;
            env->Deallocate((unsigned char *)sig);
            if (generic)
            {
                result.second = generic;
                env->Deallocate((unsigned char *)generic);
            }
        }

        return result;
    }


    const jni::type<> &get_loaded_type(std::string_view sig)
    {
        if (loaded_type_cache.count(sig) > 0)
            return loaded_type_cache.at(sig);

        const auto types = get_loaded_types();

        for (const auto &t : types)
        {
            const auto type_sig = get_type_signature(t);
            if (type_sig.first == sig)
            {
                loaded_type_cache.insert(std::pair{sig, jni::type((jni::type<> &&) std::move(t))});
                return loaded_type_cache.at(sig);
            }
        }
        
        //TODO: throw custom exception
        throw std::runtime_error(std::format("couldn't find loaded jni type {}", sig));
    }
}