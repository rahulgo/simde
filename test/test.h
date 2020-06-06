#if !defined(SIMDE_TESTS_H)
#define SIMDE_TESTS_H

#include "../simde/simde-common.h"

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <stdarg.h>

typedef enum SimdeTestVecPos {
  SIMDE_TEST_VEC_POS_FIRST  =  1,
  SIMDE_TEST_VEC_POS_MIDDLE =  0,
  SIMDE_TEST_VEC_POS_LAST   = -1
} SimdeTestVecPos;

HEDLEY_DIAGNOSTIC_PUSH
SIMDE_DIAGNOSTIC_DISABLE_VLA_
SIMDE_DIAGNOSTIC_DISABLE_UNUSED_FUNCTION_
SIMDE_DIAGNOSTIC_DISABLE_PADDED_
SIMDE_DIAGNOSTIC_DISABLE_ZERO_AS_NULL_POINTER_CONSTANT_
SIMDE_DIAGNOSTIC_DISABLE_CAST_FUNCTION_TYPE_
SIMDE_DIAGNOSTIC_DISABLE_NON_CONSTANT_AGGREGATE_INITIALIZER_
SIMDE_DIAGNOSTIC_DISABLE_C99_EXTENSIONS_
SIMDE_DIAGNOSTIC_DISABLE_NO_EMMS_INSTRUCTION_

#if \
    HEDLEY_HAS_BUILTIN(__builtin_abort) || \
    HEDLEY_GCC_VERSION_CHECK(3,4,6) || \
    HEDLEY_ARM_VERSION_CHECK(4,1,0)
  #define simde_abort() __builtin_abort()
#elif defined(SIMDE_HAVE_STDLIB_H)
  #define simde_abort() abort()
#endif

#if !defined(SIMDE_DEBUG)
  HEDLEY_NO_RETURN
#endif
HEDLEY_PRINTF_FORMAT(1, 2)
static void
simde_test_debug_printf_(const char* format, ...) {
  va_list ap;

  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);
  fflush(stderr);

  /* Debug trap is great for local development where you can attach a
   * debugger, but processes exiting with a SIGTRAP seem to be rather
   * confusing for CI. */
  #if defined(SIMDE_DEBUG)
    simde_trap();
  #else
    simde_abort();
  #endif
}

HEDLEY_PRINTF_FORMAT(3, 4)
static void
simde_test_codegen_snprintf_(char* str, size_t size, const char* format, ...) {
  va_list ap;
  int w;

  va_start(ap, format);
  w = vsnprintf(str, size, format, ap);
  va_end(ap);

  if (w > HEDLEY_STATIC_CAST(int, size)) {
    simde_test_debug_printf_("Not enough space to write value (given %zu bytes, need %d bytes)\n", size, w + 1);
  }
}

static void
simde_test_codegen_f32(size_t buf_len, char buf[HEDLEY_ARRAY_PARAM(buf_len)], simde_float32 value) {
  if (simde_math_isnan(value)) {
    simde_test_codegen_snprintf_(buf, buf_len, "%25s", "SIMDE_MATH_NANF");
  } else if (simde_math_isinf(value)) {
    simde_test_codegen_snprintf_(buf, buf_len, "%5cSIMDE_MATH_INFINITYF", value < 0 ? '-' : ' ');
  } else {
    simde_test_codegen_snprintf_(buf, buf_len, "SIMDE_FLOAT32_C(%9.2f)", HEDLEY_STATIC_CAST(double, value));
  }
}

static void
simde_test_codegen_f64(size_t buf_len, char buf[HEDLEY_ARRAY_PARAM(buf_len)], simde_float64 value) {
  if (simde_math_isnan(value)) {
    simde_test_codegen_snprintf_(buf, buf_len, "%26s", "NAN");
  } else if (simde_math_isinf(value)) {
    simde_test_codegen_snprintf_(buf, buf_len, "%7cSIMDE_MATH_INFINITY", value < 0 ? '-' : ' ');
  } else {
    simde_test_codegen_snprintf_(buf, buf_len, "SIMDE_FLOAT64_C(%9.2f)", HEDLEY_STATIC_CAST(double, value));
  }
}

