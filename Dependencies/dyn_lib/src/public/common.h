#pragma once

#ifdef SOURCE_EXPORT_SHARED_LIB
    #define MY_API __declspec(dllexport)
#else
    #define MY_API __declspec(dllimport)
#endif
