#include <string>
#include <utility>

#include "c_string.h"

String new_String() {
  String c_string;
  c_string.ptr = reinterpret_cast<void *>(new std::string);
  c_string.mem_owner = true;
  return c_string;
}

String new_String_1(const char* str) {
  String c_string;
  c_string.ptr = reinterpret_cast<void *>(new std::string(str));
  c_string.mem_owner = true;
  return c_string;
}