static void
simde_test_codegen_i8(size_t buf_len, char buf[HEDLEY_ARRAY_PARAM(buf_len)], int8_t value) {
  if (value == INT8_MIN) {
    simde_test_codegen_snprintf_(buf, buf_len, "     INT8_MIN");
  } else if (value == INT8_MAX) {
    simde_test_codegen_snprintf_(buf, buf_len, "     INT8_MAX");
  } else {
    simde_test_codegen_snprintf_(buf, buf_len, "%cINT8_C(%4" PRId8 ")", (value < 0) ? '-' : ' ', HEDLEY_STATIC_CAST(int8_t, (value < 0) ? -value : value));
  }
}

static void
simde_test_codegen_i16(size_t buf_len, char buf[HEDLEY_ARRAY_PARAM(buf_len)], int16_t value) {
  if (value == INT16_MIN) {
    simde_test_codegen_snprintf_(buf, buf_len, "%15s", "INT16_MIN");
  } else if (value == INT16_MAX) {
    simde_test_codegen_snprintf_(buf, buf_len, "%15s", "INT16_MAX");
  } else {
    simde_test_codegen_snprintf_(buf, buf_len, "%cINT16_C(%6" PRId16 ")", (value < 0) ? '-' : ' ', HEDLEY_STATIC_CAST(int16_t, (value < 0) ? -value : value));
  }
}

static void
simde_test_codegen_i32(size_t buf_len, char buf[HEDLEY_ARRAY_PARAM(buf_len)], int32_t value) {
  if (value == INT32_MIN) {
    simde_test_codegen_snprintf_(buf, buf_len, "%20s", "INT32_MIN");
  } else if (value == INT32_MAX) {
    simde_test_codegen_snprintf_(buf, buf_len, "%20s", "INT32_MAX");
  } else {
    simde_test_codegen_snprintf_(buf, buf_len, "%cINT32_C(%12" PRId32 ")", (value < 0) ? '-' : ' ', HEDLEY_STATIC_CAST(int32_t, (value < 0) ? -value : value));
  }
}

static void
simde_test_codegen_i64(size_t buf_len, char buf[HEDLEY_ARRAY_PARAM(buf_len)], int64_t value) {
  if (value == INT64_MIN) {
    simde_test_codegen_snprintf_(buf, buf_len, "%29s", "INT64_MIN");
  } else if (value == INT64_MAX) {
    simde_test_codegen_snprintf_(buf, buf_len, "%29s", "INT64_MAX");
  } else {
    simde_test_codegen_snprintf_(buf, buf_len, "%cINT64_C(%20" PRId64 ")", (value < 0) ? '-' : ' ', HEDLEY_STATIC_CAST(int64_t, (value < 0) ? -value : value));
  }
}

static void
simde_test_codegen_u8(size_t buf_len, char buf[HEDLEY_ARRAY_PARAM(buf_len)], uint8_t value) {
  if (value == UINT8_MAX) {
    simde_test_codegen_snprintf_(buf, buf_len, "   UINT8_MAX");
  } else {
    simde_test_codegen_snprintf_(buf, buf_len, "UINT8_C(%3" PRIu8 ")", value);
  }
}

static void
simde_test_codegen_u16(size_t buf_len, char buf[HEDLEY_ARRAY_PARAM(buf_len)], uint16_t value) {
  if (value == UINT16_MAX) {
    simde_test_codegen_snprintf_(buf, buf_len, "%15s", "UINT16_MAX");
  } else {
    simde_test_codegen_snprintf_(buf, buf_len, "UINT16_C(%5" PRIu16 ")", value);
  }
}

static void
simde_test_codegen_u32(size_t buf_len, char buf[HEDLEY_ARRAY_PARAM(buf_len)], uint32_t value) {
  if (value == UINT32_MAX) {
    simde_test_codegen_snprintf_(buf, buf_len, "%20s", "UINT32_MAX");
  } else {
    simde_test_codegen_snprintf_(buf, buf_len, "UINT32_C(%10" PRIu32 ")", value);
  }
}

static void
simde_test_codegen_u64(size_t buf_len, char buf[HEDLEY_ARRAY_PARAM(buf_len)], uint64_t value) {
  if (value == UINT64_MAX) {
    simde_test_codegen_snprintf_(buf, buf_len, "%29s", "UINT64_MAX");
  } else {
    simde_test_codegen_snprintf_(buf, buf_len, "UINT64_C(%20" PRIu64 ")", value);
  }
}

static void
simde_test_codegen_write_indent(int indent) {
  for (int i = 0 ; i < indent ; i++) {
    fputs("  ", stdout);
  }
}

