#pragma once
#include "object.hpp"
#include <map>
#include <string_view>
#include <memory>

namespace jni
{

    using class_ref = jclass;

    template <reference_type RefType1 = reference_type::global>
    class type : public object<RefType1>
    {
    public:
        type(class_ref cls) : object<RefType1>(cls)
        {
        }

        type(type &other) = delete;

        type(type &&type) : object<RefType1>(std::move(type))
        {
        }

        template <class T, reference_type RefType2>
        T get_field(const object<RefType2> &obj, std::string_view name, std::string_view sig) const
        {
            const auto field_id = this->get_field_id(name, sig);

            if constexpr (std::is_base_of<object<local>, T>::value)
            {
                auto local_obj = detail::get_local_env()->GetObjectField(obj.ref, field_id);
                return T(local_obj);
            }
            else if constexpr (std::is_base_of<object<global>, T>::value)
            {
                auto local_obj = detail::get_local_env()->GetObjectField(obj.ref, field_id);
                auto global_obj = detail::get_local_env()->NewGlobalRef(local_obj);
                detail::get_local_env()->DeleteLocalRef(local_obj);
                return T(global_obj);
            }
            else if constexpr (std::is_same<T, bool>::value)
            {
                return detail::get_local_env()->GetBooleanField(obj.ref, field_id) == JNI_TRUE;
            }
            else if constexpr (std::is_same<T, std::byte>::value || std::is_same<T, unsigned char>::value)
            {
                return static_cast<std::byte>(detail::get_local_env()->GetByteField(obj.ref, field_id));
            }
            else if constexpr (std::is_same<T, short>::value || std::is_same<T, std::int16_t>::value)
            {
                return detail::get_local_env()->GetShortField(obj.ref, field_id);
            }
            else if constexpr (std::is_same<T, int>::value)
            {
                return detail::get_local_env()->GetIntField(obj.ref, field_id);
            }
            else if constexpr (std::is_same<T, std::int64_t>::value)
            {
                return detail::get_local_env()->GetLongField(obj.ref, field_id);
            }
            else if constexpr (std::is_same<T, float>::value)
            {
                return detail::get_local_env()->GetFloatField(obj.ref, field_id);
            }
            else if constexpr (std::is_same<T, double>::value)
            {
                return detail::get_local_env()->GetDoubleField(obj.ref, field_id);
            }
            else
            {
                static_assert(false, "unsupported template type in get_field");
            }

            throw std::runtime_error("unsupported template type in get_field");
        }

        template <class T>
        T get_static_field(std::string_view name) const
        {
            if constexpr (std::is_base_of<object<local>, T>::value)
            {
                static_assert(false, "a jni class signature must be provided to get_static_field if the signature is not deducible");
            }
            else if constexpr (std::is_base_of<object<global>, T>::value) //(std::<T, object<global>>::value)
            {
                static_assert(false, "a jni class signature must be provided to get_static_field if the signature is not deducible");
            }
            else if constexpr (std::is_same<T, bool>::value)
            {
                const auto field_id = this->get_static_field_id(name, "Z");
                return detail::get_local_env()->GetStaticBooleanField(static_cast<jclass>(this->ref), field_id) == JNI_TRUE;
            }
            else if constexpr (std::is_same<T, std::byte>::value || std::is_same<T, unsigned char>::value)
            {
                const auto field_id = this->get_static_field_id(name, "B");
                return static_cast<std::byte>(detail::get_local_env()->GetStaticByteField(static_cast<jclass>(this->ref), field_id));
            }
            else if constexpr (std::is_same<T, short>::value || std::is_same<T, std::int16_t>::value)
            {
                const auto field_id = this->get_static_field_id(name, "S");
                return detail::get_local_env()->GetStaticShortField(static_cast<jclass>(this->ref), field_id);
            }
            else if constexpr (std::is_same<T, int>::value)
            {
                const auto field_id = this->get_static_field_id(name, "I");
                return detail::get_local_env()->GetStaticIntField(static_cast<jclass>(this->ref), field_id);
            }
            else if constexpr (std::is_same<T, std::int64_t>::value)
            {
                const auto field_id = this->get_static_field_id(name, "J");
                return detail::get_local_env()->GetStaticLongField(static_cast<jclass>(this->ref), field_id);
            }
            else if constexpr (std::is_same<T, float>::value)
            {
                const auto field_id = this->get_static_field_id(name, "F");
                return detail::get_local_env()->GetStaticFloatField(static_cast<jclass>(this->ref), field_id);
            }
            else if constexpr (std::is_same<T, double>::value)
            {
                const auto field_id = this->get_static_field_id(name, "D");
                return detail::get_local_env()->GetStaticDoubleField(static_cast<jclass>(this->ref), field_id);
            }
            else
            {
                static_assert(false, "unsupported template type in get_static_field");
            }

            throw std::runtime_error("unsupported template type in get_static_field");
        }

