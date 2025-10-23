#include <memory>
class BigInt{
public:
    BigInt(int n):n(n){};
    int n;
    static BigInt* CreateRaw(int n){
        return new BigInt(n);
    }
    static std::shared_ptr<BigInt> CreateShared(int n){
        return std::make_shared<BigInt>(n);
    }
    static std::unique_ptr<BigInt> CreateUnique(int n){
        return std::make_unique<BigInt>(n);
    }
/**
 * 创建一个BigInt引用
 * @param n 初始化的整数值
 * @return BigInt对象的引用
 */
    static BigInt& CreateRef(int n){
        return *CreateRaw(n);  // 通过CreateRaw创建原始指针并返回其引用
    }
    static BigInt Create(int n){
        return BigInt(n);
    }
};

BigInt my_add_copy(BigInt a, BigInt b){
    return BigInt(a.n + b.n);
}

BigInt& my_add_ref(BigInt &a, BigInt b){
    a.n = a.n + b.n;
    return a;
}

BigInt* my_add_ptr(BigInt* a, BigInt b){
    a->n = a->n + b.n;
    return a;
}

std::shared_ptr<BigInt> my_add_shared(std::shared_ptr<BigInt> a,  BigInt b){
    a->n = a->n + b.n;
    return a;
}

std::unique_ptr<BigInt> my_add_unique(std::unique_ptr<BigInt> a, BigInt b){
    a->n = a->n + b.n;
    return a;
}


extern "C" {

// ------------------- BigInt 工厂函数 -------------------
void* N_BigInt_CreateRaw(int n);
void* N_BigInt_CreateShared(int n);
void* N_BigInt_CreateUnique(int n);
void* N_BigInt_CreateRef(int n);
void* N_BigInt_Create(int n);

// ------------------- my_add 重载 -------------------
void* N_BigInt_my_add_copy(void* a, void* b);
void* N_BigInt_my_add_ref(void* a, void* b);
void* N_BigInt_my_add_ptr(void* a, void* b);
void* N_BigInt_my_add_shared(void* a, void* b);
void* N_BigInt_my_add_unique(void* a, void* b);

// ------------------- 释放函数 -------------------
void N_BigInt_Delete(void* obj);
void N_BigInt_shared_Delete(void* obj);
void N_BigInt_unique_Delete(void* obj);

}


// =========================================================
// 实现部分
// =========================================================
#include <memory>

// ---------- 工厂函数 ----------
void* N_BigInt_CreateRaw(int n) {
    return BigInt::CreateRaw(n);
}

void* N_BigInt_CreateShared(int n) {
    return new std::shared_ptr<BigInt>(BigInt::CreateShared(n));
}

void* N_BigInt_CreateUnique(int n) {
    return new std::unique_ptr<BigInt>(BigInt::CreateUnique(n));
}

void* N_BigInt_CreateRef(int n) {
    return &BigInt::CreateRef(n);
}

void* N_BigInt_Create(int n) {
    return new BigInt(BigInt::Create(n));
}

// ---------- my_add 重载 ----------
void* N_BigInt_my_add_copy(void* a, void* b) {
    BigInt* lhs = reinterpret_cast<BigInt*>(a);
    BigInt* rhs = reinterpret_cast<BigInt*>(b);
    BigInt result = my_add_copy(*lhs, *rhs);
    return new BigInt(result);
}

void* N_BigInt_my_add_ref(void* a, void* b) {
    BigInt* lhs = reinterpret_cast<BigInt*>(a);
    BigInt* rhs = reinterpret_cast<BigInt*>(b);
    BigInt& result = my_add_ref(*lhs, *rhs);
    return &result;
}

void* N_BigInt_my_add_ptr(void* a, void* b) {
    BigInt* lhs = reinterpret_cast<BigInt*>(a);
    BigInt* rhs = reinterpret_cast<BigInt*>(b);
    BigInt* result = my_add_ptr(lhs, *rhs);
    return result;
}

void* N_BigInt_my_add_shared(void* a, void* b) {
    auto lhs = reinterpret_cast<std::shared_ptr<BigInt>*>(a);
    BigInt* rhs = reinterpret_cast<BigInt*>(b);
    auto result = my_add_shared(*lhs, *rhs);
    return new std::shared_ptr<BigInt>(result);
}

void* N_BigInt_my_add_unique(void* a, void* b) {
    auto lhs = reinterpret_cast<std::unique_ptr<BigInt>*>(a);
    BigInt* rhs = reinterpret_cast<BigInt*>(b);
    auto result = my_add_unique(std::move(*lhs), *rhs);
    return new std::unique_ptr<BigInt>(std::move(result));
}

