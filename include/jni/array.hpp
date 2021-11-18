#pragma once
#include "object.hpp"
#include "detail/detail.hpp"
#include <cstddef>

namespace jni
{
    template <typename T, reference_type RefType = reference_type::local>
    class array : public object<RefType>
    {
    public:
        class iterator
        {
        public:

            iterator(std::size_t start_index, const array &arr) : _index(start_index), _array(arr) {}

            T operator*() const
            {
                return _array.at(_index);
            }

            T operator->() const
            {
                return _array.at(_index);
            }

            iterator &operator++()
            {
                _index++;
                return *this;
            }

            friend bool operator==(const iterator &lhs, const iterator &rhs)
            {
                return lhs._index == rhs._index;
            }

            friend bool operator!=(const iterator &lhs, const iterator &rhs)
            {
                return lhs._index != rhs._index;
            }

        private:
            const array<T, RefType> &_array;
            std::size_t _index;
        };

        array(object_ref ref) : object<RefType>(ref) {}

        std::int32_t size() const
        {
            auto res = detail::get_local_env()->GetArrayLength(static_cast<jarray>(this->ref));
            return res;
        }

        T at(std::int32_t index) const
        {
            if constexpr (std::is_base_of<object<local>, T>::value)
            {
                const auto res = detail::get_local_env()->GetObjectArrayElement(this->ref, index);
                return T(res);
            }
            else if constexpr (std::is_base_of<object<global>, T>::value)
            {
                auto res = detail::get_local_env()->GetObjectArrayElement((jobjectArray)this->ref, (jsize)index);
                auto ref = detail::get_local_env()->NewGlobalRef(res);
                return T(ref);
            }
            else if constexpr (std::is_same<T, bool>::value)
            {
                jboolean res = JNI_FALSE;
                detail::get_local_env()->GetBooleanArrayRegion(this->ref, index, 1, &res);
                return static_cast<bool>(res);
            }
            else if constexpr (std::is_same<T, std::byte>::value || std::is_same<T, unsigned char>::value)
            {
                jbyte res = 0;
                detail::get_local_env()->GetByteArrayRegion(this->ref, index, 1, &res);
                return static_cast<std::byte>(res);
            }
            else if constexpr (std::is_same<T, short>::value || std::is_same<T, std::int16_t>::value)
            {
                jshort res = 0;
                detail::get_local_env()->GetShortArrayRegion(this->ref, index, 1, &res);
                return static_cast<short>(res);
            }
            else if constexpr (std::is_same<T, int>::value)
            {
                jint res = 0;
                detail::get_local_env()->GetIntArrayRegion(this->ref, index, 1, &res);
                return static_cast<int>(res);
            }
            else if constexpr (std::is_same<T, std::int64_t>::value)
            {
                jlong res = 0;
                detail::get_local_env()->GetLongArrayRegion(this->ref, index, 1, &res);
                return static_cast<std::int64_t>(res);
            }
            else if constexpr (std::is_same<T, float>::value)
            {
                jfloat res = 0;
                detail::get_local_env()->GetFloatArrayRegion(this->ref, index, 1, &res);
                return static_cast<float>(res);
            }
            else if constexpr (std::is_same<T, double>::value)
            {
                jdouble res = 0;
                detail::get_local_env()->GetDoubleArrayRegion(this->ref, index, 1, &res);
                return static_cast<double>(res);
            }
            else
            {
                static_assert(false, "unsupported jni array type");
            }
            throw new std::runtime_error("unsupported jni array type");
        }

        void set(std::int32_t index, T value) const
        {
            if constexpr (std::is_base_of<object<local>, T>::value)
            {
                detail::get_local_env()->SetObjectArrayElement(this->ref, index, value.ref);
            }
            else if constexpr (std::is_base_of<object<global>, T>::value)
            {
                detail::get_local_env()->SetObjectArrayElement(this->ref, index, value.ref);
            }
            else if constexpr (std::is_same<T, bool>::value)
            {
                detail::get_local_env()->SetBooleanArrayRegion(this->ref, index, 1, &value);
            }
            else if constexpr (std::is_same<T, std::byte>::value)
            {
                detail::get_local_env()->SetByteArrayRegion(this->ref, index, 1, &value);
            }
            else if constexpr (std::is_same<T, short>::value)
            {
                detail::get_local_env()->SetShortArrayRegion(this->ref, index, 1, &value);
            }
            else if constexpr (std::is_same<T, int>::value)
            {
                detail::get_local_env()->SetIntArrayRegion(this->ref, index, 1, &value);
            }
            else if constexpr (std::is_same<T, std::int64_t>::value)
            {
                detail::get_local_env()->SetLongArrayRegion(this->ref, index, 1, &value);
            }
            else if constexpr (std::is_same<T, float>::value)
            {
                detail::get_local_env()->SetFloatArrayRegion(this->ref, index, 1, &value);
            }
            else if constexpr (std::is_same<T, double>::value)
            {
                detail::get_local_env()->SetDoubleArrayRegion(this->ref, index, 1, &value);
            }
            else
            {
                static_assert(false, "unsupported jni array type");
            }
        }

        iterator begin() const
        {
            return iterator(0, *this);
        }

        iterator end() const
        {
            return iterator(size(), *this);
        }

    private:
    };
}