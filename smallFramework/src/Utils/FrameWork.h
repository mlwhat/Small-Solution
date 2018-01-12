#pragma once
#include <stdint.h>
#include <memory>
#include <glm/glm.hpp>
#include <iostream>

#include "Logger.h"



#ifndef arraysize
#define arraysize(a) (sizeof(a)/sizeof(a[0]))
#endif
#ifndef offsetof
#define offsetof(s, m) (size_t)( (ptrdiff_t)&reinterpret_cast<const volatile char&>((((s *)0)->m)) )
#endif

#ifdef assert
#undef assert
#endif

#ifdef _DEBUG
#define assert(a)\
    if (!(a)) {\
        std::string str = "assertion failed(" + std::string(#a) + ")\nFile " + __FILE__ + ", line " + std::to_string(__LINE__);\
        smallFrameWork::logError(str);\
    }

#define should_not_get_here() assert(false);
#else
#ifdef _AUTOTESTING
#define assert(a) if (!(a)) throw std::exception("Assertion Failure");
#else
#define assert(a)
#endif

#define should_not_get_here() __assume(0)
#endif

#define safe_delete(_a) {delete _a; _a = nullptr;}
#define safe_delete_array(_a) {delete[] _a; _a = nullptr;}
#define align_to(_alignment, _val) (((_val + _alignment - 1) / _alignment) * _alignment)

#if defined(_MSC_VER)
#define FALCOR_DEPRECATED(MESSAGE) __declspec(deprecated(MESSAGE))
#else
// TODO: add cases for clang/gcc when/if needed
#define FALCOR_DEPRECATED(MESSAGE) /* emtpy */
#endif
