#pragma once
#include <jni.h>

namespace jni
{
    class field_id
    {
    public:
        field_id() noexcept
            : _id(nullptr)
        {
        }

        field_id(jfieldID id) noexcept
            : _id(id)
        {
        }

        field_id(const field_id &other) noexcept
            : _id(other._id)
        {
        }

        field_id(field_id &&other) noexcept
            : _id(std::exchange(other._id, nullptr))
        {
        }

        field_id &operator=(const field_id &other) noexcept
        {
            _id = other._id;
            return *this;
        }

        field_id &operator=(field_id &&other) noexcept
        {
            _id = std::exchange(other._id, nullptr);
            return *this;
        }

        operator bool() const noexcept
        {
            return _id != nullptr;
        }
        
        operator jfieldID() const noexcept
        {
            return _id;
        }

    private:
        jfieldID _id;
    };

}