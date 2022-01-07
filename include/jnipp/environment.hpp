#pragma once
#include "type.hpp"
#include "field_id.hpp"
#include "method_id.hpp"
#include "string.hpp"
#include "array.hpp"
#include <string_view>
#include <vector>

namespace jni
{
    class environment
    {
    public:
        environment() noexcept
            : _env(nullptr)
        {
        }

        environment(JNIEnv *env) noexcept
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

        operator bool() const noexcept
        {
            return _env != nullptr;
        }

        int get_version() const
        {
            return _env->GetVersion();
        }

        template <reference_type RefType>
        type<RefType> define_class(std::string_view name, const auto &loader, const std::byte *data, std::size_t size) const
        {
            return type<RefType>(_env->DefineClass(name.data(), loader.unsafe_ref(), data, size));
        }

        template <reference_type RefType>
        type<RefType> find_class(std::string_view name) const
        {
            return type<RefType>(_env->FindClass(name.data()));
        }

        method_id from_reflected_method(const auto &method) const
        {
            return method_id(_env->FromReflectedMethod(method.unsafe_ref()));
        }

        field_id from_reflected_field(const auto &field) const
        {
            return field_id(_env->FromReflectedField(field.unsafe_ref()));
        }

        template <reference_type RefType>
        object<RefType> to_reflected_method(const auto &type, const method_id &method_id, bool is_static) const
        {
            return object<RefType>(_env->ToReflectedMethod(type.unsafe_ref(), method_id, is_static));
        }

        template <reference_type RefType>
        type<RefType> get_super_class(const auto &type) const
        {
            return type<RefType>(_env->GetSuperclass(type.unsafe_ref()));
        }

        bool is_assignable_from(const auto &type1, const auto &type2) const
        {
            return _env->IsAssignableFrom(type1.unsafe_ref(), type2.unsafe_ref());
        }

        template <reference_type RefType>
        object<RefType> to_reflected_field(const auto &type, const field_id &field_id, bool is_static) const
        {
            return object<RefType>(_env->ToReflectedField(type.unsafe_ref(), field_id, is_static));
        }

        std::int32_t throw_exception(const auto &exception) const
        {
            return _env->Throw(exception.unsafe_ref());
        }

        std::int32_t throw_new(const auto &type, const std::string_view &message) const
        {
            return _env->ThrowNew(type.unsafe_ref(), message.data());
        }

        template <reference_type RefType>
        object<RefType> exception_occurred() const
        {
            return object<RefType>(_env->ExceptionOccurred());
        }

        void exception_describe() const
        {
            _env->ExceptionDescribe();
        }

        void exception_clear() const
        {
            _env->ExceptionClear();
        }

        void fatal_error(const std::string_view &message) const
        {
            _env->FatalError(message.data());
        }

        std::int32_t push_local_frame(std::int32_t capacity) const
        {
            return _env->PushLocalFrame(capacity);
        }

        template <reference_type RefType>
        object<RefType> pop_local_frame(const auto &result) const
        {
            return object<RefType>(_env->PopLocalFrame(result.unsafe_ref()));
        }

        object<reference_type::global> new_global_ref(const auto &obj) const
        {
            return object<reference_type::global>(_env->NewGlobalRef(obj.unsafe_ref()));
        }

        void delete_global_ref(const object<reference_type::global> &obj) const
        {
            _env->DeleteGlobalRef(obj.unsafe_ref());
        }

        void delete_local_ref(const object<reference_type::local> &obj) const
        {
            _env->DeleteLocalRef(obj.unsafe_ref());
        }

        bool is_same_object(const auto &obj1, const auto &obj2) const
        {
            return _env->IsSameObject(obj1.unsafe_ref(), obj2.unsafe_ref());
        }

        object<reference_type::local> new_local_ref(const auto &obj) const
        {
            return object<reference_type::local>(_env->NewLocalRef(obj.unsafe_ref()));
        }

        std::int32_t ensure_local_capacity(int capacity) const
        {
            return _env->EnsureLocalCapacity(capacity);
        }

        template <reference_type RefType>
        object<RefType> alloc_object(const auto &type) const
        {
            return object<RefType>(_env->AllocObject(type.unsafe_ref()));
        }

