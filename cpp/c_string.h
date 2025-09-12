#ifndef C_STD_STRING_H
#define C_STD_STRING_H

#include "c_string_types.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

__declspec(dllexport) String new_String();
__declspec(dllexport) String new_String_1(const char* str);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif
