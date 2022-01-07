#pragma once
#include <utility>
#include <jni.h>

namespace jni
{
    class method_id
    {
    public:
        method_id() noexcept
            : _id(nullptr)
        {
        }

        method_id(jmethodID id) noexcept
            : _id(id)
        {
        }

        method_id(const method_id &other) noexcept
            : _id(other._id)
        {
        }

        method_id(method_id &&other) noexcept
        : _id(std::exchange(other._id, nullptr))
        {
        }

        method_id &operator=(const method_id &other) noexcept
        {
            _id = other._id;
            return *this;
        }

        method_id &operator=(method_id &&other) noexcept
        {
            _id = std::exchange(other._id, nullptr);
            return *this;
        }

        operator bool () const noexcept
        {
            return _id != nullptr;
        }
        
        operator jmethodID() const noexcept
        {
            return _id;
        }

    private:
        jmethodID _id;
    };
}