        template <reference_type RefType>
        object<RefType> new_object(const auto &type, const method_id &method_id, const auto &...args) const
        {
            static_assert(false, "get_array_elements not implemented");
        }

        template <reference_type RefType>
        type<RefType> get_object_class(const auto &obj) const
        {
            return type<RefType>(_env->GetObjectClass(obj.unsafe_ref()));
        }

        bool is_instance_of(const auto &obj, const auto &type) const
        {
            return _env->IsInstanceOf(obj.unsafe_ref(), type.unsafe_ref());
        }

        method_id get_method_id(const auto &type, std::string_view name, std::string_view signature) const
        {
            return method_id(_env->GetMethodID(type.unsafe_ref(), name.data(), signature.data()));
        }

        template <typename T>
        T call_method(const auto &obj, const method_id &method_id, const auto &...args) const
        {
            static_assert(false, "get_array_elements not implemented");
        }

        template <typename T>
        T call_static_method(const auto &type, const method_id &method_id, const auto &...args) const
        {
            static_assert(false, "get_array_elements not implemented");
        }

        field_id get_field_id(const auto &type, std::string_view name, std::string_view signature) const
        {
            return field_id(_env->GetFieldID(type.unsafe_ref(), name.data(), signature.data()));
        }

        template <typename T>
        T get_field(const auto &obj, const field_id &field_id) const
        {
            static_assert(false, "get_array_elements not implemented");
        }

        template <typename T>
        void set_field(const auto &obj, const field_id &field_id, const T &value) const
        {
            static_assert(false, "get_array_elements not implemented");
        }

        method_id get_static_method_id(const auto &type, const std::string_view &name, const std::string_view &signature) const
        {
            return method_id(_env->GetStaticMethodID(type.unsafe_ref(), name.data(), signature.data()));
        }

        field_id get_static_field_id(const auto &type, const std::string_view &name, const std::string_view &signature) const
        {
            return field_id(_env->GetStaticFieldID(type.unsafe_ref(), name.data(), signature.data()));
        }

        template <typename T>
        T get_static_field(const auto &type, const field_id &field_id) const
        {
            static_assert(false, "get_array_elements not implemented");
        }

        template <typename T>
        void set_static_field(const auto &type, const field_id &field_id, const T &value) const
        {
            static_assert(false, "get_array_elements not implemented");
        }

        template <reference_type RefType>
        string<RefType> new_string(std::string_view utf8) const
        {
            return string<RefType>(_env->NewStringUTF(utf8.data()));
        }

        std::int32_t get_string_length(const auto &str) const
        {
            return _env->GetStringLength(str.unsafe_ref());
        }

        std::string_view get_string_chars(const auto &str, bool &is_copy) const
        {
            const char *chars = _env->GetStringUTFChars(str.unsafe_ref(), &is_copy);
            return std::string_view(chars, _env->GetStringUTFLength(str.unsafe_ref()));
        }

        void release_string_chars(const auto &str, std::string_view chars) const
        {
            _env->ReleaseStringUTFChars(str.unsafe_ref(), chars.data());
        }

        std::int32_t get_array_length(const auto &array) const
        {
            return _env->GetArrayLength(array.unsafe_ref());
        }

        template <typename T, reference_type RefType>
        array<T, RefType> new_array(std::int32_t length) const
        {
            static_assert(false, "get_array_elements not implemented");
        }

        auto *get_array_elements(const auto &array, bool &is_copy) const
        {
            static_assert(false, "get_array_elements not implemented");
        }

        void release_array_elements(const auto &array, auto *elements, std::int32_t mode) const
        {
            static_assert(false, "get_array_elements not implemented");
        }

        auto get_array_region(const auto &array, std::int32_t start, std::int32_t len) const
        {
            static_assert(false, "get_array_elements not implemented");
        }

        auto get_array_element(const auto &array, std::int32_t index) const
        {
            static_assert(false, "get_array_elements not implemented");
        }

        void set_array_region(const auto &array, std::int32_t start, std::int32_t length, const auto *buf) const
        {
            static_assert(false, "get_array_elements not implemented");
        }

        void set_array_element(const auto &array, std::int32_t index, const auto &value) const
        {
            static_assert(false, "get_array_elements not implemented");
        }

    private:
        JNIEnv *_env;
    };
}