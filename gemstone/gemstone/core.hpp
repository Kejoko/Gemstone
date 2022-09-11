#pragma once

#include "util/macros.hpp"

/**
 * @brief This is so we don't get linting warnings showing that the version macros
 * are undefined. By doing this the macros will always be defined
 */
#ifdef GEMSTONE_VER
#define GT_MAJOR_VER GEMSTONE_MAJOR_VER
#define GT_MINOR_VER GEMSTONE_MINOR_VER
#define GT_PATCH_VER GEMSTONE_PATCH_VER
#else
#define GT_MAJOR_VER -1
#define GT_MINOR_VER -1
#define GT_PATCH_VER -1
#endif

/**
 * @brief A macro allowing us to assert at compile time that we have information about Gemstone's
 *  version from preprocessor directives. This is essentially just a check to ensure that we were
 * compiled correctly with all of the information that we need.
 */
#ifdef DEBUG
#define ASSERT_GT_VER() \
static_assert(GT_MAJOR_VER >= 0); \
static_assert(GT_MINOR_VER >= 0); \
static_assert(GT_PATCH_VER >= 0); \
REQUIRE_SEMICOLON
#else
#define ASSERT_GT_VER() \
REQUIRE_SEMICOLON
#endif