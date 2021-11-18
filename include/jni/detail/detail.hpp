#pragma once
#include <jni.h>

namespace jni::detail
{
    JavaVM* java_vm;

    // TODO: Wrap and destruct the JNIEnv.
    thread_local JNIEnv* local_jni_env;

    JavaVM* get_java_vm()
    {
        if(java_vm == nullptr)
        {
            //TODO: Error check here.
            jsize count{};
            JNI_GetCreatedJavaVMs(&java_vm, 1, &count);
        }
        
        return java_vm;
    }

    JNIEnv* get_local_env()
    {
        if(local_jni_env != nullptr)
            return local_jni_env;

        const auto vm = get_java_vm();

        JNIEnv* temp;
        auto res = vm->GetEnv((void**)&temp, JNI_VERSION_1_1);
        if(res == JNI_OK)
        {
            local_jni_env = temp;
            return local_jni_env;
        }
        else if(res == JNI_EDETACHED)
        {
            // TODO: error check here
            vm->AttachCurrentThread((void**)&temp, nullptr);
            local_jni_env = temp;
            return local_jni_env;
        }

        return nullptr;
    }
}