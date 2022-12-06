#pragma once

/**
 * @brief This file is seperated from gemstone/Core.hpp so that the Gemstone source doesn't
 * need to know about the application information
 */

/**
 * @brief This is so we don't get linting warnings showing that the version macros
 * are undefined. By doing this the macros will always be defined
 */
#ifdef APPLICATION_VERSION
#define APP_MAJOR_VERSION APPLICATION_MAJOR_VERSION
#define APP_MINOR_VERSION APPLICATION_MINOR_VERSION
#define APP_PATCH_VERSION APPLICATION_PATCH_VERSION
#else
#define APP_MAJOR_VERSION -1
#define APP_MINOR_VERSION -1
#define APP_PATCH_VERSION -1
#endif

/**
 * @brief A macro allowing us to assert at compile time that we have information about Gemstone's
 * version from preprocessor directives. This is essentially just a check to ensure that we were
 * compiled correctly with all of the information that we need.
 */
#ifdef DEBUG
#define ASSERT_APP_VERSION() \
static_assert(APP_MAJOR_VERSION >= 0); \
static_assert(APP_MINOR_VERSION >= 0); \
static_assert(APP_PATCH_VERSION >= 0); \
REQUIRE_SEMICOLON
#else
#define ASSERT_APP_VERSION() \
REQUIRE_SEMICOLON
#endif