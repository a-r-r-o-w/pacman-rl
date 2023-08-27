#ifndef HEADER_TYPES_H
#define HEADER_TYPES_H
#pragma once

#include <cstdint>

using byte = unsigned char;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;
using f128 = long double;

static_assert(sizeof(byte) == 1, "byte is not 1 byte");
static_assert(sizeof(f32) == 4, "f32 is not 4 bytes");
static_assert(sizeof(f64) == 8, "f64 is not 8 bytes");
static_assert(sizeof(f128) == 16, "f128 is not 16 bytes");

#endif // HEADER_TYPES_H
