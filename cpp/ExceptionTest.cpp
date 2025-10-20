#include <stdexcept>

// Native C++
void test_exception(){
    throw std::runtime_error("this is test exception.");
}
extern "C"{
// Native C
typedef void(* std_exception_callback_type)(const char* msg);
std_exception_callback_type std_exception_callback = nullptr;


void __declspec(dllexport) std_exception_callback_register(std_exception_callback_type callback){
    std_exception_callback = callback;
}

void c_native_test_exception(){
    try{
        test_exception();
    } catch (std::exception& e){
        if (std_exception_callback != nullptr){
            std_exception_callback(e.what());
        }
        else{
            throw e; // rethrow
        }
    }
}

// CSharp C Wrapper
void __declspec(dllexport) csharp_test_exception(){
    c_native_test_exception();
}
}