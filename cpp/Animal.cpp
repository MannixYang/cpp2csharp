#include <iostream>
#include <map>
class Animal {
public:
  virtual ~Animal() noexcept = default;

  virtual int weight() const = 0;

  template <typename PropertyType> inline PropertyType &as() {
    return dynamic_cast<PropertyType &>(*this);
  }
};

class Cat : public Animal {
public:
  Cat() = default;
  ~Cat() noexcept override = default;
  int weight() const override { return 5; }
  bool operator==(const Animal &other) const { return this == &other; }
  Animal* copy() const { return new Cat(*this); }
};

extern "C" {

// Cat 构造 & 析构
__declspec(dllexport) Cat *Cat_new() { return new Cat(); }
__declspec(dllexport) void Cat_delete(Cat *c) { delete c; }

// Cat 类接口
__declspec(dllexport) int Cat_weight(Cat *c) { return c->weight(); }
__declspec(dllexport) Animal* Cat_copy(Cat *c) { return c->copy(); }

// Animal 基类接口
__declspec(dllexport) void Animal_delete(Animal* a) { delete a; }
__declspec(dllexport) int Animal_weight(Animal *a) { return a->weight(); }

// 动态类型转换
__declspec(dllexport) Cat *Animal_to_Cat(Animal *a) {
  return dynamic_cast<Cat *>(a);
}

// Cat 比较
__declspec(dllexport) bool Cat_Equals(const Cat *lhs, const Animal *rhs) {
  if (!lhs || !rhs)
    return false;
  return (*lhs == *rhs);
}

}