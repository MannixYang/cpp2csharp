#include "MyLib.h"
#include "MyLib_CAPI.h"
#include <iostream>
#include <exception>

// ------------- Calculator -------------
CalculatorHandle Calculator_Create() {
    return new math::Calculator();
}

void Calculator_Destroy(CalculatorHandle handle) {
    delete static_cast<math::Calculator*>(handle);
}

int Calculator_Add(CalculatorHandle handle, int a, int b) {
    return static_cast<math::Calculator*>(handle)->Add(a, b);
}

int Calculator_Sub(CalculatorHandle handle, int a, int b) {
    return static_cast<math::Calculator*>(handle)->Sub(a, b);
}

// ------------- Vector2D -------------
Vector2DHandle Vector2D_Create(double x, double y) {
    return new physics::Vector2D(x, y);
}

void Vector2D_Destroy(Vector2DHandle handle) {
    delete static_cast<physics::Vector2D*>(handle);
}

double Vector2D_Length2_Native(Vector2DHandle handle) {
    return static_cast<physics::Vector2D*>(handle)->Length2();
}

double Vector2D_Length2(Vector2DHandle handle) {
    try{
        double res = Vector2D_Length2_Native(handle);
        return res;
    }catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << '\n';
        return -1;
    }
    // return static_cast<physics::Vector2D*>(handle)->Length2();
}
