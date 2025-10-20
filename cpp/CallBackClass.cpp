#include <iostream>

// native C++

class ICallBack {
public:
  virtual void print() = 0;
  virtual ~ICallBack() {}
};

void call_print(ICallBack *cb) { cb->print(); }

// native c
void N_ICallBack_print(void *self) { static_cast<ICallBack *>(self)->print(); }
void N_call_print(void *cb) { call_print(static_cast<ICallBack *>(cb)); }

// csharp c wrapper
extern "C" {
__declspec(dllexport) void CS_ICallBack_print(void *self) {
  // 字符串编码转换等工作,，虽然这个接口不需要，但这一层不可避免
  N_ICallBack_print(self);
}
__declspec(dllexport) void CS_call_print(void *cb) { N_call_print(cb); }
}

// Native C
class C_ICallBack_Middle : public ICallBack  {
public:
  void *self;
  void (*_print)(void *);
  inline void print() override {
    return _print(self);
  };
};

void *N_new_i_call_back(void *self, void (*print)(void *)) {
  auto *ptr = new C_ICallBack_Middle();
  ptr->self = self;
  ptr->_print = print;
  return ptr;
}

// CSharp C Wrapper
extern "C" {
__declspec(dllexport) void *CS_new_i_call_back(void *self, void (*print)(void *)) {
  return N_new_i_call_back(self, print);
}
}