        template <class T>
        T get_static_field(std::string_view name, std::string_view sig) const
        {
            const auto field_id = this->get_static_field_id(name, sig);
            if constexpr (std::is_base_of<object<local>, T>::value)
            {
                auto local_obj = detail::get_local_env()->GetStaticObjectField(static_cast<jclass>(this->ref), field_id);
                return T(local_obj);
            }
            else if constexpr (std::is_base_of<object<global>, T>::value)
            {
                auto local_obj = detail::get_local_env()->GetStaticObjectField(static_cast<jclass>(this->ref), field_id);
                auto global_obj = detail::get_local_env()->NewGlobalRef(local_obj);
                detail::get_local_env()->DeleteLocalRef(local_obj);
                return T(global_obj);
            }
            else if constexpr (std::is_same<T, bool>::value)
            {
                return detail::get_local_env()->GetStaticBooleanField(static_cast<jclass>(this->ref), field_id) == JNI_TRUE;
            }
            else if constexpr (std::is_same<T, std::byte>::value || std::is_same<T, unsigned char>::value)
            {
                return static_cast<std::byte>(detail::get_local_env()->GetStaticByteField(static_cast<jclass>(this->ref), field_id));
            }
            else if constexpr (std::is_same<T, short>::value || std::is_same<T, std::int16_t>::value)
            {
                return detail::get_local_env()->GetStaticShortField(static_cast<jclass>(this->ref), field_id);
            }
            else if constexpr (std::is_same<T, int>::value)
            {
                return detail::get_local_env()->GetStaticIntField(static_cast<jclass>(this->ref), field_id);
            }
            else if constexpr (std::is_same<T, std::int64_t>::value)
            {
                return detail::get_local_env()->GetStaticLongField(static_cast<jclass>(this->ref), field_id);
            }
            else if constexpr (std::is_same<T, float>::value)
            {
                return detail::get_local_env()->GetStaticFloatField(static_cast<jclass>(this->ref), field_id);
            }
            else if constexpr (std::is_same<T, double>::value)
            {
                return detail::get_local_env()->GetStaticDoubleField(static_cast<jclass>(this->ref), field_id);
            }
            else
            {
                static_assert(false, "unsupported template type in get_static_field");
            }

            throw std::runtime_error("unsupported template type in get_static_field");
        }

        template <class T, reference_type RefType2, class... Args>
        T call_method(const object<RefType2> &obj, std::string_view name, std::string_view sig, Args... args) const
        {
            const auto method_id = this->get_method_id(name, sig);
            if constexpr (std::is_base_of<object<local>, T>::value)
            {
                auto local_obj = detail::get_local_env()->CallObjectMethodA(obj.ref, method_id, util::args_to_jvalues(args...));
                return T(local_obj);
            }
            else if constexpr (std::is_base_of<object<global>, T>::value)
            {
                auto local_obj = detail::get_local_env()->CallObjectMethodA(obj.ref, method_id, util::args_to_jvalues(args...));
                auto global_obj = detail::get_local_env()->NewGlobalRef(local_obj);
                detail::get_local_env()->DeleteLocalRef(local_obj);
                return T(global_obj);
            }
            else if constexpr (std::is_same<T, bool>::value)
            {
                return detail::get_local_env()->CallBooleanMethodA(obj.obj, method_id, util::args_to_jvalues(args...)) == JNI_TRUE;
            }
            else if constexpr (std::is_same<T, std::byte>::value || std::is_same<T, unsigned char>::value)
            {
                return static_cast<std::byte>(detail::get_local_env()->CallByteMethodA(obj.obj, method_id, util::args_to_jvalues(args...)));
            }
            else if constexpr (std::is_same<T, short>::value || std::is_same<T, std::int16_t>::value)
            {
                return detail::get_local_env()->CallShortMethodA(obj.ref, method_id, util::args_to_jvalues(args...));
            }
            else if constexpr (std::is_same<T, int>::value)
            {
                return detail::get_local_env()->CallIntMethodA(obj.ref, method_id, util::args_to_jvalues(args...));
            }
            else if constexpr (std::is_same<T, std::int64_t>::value)
            {
                return detail::get_local_env()->CallLongMethodA(obj.ref, method_id, util::args_to_jvalues(args...));
            }
            else if constexpr (std::is_same<T, float>::value)
            {
                return detail::get_local_env()->CallFloatMethodA(obj.ref, method_id, util::args_to_jvalues(args...));
            }
            else if constexpr (std::is_same<T, double>::value)
            {
                return detail::get_local_env()->CallDoubleMethodA(obj.ref, method_id, util::args_to_jvalues(args...));
            }
            else
            {
                []<bool flag = false>() { static_assert(flag, "unsupported template type in call_method"); }
                ();
            }
        }