static int simde_codegen_rand(void) {
  /* Single-threaded programs are so nice */
  static int is_init = 0;
  if (HEDLEY_UNLIKELY(!is_init)) {
    srand(HEDLEY_STATIC_CAST(unsigned int, time(NULL)));
    is_init = 1;
  }
  return rand();
}

static void
simde_codegen_random_memory(size_t buf_len, uint8_t buf[HEDLEY_ARRAY_PARAM(buf_len)]) {
  for (size_t i = 0 ; i < buf_len ; i++) {
    buf[i] = HEDLEY_STATIC_CAST(uint8_t, simde_codegen_rand() & 0xff);
  }
}

static simde_float32
simde_codegen_random_f32(simde_float32 min, simde_float32 max) {
  return (HEDLEY_STATIC_CAST(simde_float32, simde_codegen_rand()) / (HEDLEY_STATIC_CAST(simde_float32, RAND_MAX) / (max - min))) + min;
}

static simde_float64
simde_codegen_random_f64(simde_float64 min, simde_float64 max) {
  return (HEDLEY_STATIC_CAST(simde_float64, simde_codegen_rand()) / (HEDLEY_STATIC_CAST(simde_float64, RAND_MAX) / (max - min))) + min;
}

static void
simde_codegen_random_vf32(size_t elem_count, simde_float32 values[HEDLEY_ARRAY_PARAM(elem_count)], simde_float32 min, simde_float32 max) {
  for (size_t i = 0 ; i < elem_count ; i++) {
    values[i] = simde_codegen_random_f32(min, max);
  }
}

static void
simde_codegen_random_vf64(size_t elem_count, simde_float64 values[HEDLEY_ARRAY_PARAM(elem_count)], simde_float64 min, simde_float64 max) {
  for (size_t i = 0 ; i < elem_count ; i++) {
    values[i] = simde_codegen_random_f64(min, max);
  }
}

#define SIMDE_TEST_CODEGEN_GENERATE_WRITE_VECTOR_FUNC_(T, symbol_identifier, elements_per_line) \
  static void \
  simde_test_codegen_write_v##symbol_identifier(int indent, size_t elem_count, T values[HEDLEY_ARRAY_PARAM(elem_count)], SimdeTestVecPos pos) { \
    switch (pos) { \
      case SIMDE_TEST_VEC_POS_FIRST: \
        simde_test_codegen_write_indent(indent); \
        indent++; \
        fputs("{ ", stdout); \
        break; \
      case SIMDE_TEST_VEC_POS_MIDDLE: \
      case SIMDE_TEST_VEC_POS_LAST: \
        indent++; \
        simde_test_codegen_write_indent(indent); \
        break; \
    } \
 \
    fputs("{ ", stdout); \
    for (size_t i = 0 ; i < elem_count ; i++) { \
      if (i != 0) { \
        fputc(',', stdout); \
        if ((i % elements_per_line) == 0) { \
          fputc('\n', stdout); \
          simde_test_codegen_write_indent(indent + 1); \
        } else { \
          fputc(' ', stdout); \
        } \
      } \
 \
      char buf[53]; \
      simde_test_codegen_##symbol_identifier(sizeof(buf), buf, values[i]); \
      fputs(buf, stdout); \
    } \
    fputs(" }", stdout); \
 \
    switch (pos) { \
      case SIMDE_TEST_VEC_POS_FIRST: \
      case SIMDE_TEST_VEC_POS_MIDDLE: \
        fputc(',', stdout); \
        break; \
      case SIMDE_TEST_VEC_POS_LAST: \
        fputs(" },", stdout); \
        break; \
    } \
 \
    fputc('\n', stdout); \
  }

SIMDE_TEST_CODEGEN_GENERATE_WRITE_VECTOR_FUNC_(simde_float32, f32, 4)
SIMDE_TEST_CODEGEN_GENERATE_WRITE_VECTOR_FUNC_(simde_float64, f64, 4)
SIMDE_TEST_CODEGEN_GENERATE_WRITE_VECTOR_FUNC_(int8_t, i8, 8)
SIMDE_TEST_CODEGEN_GENERATE_WRITE_VECTOR_FUNC_(int16_t, i16, 8)
SIMDE_TEST_CODEGEN_GENERATE_WRITE_VECTOR_FUNC_(int32_t, i32, 8)
SIMDE_TEST_CODEGEN_GENERATE_WRITE_VECTOR_FUNC_(int64_t, i64, 4)
SIMDE_TEST_CODEGEN_GENERATE_WRITE_VECTOR_FUNC_(uint8_t, u8, 8)
SIMDE_TEST_CODEGEN_GENERATE_WRITE_VECTOR_FUNC_(uint16_t, u16, 8)
SIMDE_TEST_CODEGEN_GENERATE_WRITE_VECTOR_FUNC_(uint32_t, u32, 8)
SIMDE_TEST_CODEGEN_GENERATE_WRITE_VECTOR_FUNC_(uint64_t, u64, 4)

