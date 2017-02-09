/**
 *	File: AXengine/Tool/Utility.h
 *	Purpose: Define portable types
 */

#ifndef __AX__TOOL_UTILITY_H
#define __AX__TOOL_UTILITY_H

#include <cstdlib>
#include <cstdint>

namespace AX { namespace Tool {

typedef float F32;
typedef double F64;
typedef char CHR;
typedef unsigned char UCHR;
typedef wchar_t WCHR;

typedef int_fast8_t I8;
typedef uint_fast8_t U8;

typedef int_fast16_t I16;
typedef uint_fast16_t U16;

typedef int_fast32_t I32;
typedef uint_fast32_t U32;

typedef int_fast64_t I64;
typedef uint_fast64_t U64;

typedef size_t SIZE;

const SIZE I8_MIN = INT_FAST8_MIN;
const SIZE I8_MAX = INT_FAST8_MAX;
const SIZE U8_MAX = UINT_FAST8_MAX;

const SIZE I16_MIN = INT_FAST16_MIN;
const SIZE I16_MAX = INT_FAST16_MAX;
const SIZE U16_MAX = UINT_FAST16_MAX;

const SIZE I32_MIN = INT_FAST32_MIN;
const SIZE I32_MAX = INT_FAST32_MAX;
const SIZE U32_MAX = UINT_FAST32_MAX;

const SIZE I64_MIN = INT_FAST64_MIN;
const SIZE I64_MAX = INT_FAST64_MAX;
const SIZE U64_MAX = UINT_FAST64_MAX;

} } //namespace AX::Tool

#endif // __AX__TOOL_UTILITY_H
