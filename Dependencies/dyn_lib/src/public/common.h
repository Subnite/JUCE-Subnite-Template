#pragma once

#ifdef _WIN32
    #ifdef SOURCE_EXPORT_SHARED_LIB
        #define MY_API __declspec(dllexport)
    #else
        #define MY_API __declspec(dllimport)
    #endif
#elif __GNUC__ >= 4
    #define MY_API __attribute__((visibility("default")))
#endif