#define SIMDE_TEST_CODEGEN_WRITE_SCALAR_FUNC_(T, symbol_identifier) \
  static void \
  simde_test_codegen_write_##symbol_identifier(int indent, T value, SimdeTestVecPos pos) { \
    switch (pos) { \
      case SIMDE_TEST_VEC_POS_FIRST: \
        simde_test_codegen_write_indent(indent); \
        indent++; \
        fputs("{ ", stdout); \
        break; \
      case SIMDE_TEST_VEC_POS_MIDDLE: \
      case SIMDE_TEST_VEC_POS_LAST: \
        indent++; \
        simde_test_codegen_write_indent(indent); \
        break; \
    } \
 \
    { \
      char buf[53]; \
      simde_test_codegen_##symbol_identifier(sizeof(buf), buf, value); \
      fputs(buf, stdout); \
    } \
 \
    switch (pos) { \
      case SIMDE_TEST_VEC_POS_FIRST: \
      case SIMDE_TEST_VEC_POS_MIDDLE: \
        fputc(',', stdout); \
        break; \
      case SIMDE_TEST_VEC_POS_LAST: \
        fputs(" },", stdout); \
        break; \
    } \
 \
    fputc('\n', stdout); \
  }

SIMDE_TEST_CODEGEN_WRITE_SCALAR_FUNC_(int8_t,    i8)
SIMDE_TEST_CODEGEN_WRITE_SCALAR_FUNC_(int16_t,  i16)
SIMDE_TEST_CODEGEN_WRITE_SCALAR_FUNC_(int32_t,  i32)
SIMDE_TEST_CODEGEN_WRITE_SCALAR_FUNC_(int64_t,  i64)
SIMDE_TEST_CODEGEN_WRITE_SCALAR_FUNC_(uint8_t,   u8)
SIMDE_TEST_CODEGEN_WRITE_SCALAR_FUNC_(uint16_t, u16)
SIMDE_TEST_CODEGEN_WRITE_SCALAR_FUNC_(uint32_t, u32)
SIMDE_TEST_CODEGEN_WRITE_SCALAR_FUNC_(uint64_t, u64)

static int
simde_test_equal_f32(simde_float32 a, simde_float32 b, simde_float32 slop) {
  if (simde_math_isnan(a)) {
    return simde_math_isnan(b);
  } else if (simde_math_isinf(a)) {
    return !((a < b) || (a > b));
  } else {
    return ((b > (a - slop)) && (b < (a + slop)));
  }
}

static int
simde_test_equal_f64(simde_float64 a, simde_float64 b, simde_float64 slop) {
  if (simde_math_isnan(a)) {
    return simde_math_isnan(b);
  } else if (simde_math_isinf(a)) {
    return !((a < b) || (a > b));
  } else {
    return ((b > (a - slop)) && (b < (a + slop)));
  }
}

static int
simde_assert_equal_vf32_(
    size_t vec_len, simde_float32 const a[HEDLEY_ARRAY_PARAM(vec_len)], simde_float32 const b[HEDLEY_ARRAY_PARAM(vec_len)], simde_float32 slop,
    const char* filename, int line, const char* astr, const char* bstr) {
  for (size_t i = 0 ; i < vec_len ; i++) {
    if (HEDLEY_UNLIKELY(!simde_test_equal_f32(a[i], b[i], slop))) {
      simde_test_debug_printf_("%s:%d: assertion failed: %s[%zu] ~= %s[%zu] (%f ~= %f)\n",
              filename, line, astr, i, bstr, i, HEDLEY_STATIC_CAST(double, a[i]), HEDLEY_STATIC_CAST(double, b[i]));
    }
  }
  return 1;
}
#define simde_assert_equal_vf32(vec_len, a, b, precision) simde_assert_equal_vf32_(vec_len, a, b, 1e-##precision##f, __FILE__, __LINE__, #a, #b)

