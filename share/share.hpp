#pragma once
#include <windows.h>

#ifdef __cplusplus
#define DLL_EXPORT_C_DECL extern "C" __declspec(dllexport)
#else
#define DLL_EXPORT_C_DECL __declspec(dllexport)
#endif

//
// dll export function
//

DLL_EXPORT_C_DECL bool init();
DLL_EXPORT_C_DECL bool begin();
DLL_EXPORT_C_DECL bool end();
DLL_EXPORT_C_DECL void line(float x1, float y1, float x2, float y2, int rgb, float t = 1.0f);
DLL_EXPORT_C_DECL void rect(float x, float y, float w, float h, int rgb, float t = 1.0f);
DLL_EXPORT_C_DECL void circle(float x, float y, float r, int rgb, float t = 1.0f);
DLL_EXPORT_C_DECL void text(float x, float y, int rgb, const char* txt);
