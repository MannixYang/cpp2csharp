#pragma once

#include <exception>
namespace math {
    class Calculator {
    public:
        Calculator() = default;
        int Add(int a, int b) { return a + b; }
        int Sub(int a, int b) { return a - b; }
    };
}

namespace physics {
    class Vector2D {
    public:
        double x, y;
        Vector2D(double x = 0, double y = 0) : x(x), y(y) {}
        double Length2() const { 
            throw std::exception();
            return x * x + y * y; 
        }
    };
}