static int
simde_assert_equal_vf64_(
    size_t vec_len, simde_float64 const a[HEDLEY_ARRAY_PARAM(vec_len)], simde_float64 const b[HEDLEY_ARRAY_PARAM(vec_len)], simde_float64 slop,
    const char* filename, int line, const char* astr, const char* bstr) {
  for (size_t i = 0 ; i < vec_len ; i++) {
    if (HEDLEY_UNLIKELY(!simde_test_equal_f64(a[i], b[i], slop))) {
      simde_test_debug_printf_("%s:%d: assertion failed: %s[%zu] ~= %s[%zu] (%f ~= %f)\n",
              filename, line, astr, i, bstr, i, HEDLEY_STATIC_CAST(double, a[i]), HEDLEY_STATIC_CAST(double, b[i]));
    }
  }
  return 1;
}
#define simde_assert_equal_vf64(vec_len, a, b, precision) simde_assert_equal_vf64_(vec_len, a, b, 1e-##precision, __FILE__, __LINE__, #a, #b)

#if !defined(SIMDE_TEST_ASSERTION_FAILURES_NON_FATAL)
  #define SIMDE_TEST_GENERATE_ASSERT_EQUAL_FUNC_(T, symbol_identifier, fmt) \
    static int \
    simde_assert_equal_v##symbol_identifier##_( \
        size_t vec_len, T const a[HEDLEY_ARRAY_PARAM(vec_len)], T const b[HEDLEY_ARRAY_PARAM(vec_len)], \
        const char* filename, int line, const char* astr, const char* bstr) { \
      for (size_t i = 0 ; i < vec_len ; i++) { \
        if (HEDLEY_UNLIKELY(a[i] != b[i])) { \
          simde_test_debug_printf_("%s:%d: assertion failed: %s[%zu] != %s[%zu] (%" fmt " != %" fmt ")\n", \
                filename, line, astr, i, bstr, i, a[i], b[i]); \
        } \
      } \
      return 1; \
    }
#else
  #define SIMDE_TEST_GENERATE_ASSERT_EQUAL_FUNC_(T, symbol_identifier, fmt) \
    static int \
    simde_assert_equal_v##symbol_identifier##_( \
        size_t vec_len, T a[HEDLEY_ARRAY_PARAM(vec_len)], T b[HEDLEY_ARRAY_PARAM(vec_len)], \
        const char* filename, int line, const char* astr, const char* bstr) { \
      for (size_t i = 0 ; i < vec_len ; i++) { \
        if (HEDLEY_UNLIKELY(a[i] != b[i])) { \
          simde_test_debug_printf_("%s:%d: assertion failed: %s[%zu] != %s[%zu] (%" fmt " != %" fmt ")\n", \
                filename, line, astr, i, bstr, i, a[i], b[i]); \
        } \
      } \
      return 1; \
    }
#endif

SIMDE_TEST_GENERATE_ASSERT_EQUAL_FUNC_(int8_t,    i8,  PRId8)
SIMDE_TEST_GENERATE_ASSERT_EQUAL_FUNC_(int16_t,  i16, PRId16)
SIMDE_TEST_GENERATE_ASSERT_EQUAL_FUNC_(int32_t,  i32, PRId32)
SIMDE_TEST_GENERATE_ASSERT_EQUAL_FUNC_(int64_t,  i64, PRId64)
SIMDE_TEST_GENERATE_ASSERT_EQUAL_FUNC_(uint8_t,   u8,  PRIu8)
SIMDE_TEST_GENERATE_ASSERT_EQUAL_FUNC_(uint16_t, u16, PRIu16)
SIMDE_TEST_GENERATE_ASSERT_EQUAL_FUNC_(uint32_t, u32, PRIu32)
SIMDE_TEST_GENERATE_ASSERT_EQUAL_FUNC_(uint64_t, u64, PRIu64)

#define simde_assert_equal_vi8(vec_len, a, b) simde_assert_equal_vi8_(vec_len, a, b, __FILE__, __LINE__, #a, #b)
#define simde_assert_equal_vi16(vec_len, a, b) simde_assert_equal_vi16_(vec_len, a, b, __FILE__, __LINE__, #a, #b)
#define simde_assert_equal_vi32(vec_len, a, b) simde_assert_equal_vi32_(vec_len, a, b, __FILE__, __LINE__, #a, #b)
#define simde_assert_equal_vi64(vec_len, a, b) simde_assert_equal_vi64_(vec_len, a, b, __FILE__, __LINE__, #a, #b)
#define simde_assert_equal_vu8(vec_len, a, b) simde_assert_equal_vu8_(vec_len, a, b, __FILE__, __LINE__, #a, #b)
#define simde_assert_equal_vu16(vec_len, a, b) simde_assert_equal_vu16_(vec_len, a, b, __FILE__, __LINE__, #a, #b)
#define simde_assert_equal_vu32(vec_len, a, b) simde_assert_equal_vu32_(vec_len, a, b, __FILE__, __LINE__, #a, #b)
#define simde_assert_equal_vu64(vec_len, a, b) simde_assert_equal_vu64_(vec_len, a, b, __FILE__, __LINE__, #a, #b)

