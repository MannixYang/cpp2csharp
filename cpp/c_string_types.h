#ifndef C_STRING_TYPES_H
#define C_STRING_TYPES_H


typedef struct __declspec(dllexport) String {
  void *ptr;
  bool mem_owner;
} String;

#endif
