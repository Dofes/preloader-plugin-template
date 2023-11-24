#pragma once
#pragma warning(error : 4834)

#define VA_EXPAND(...) __VA_ARGS__

#ifdef CC_EXPORT
#define LPAPI [[maybe_unused]] __declspec(dllexport)
#else
#define LPAPI [[maybe_unused]] __declspec(dllimport)
#endif