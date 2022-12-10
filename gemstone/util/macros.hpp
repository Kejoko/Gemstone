#pragma once

/**
 * @brief A macro to wrap around NDEBUG. This allows us to just use DEBUG instead of NDEBUG
 */
#if defined NDEBUG && !defined DEBUG
#define DEBUG
#endif
#if defined DEBUG && !defined NDEBUG
#define NDEBUG
#endif

/**
 * @brief A simple macro to be placed on the end of a macro serving as a function.
 * This requires the macros ending with this to have a semicolon following it.
 * 
 * @example Check out ASSERT_GEM_VERSION() in gemstone/core.hpp
 */
#define REQUIRE_SEMICOLON static_assert(true)

/**
 * @brief A macro to use to get rid of the unused variable/parameter warnings and
 * errors when prototyping a function
 * 
 * @example UNUSED(argv);
 */
#define UNUSED(x) (void)(x); REQUIRE_SEMICOLON

/**
 * @brief A macro to generate a unique variable name given the base name for that
 * variable.
 * 
 * @example See gemstone/util/logger/Logger.hpp LOG_SCOPE_CHANGE()
 */
#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b
#define UNIQUE_NAME(baseName) CONCAT(baseName, __LINE__)