// ---------- 释放函数 ----------
void N_BigInt_Delete(void* obj) {
    delete reinterpret_cast<BigInt*>(obj);
}

void N_BigInt_shared_Delete(void* obj) {
    delete reinterpret_cast<std::shared_ptr<BigInt>*>(obj);
}

void N_BigInt_unique_Delete(void* obj) {
    delete reinterpret_cast<std::unique_ptr<BigInt>*>(obj);
}

// ========================== my_add CSharp C Wrapper ===============================
extern "C" {
enum CSHandleType{
    CSHandleTypeRaw,
    CSHandleTypeShared,
    CSHandleTypeUnique,
};
struct CS_BigInt{
    void* self;
    CSHandleType type;
};
void* CS_BigInt_my_add_copy(void* a, void* b) {
    CS_BigInt * arg_a = reinterpret_cast<CS_BigInt*>(a);
    CS_BigInt * arg_b = reinterpret_cast<CS_BigInt*>(b);
    CS_BigInt * result = nullptr;

    void * a_raw =  nullptr;
    if (arg_a->type == CSHandleTypeRaw)
        a_raw = arg_a->self;
    else if (arg_a->type == CSHandleTypeShared)
        a_raw = reinterpret_cast<std::shared_ptr<BigInt>*>(arg_a->self)->get();
    else if (arg_a->type == CSHandleTypeUnique)
        a_raw = reinterpret_cast<std::unique_ptr<BigInt>*>(arg_a->self)->get();

    void * b_raw =  nullptr;
    if (arg_b->type == CSHandleTypeRaw)
        b_raw = arg_b->self;
    else if (arg_b->type == CSHandleTypeShared)
        b_raw = reinterpret_cast<std::shared_ptr<BigInt>*>(arg_b->self)->get();
    else if (arg_b->type == CSHandleTypeUnique)
        b_raw = reinterpret_cast<std::unique_ptr<BigInt>*>(arg_b->self)->get();

    void * res_self = N_BigInt_my_add_copy(a_raw, b_raw);
    result = new CS_BigInt{res_self, CSHandleTypeRaw};
    return result;
}

void* CS_BigInt_my_add_ref(void* a, CSHandleType a_type, void* b, CSHandleType b_type) {
    void * a_raw =  nullptr;
    if (a_type == CSHandleTypeRaw)
        a_raw = a;
    else if (a_type == CSHandleTypeShared)
        a_raw = reinterpret_cast<std::shared_ptr<BigInt>*>(a)->get();
    else if (a_type == CSHandleTypeUnique)
        a_raw = reinterpret_cast<std::unique_ptr<BigInt>*>(a)->get();

    void * b_raw =  nullptr;
    if (b_type == CSHandleTypeRaw)
        b_raw = b;
    else if (b_type == CSHandleTypeShared)
        b_raw = reinterpret_cast<std::shared_ptr<BigInt>*>(b)->get();
    else if (b_type == CSHandleTypeUnique)
        b_raw = reinterpret_cast<std::unique_ptr<BigInt>*>(b)->get();

    return N_BigInt_my_add_ref(a_raw, b_raw);
}

void* CS_BigInt_my_add_ptr(void* a, CSHandleType a_type, void* b, CSHandleType b_type) {
    void * a_raw =  nullptr;
    if (a_type == CSHandleTypeRaw)
        a_raw = a;
    else if (a_type == CSHandleTypeShared)
        a_raw = reinterpret_cast<std::shared_ptr<BigInt>*>(a)->get();
    else if (a_type == CSHandleTypeUnique)
        a_raw = reinterpret_cast<std::unique_ptr<BigInt>*>(a)->get();

    void * b_raw =  nullptr;
    if (b_type == CSHandleTypeRaw)
        b_raw = b;
    else if (b_type == CSHandleTypeShared)
        b_raw = reinterpret_cast<std::shared_ptr<BigInt>*>(b)->get();
    else if (b_type == CSHandleTypeUnique)
        b_raw = reinterpret_cast<std::unique_ptr<BigInt>*>(b)->get();

    return N_BigInt_my_add_ptr(a_raw, b_raw);
}

// void* CS_BigInt_my_add_shared(void* a, CSHandleType a_type, void* b, CSHandleType b_type) {
//     void * a_shared =  nullptr;
//     if (a_type == CSHandleTypeRaw)
//         a_shared = new std::shared_ptr<BigInt>(reinterpret_cast<BigInt*>(a));
//     else if (a_type == CSHandleTypeShared)
//         a_shared = reinterpret_cast<std::shared_ptr<BigInt>*>(a);
//     else if (a_type == CSHandleTypeUnique)
//         a_shared = new std::shared_ptr<BigInt>(reinterpret_cast<std::unique_ptr<BigInt>*>(a)->release());

//     void * b_raw =  nullptr;
//     if (b_type == CSHandleTypeRaw)
//         b_raw = b;
//     else if (b_type == CSHandleTypeShared)
//         b_raw = reinterpret_cast<std::shared_ptr<BigInt>*>(b)->get();
//     else if (b_type == CSHandleTypeUnique)
//         b_raw = reinterpret_cast<std::unique_ptr<BigInt>*>(b)->get();

//     auto result = N_BigInt_my_add_shared(a_shared, b_raw);
//     return new std::shared_ptr<BigInt>(*reinterpret_cast<std::shared_ptr<BigInt>*>(result));
// }

void* CS_BigInt_my_add_shared(void* a, void* b) {
    auto arg_a = reinterpret_cast<CS_BigInt*>(a);
    auto arg_b = reinterpret_cast<CS_BigInt*>(b);

    // 处理 a 参数
    std::shared_ptr<BigInt> a_shared;
    auto deleter = [](BigInt*){};

    if (arg_a->type == CSHandleTypeRaw) 
        // raw -> shared
        a_shared = std::shared_ptr<BigInt>(reinterpret_cast<BigInt*>(arg_a->self), deleter);
    else if (arg_a->type == CSHandleTypeShared) 
        // shared -> shared
        a_shared = *reinterpret_cast<std::shared_ptr<BigInt>*>(arg_a->self);
    else if (arg_a->type == CSHandleTypeUnique) 
        // unique -> shared
        a_shared = std::shared_ptr<BigInt>(reinterpret_cast<std::unique_ptr<BigInt>*>(arg_a->self)->get(), deleter);

    // 处理 b 参数
    BigInt* b_raw = nullptr;
    if (arg_b->type == CSHandleTypeRaw)
        // raw -> raw
        b_raw = reinterpret_cast<BigInt*>(arg_b->self);
    else if (arg_b->type == CSHandleTypeShared)
        // shared -> raw
        b_raw = reinterpret_cast<std::shared_ptr<BigInt>*>(arg_b->self)->get();
    else if (arg_b->type == CSHandleTypeUnique)
        // unique -> raw
        b_raw = reinterpret_cast<std::unique_ptr<BigInt>*>(arg_b->self)->get();

    void* res = N_BigInt_my_add_shared(&a_shared, b_raw);
    return new CS_BigInt{res, CSHandleTypeShared};
}

// void* CS_BigInt_my_add_unique(void* a, CSHandleType a_type, void* b, CSHandleType b_type) {
//     void * a_unique =  nullptr;
//     if (a_type == CSHandleTypeRaw)
//         a_unique = new std::unique_ptr<BigInt>(reinterpret_cast<BigInt*>(a));
//     else if (a_type == CSHandleTypeShared){
//         a_unique = new std::unique_ptr<BigInt>(reinterpret_cast<std::shared_ptr<BigInt>*>(a)->get());
//     }
//     else if(a_type == CSHandleTypeUnique)
//         a_unique = new std::unique_ptr<BigInt>(std::move(*reinterpret_cast<std::unique_ptr<BigInt>*>(a)));

//     void * b_raw =  nullptr;
//     if (b_type == CSHandleTypeRaw)
//         b_raw = b;
//     else if (b_type == CSHandleTypeShared)
//         b_raw = reinterpret_cast<std::shared_ptr<BigInt>*>(b)->get();
//     else if (b_type == CSHandleTypeUnique)
//         b_raw = reinterpret_cast<std::unique_ptr<BigInt>*>(b)->get();

//     return N_BigInt_my_add_unique(a_unique, b_raw);
// }
// }

void* CS_BigInt_my_add_unique(void* a, void* b){
  // shared -> unique
  std::unique_ptr<BigInt> a_unique = std::unique_ptr<BigInt>(new BigInt(*reinterpret_cast<std::shared_ptr<BigInt>*>(a)->get()));

  // shared -> raw
  BigInt* b_raw = reinterpret_cast<std::shared_ptr<BigInt>*>(b)->get();

  void* result = N_BigInt_my_add_unique(&a_unique, b_raw);

  // unqiue -> shared
  return new std::shared_ptr<BigInt>(std::move(*reinterpret_cast<std::unique_ptr<BigInt>*>(a)));

}
}