/* Since each test is compiled in 4 different versions (C/C++ and
 * native/emul), we need to be able to generate different symbols
 * depending on preprocessor macros. */
#if defined(SIMDE_NO_NATIVE)
  #if defined(__cplusplus)
    #define SIMDE_TEST_GENERATE_VARIANT_SYMBOL_CURRENT(name) HEDLEY_CONCAT(name,_emul_cpp)
    #define SIMDE_TEST_GENERATE_VARIANT_NAME_CURRENT(name) #name "/emul/cpp"
  #else
    #define SIMDE_TEST_GENERATE_VARIANT_SYMBOL_CURRENT(name) HEDLEY_CONCAT(name,_emul_c)
    #define SIMDE_TEST_GENERATE_VARIANT_NAME_CURRENT(name) #name "/emul/c"
  #endif
#else
  #if defined(__cplusplus)
    #define SIMDE_TEST_GENERATE_VARIANT_SYMBOL_CURRENT(name) HEDLEY_CONCAT(name,_native_cpp)
    #define SIMDE_TEST_GENERATE_VARIANT_NAME_CURRENT(name) #name "/native/cpp"
  #else
    #define SIMDE_TEST_GENERATE_VARIANT_SYMBOL_CURRENT(name) HEDLEY_CONCAT(name,_native_c)
    #define SIMDE_TEST_GENERATE_VARIANT_NAME_CURRENT(name) #name "/native/c"
  #endif
#endif

/* The bare version basically assumes you just want to run a single
 * test suite.  It doesn't use munit, or any other dependencies so
 * it's easy to use with creduce. */
#if defined(SIMDE_TEST_BARE)
  #define SIMDE_TEST_FUNC_LIST_BEGIN static SimdeTestFunc test_suite_tests[] = {
  #define SIMDE_TEST_FUNC_LIST_ENTRY(name) test_simde_##name,
  #define SIMDE_TEST_FUNC_LIST_END };
#else
  HEDLEY_DIAGNOSTIC_PUSH
  SIMDE_DIAGNOSTIC_DISABLE_CPP98_COMPAT_PEDANTIC_
  SIMDE_DIAGNOSTIC_DISABLE_OLD_STYLE_CAST_
  SIMDE_DIAGNOSTIC_DISABLE_VARIADIC_MACROS_
  #include "munit/munit.h"
  HEDLEY_DIAGNOSTIC_POP

  #define SIMDE_TEST_FUNC_LIST_BEGIN static MunitTest test_suite_tests[] = {
  #if defined(__cplusplus)
    #define SIMDE_TEST_FUNC_LIST_ENTRY(name) { \
        const_cast<char*>("/" SIMDE_TEST_GENERATE_VARIANT_NAME_CURRENT(name)), \
        HEDLEY_REINTERPRET_CAST(MunitTestFunc, test_simde_##name), \
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  #else
    #define SIMDE_TEST_FUNC_LIST_ENTRY(name) { \
        (char*) "/" SIMDE_TEST_GENERATE_VARIANT_NAME_CURRENT(name), \
        HEDLEY_REINTERPRET_CAST(MunitTestFunc, test_simde_##name), \
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  #endif
  #define SIMDE_TEST_FUNC_LIST_END { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL } };

  #define SIMDE_TEST_SUITE_DECLARE_GETTERS(name) \
    HEDLEY_C_DECL MunitSuite* HEDLEY_CONCAT(name, _native_c)(void); \
    HEDLEY_C_DECL MunitSuite* HEDLEY_CONCAT(name, _emul_c)(void); \
    HEDLEY_C_DECL MunitSuite* HEDLEY_CONCAT(name, _native_cpp)(void); \
    HEDLEY_C_DECL MunitSuite* HEDLEY_CONCAT(name, _emul_cpp)(void);
#endif

#endif /* !defined(SIMDE_TESTS_H) */
