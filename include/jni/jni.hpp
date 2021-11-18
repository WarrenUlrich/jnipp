#pragma once
#include "detail/detail.hpp"

namespace jni
{  
    int get_version()
    {
        return detail::get_local_env()->GetVersion();
    }
}