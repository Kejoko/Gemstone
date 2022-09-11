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
 * @example Check out ASSERT_GEM_VER() in gemstone/core.hpp
 */
#define REQUIRE_SEMICOLON static_assert(true)