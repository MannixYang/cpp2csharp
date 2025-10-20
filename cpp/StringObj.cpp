#include "c_string_types.h"
#include <cwchar>
#include <string>
#include <windows.h>

#include "c_string.h"
#include <iostream>

class StringObj {
private:
  std::string s;

public:
  StringObj() = default;
  void set(std::string ss) {
    s = ss;
    std::cout << "c++ StringObj set: " << s << std::endl;
  }
  std::string get() { return s; }
  ~StringObj() = default;
};

static std::string utf16_to_utf8(const wchar_t *str) {
  const auto utf8_size =
      WideCharToMultiByte(CP_UTF8, 0, str, -1, nullptr, 0, nullptr, nullptr);
  std::string utf8_str(utf8_size - 1, 0); // 不包含终止 null 字符.
  WideCharToMultiByte(CP_UTF8, 0, str, -1, &utf8_str[0], utf8_size, nullptr,
                      nullptr);

  return utf8_str;
}

static std::wstring utf8_to_utf16(const char *str) {
  const auto utf16_size = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
  std::wstring utf16_str(utf16_size - 1, 0); // 不包含终止 null 字符.
  MultiByteToWideChar(CP_UTF8, 0, str, -1, &utf16_str[0], utf16_size);

  return utf16_str;
}

extern "C" {

typedef struct __declspec(dllexport) C_StringObj {
  void *ptr;
  bool mem_owner;
} C_StringObj;

// C ABI
__declspec(dllexport) C_StringObj new_string_obj() {
  return {new StringObj(), true};
}

__declspec(dllexport) void string_obj_set(C_StringObj self, const String ss) {
  reinterpret_cast<StringObj *>(self.ptr)->set(
      *reinterpret_cast<const std::basic_string<char> *>(ss.ptr));
}

__declspec(dllexport) String string_obj_get(C_StringObj self) {
  return {new std::basic_string<char>(
              reinterpret_cast<StringObj *>(self.ptr)->get()),
          true};
}

__declspec(dllexport) void string_obj_delete(C_StringObj self) {
  if (self.mem_owner) {
    delete reinterpret_cast<StringObj *>(self.ptr);
  }
  self.mem_owner = false;
  self.ptr = nullptr;
}

// StringHelper
typedef char *(*CSharpStringHelperCallback)(const void *);
static CSharpStringHelperCallback csharp_string_callback = NULL;

__declspec(dllexport) void RegisterStringCallback(
    CSharpStringHelperCallback callback) {
  csharp_string_callback = callback;
}

// C ABI wrapper
__declspec(dllexport) void *cmid_new_string_obj() {
  return new_string_obj().ptr;
}

__declspec(dllexport) void cmid_string_obj_set(void *self_ptr,
                                               bool self_mem_owner,
                                               const char *jarg1) {
  std::string utf8_jarg1 =
      utf16_to_utf8(reinterpret_cast<const wchar_t *>(jarg1));
  C_StringObj self = {self_ptr, self_mem_owner};
  string_obj_set(self, new_String_1(utf8_jarg1.c_str()));
}

__declspec(dllexport) const
    char *cmid_string_obj_get(void *self_ptr, bool self_mem_owner) {
  C_StringObj self = {self_ptr, self_mem_owner};
  String res = string_obj_get(self);

  std::string utf8_res = *reinterpret_cast<std::basic_string<char> *>(res.ptr);
  std::wstring utf16_res = utf8_to_utf16(utf8_res.c_str());

  const char *jresult = csharp_string_callback(utf16_res.c_str());
  return jresult;
}

__declspec(dllexport) void cmid_string_obj_delete(void *self_ptr,
                                                  bool self_mem_owner) {
  C_StringObj self = {self_ptr, self_mem_owner};
  string_obj_delete(self);
}
} // extern "C"