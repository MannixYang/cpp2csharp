#pragma once

#ifdef _WIN32
#define API __declspec(dllexport)
#else
#define API
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ---------------- Calculator ----------------
typedef void* CalculatorHandle;

API CalculatorHandle Calculator_Create();
API void Calculator_Destroy(CalculatorHandle handle);

API int Calculator_Add(CalculatorHandle handle, int a, int b);
API int Calculator_Sub(CalculatorHandle handle, int a, int b);

// ---------------- Vector2D ----------------
typedef void* Vector2DHandle;

API Vector2DHandle Vector2D_Create(double x, double y);
API void Vector2D_Destroy(Vector2DHandle handle);

API double Vector2D_Length2(Vector2DHandle handle);

#ifdef __cplusplus
}
#endif