        template <class T, class... Args>
        T call_static_method(std::string_view name, std::string_view sig, Args... args) const
        {
            const auto method_id = this->get_method_id(name, sig);
            if constexpr (std::derived_from<T, object<local>>::value)
            {
                auto local_obj = detail::get_local_env()->CallStaticObjectMethodA(static_cast<jclass>(this->ref), method_id, util::args_to_jvalues(args...));
                return T<local>(local_obj);
            }
            else if constexpr (std::derived_from<T, object<global>>::value)
            {
                auto local_obj = detail::get_local_env()->CallStaticObjectMethodA(static_cast<jclass>(this->ref), method_id, util::args_to_jvalues(args...));
                auto global_obj = detail::get_local_env()->NewGlobalRef(local_obj);
                detail::get_local_env()->DeleteLocalRef(local_obj);
                return T<global>(global_obj);
            }
            else if constexpr (std::is_same<T, bool>::value)
            {
                return detail::get_local_env()->CallStaticBooleanMethodA(static_cast<jclass>(this->ref), method_id, util::args_to_jvalues(args...)) == JNI_TRUE;
            }
            else if constexpr (std::is_same<T, std::byte>::value || std::is_same<T, unsigned char>::value)
            {
                return static_cast<std::byte>(detail::get_local_env()->CallStaticByteMethodA(static_cast<jclass>(this->ref), method_id, util::args_to_jvalues(args...)));
            }
            else if constexpr (std::is_same<T, short>::value || std::is_same<T, std::int16_t>::value)
            {
                return detail::get_local_env()->CallStaticShortMethodA(static_cast<jclass>(this->ref), method_id, util::args_to_jvalues(args...));
            }
            else if constexpr (std::is_same<T, int>::value)
            {
                return detail::get_local_env()->CallStaticIntMethodA(static_cast<jclass>(this->ref), method_id, util::args_to_jvalues(args...));
            }
            else if constexpr (std::is_same<T, std::int64_t>::value)
            {
                return detail::get_local_env()->CallStaticLongMethodA(static_cast<jclass>(this->ref), method_id, util::args_to_jvalues(args...));
            }
            else if constexpr (std::is_same<T, float>::value)
            {
                return detail::get_local_env()->CallStaticFloatMethodA(static_cast<jclass>(this->ref), method_id, util::args_to_jvalues(args...));
            }
            else if constexpr (std::is_same<T, double>::value)
            {
                return detail::get_local_env()->CallStaticDoubleMethodA(static_cast<jclass>(this->ref), method_id, util::args_to_jvalues(args...));
            }
            else
            {
                []<bool flag = false>() { static_assert(flag, "unsupported template type in call_static_method"); }
                ();
            }
        }

        static std::shared_ptr<type<>> find(std::string_view name)
        {
            if (_type_cache.count(name) > 0)
                return _type_cache.at(name);

            const auto env = detail::get_local_env();
            const auto local_cls = env->FindClass(name.data());
            const auto global_cls = env->NewGlobalRef(local_cls);
            env->DeleteLocalRef(local_cls);

            const std::shared_ptr<type<>> result(new type<>(static_cast<jclass>(global_cls)));

            _type_cache.insert(std::pair{name, result});
            return result;
        }

    private:
        inline static std::map<std::string_view, std::shared_ptr<type<>>> _type_cache = std::map<std::string_view, std::shared_ptr<type<>>>();

        mutable std::map<std::pair<std::string_view, std::string_view>, jfieldID> _field_cache = std::map<std::pair<std::string_view, std::string_view>, jfieldID>();
        mutable std::map<std::pair<std::string_view, std::string_view>, jmethodID> _method_cache = std::map<std::pair<std::string_view, std::string_view>, jmethodID>();

        jfieldID get_field_id(std::string_view name, std::string_view sig) const
        {
            auto key = std::pair{name, sig};
            if (this->_field_cache.count(key) > 0)
                return this->_field_cache.at(key);

            const auto id = detail::get_local_env()->GetFieldID(static_cast<jclass>(this->ref), name.data(), sig.data());
            if(id == nullptr)
                throw new std::runtime_error("jfield not found");
            
            this->_field_cache.insert(std::pair{key, id});
            return id;
        }

        jfieldID get_static_field_id(std::string_view name, std::string_view sig) const
        {
            const auto key = std::pair{name, sig};
            if (this->_field_cache.count(key) > 0)
                return this->_field_cache.at(key);

            const auto id = detail::get_local_env()->GetStaticFieldID(static_cast<jclass>(this->ref), name.data(), sig.data());
            this->_field_cache.insert(std::pair{key, id});
            return id;
        }

        jmethodID get_method_id(std::string_view name, std::string_view sig) const
        {
            const auto key = std::pair{name, sig};
            if (this->_method_cache.count(key) > 0)
                return this->_method_cache.at(key);

            const auto id = detail::get_local_env()->GetMethodID(static_cast<jclass>(this->ref), name.data(), sig.data());
            this->_method_cache.insert(std::pair{key, id});
            return id;
        }

        jmethodID get_static_method_id(std::string_view name, std::string_view sig) const
        {
            const auto key = std::pair{name, sig};
            if (this->_method_cache.count(key) > 0)
                return this->_method_cache.at(key);

            const auto id = detail::get_local_env()->GetStaticMethodID(static_cast<jclass>(this->ref), name.data(), sig.data());
            this->_method_cache.insert(std::pair{key, id});
            return id;
        }
    };

    //std::map<std::string_view, std::shared_ptr<type>> type::_type_cache = std::map<std::string_view, std::shared_ptr<type>>();
}