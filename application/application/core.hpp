#pragma once

/**
 * @brief This file is seperated from gemstone/Core.hpp so that the Gemstone source doesn't
 * need to know about the application information
 */

/**
 * @brief This is so we don't get linting warnings showing that the version macros
 * are undefined. By doing this the macros will always be defined
 */
#ifdef APPLICATION_VER
#define APP_MAJOR_VER APPLICATION_MAJOR_VER
#define APP_MINOR_VER APPLICATION_MINOR_VER
#define APP_PATCH_VER APPLICATION_PATCH_VER
#else
#define APP_MAJOR_VER -1
#define APP_MINOR_VER -1
#define APP_PATCH_VER -1
#endif

/**
 * @brief A macro allowing us to assert at compile time that we have information about Gemstone's
 *  version from preprocessor directives. This is essentially just a check to ensure that we were
 * compiled correctly with all of the information that we need.
 */
#ifdef DEBUG
#define ASSERT_APP_VER() \
static_assert(APP_MAJOR_VER >= 0); \
static_assert(APP_MINOR_VER >= 0); \
static_assert(APP_PATCH_VER >= 0); \
REQUIRE_SEMICOLON
#else
#define ASSERT_APP_VER() \
REQUIRE_SEMICOLON
#endif