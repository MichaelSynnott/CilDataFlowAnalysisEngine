#pragma once

#ifndef WIN32
//#####   LINUX DEFINITIONS   #####
#include <cstdlib>
#include "pal_mstypes.h"
#include "pal.h"
#include "ntimage.h"
#include "corhdr.h"

#define CoTaskMemAlloc(cb) malloc(cb)
#define CoTaskMemFree(cb) free(cb)

#define UINT_PTR_FORMAT "lx"
#define PATH_SEPARATOR u'/'

#define WSTRING std::u16string
#define WSTR(str) u##str
#define WSTRRAW(str) uR##str

#define PROFILER_STUB __attribute__((visibility("hidden"))) EXTERN_C void STDMETHODCALLTYPE

#define EXPORT __attribute__((visibility("default")))
#define IMPORT

#else
//#####   WINDOWS DEFINITIONS   #####
#define UINT_PTR_FORMAT "llx"
#define PATH_SEPARATOR L'\\'
#define WSTRING std::wstring
#define WSTR(str) L##str
#define WSTRRAW(str) LR##str

#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)

#define PROFILER_STUB EXTERN_C void STDMETHODCALLTYPE
#endif
