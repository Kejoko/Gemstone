#pragma once

#include "util/macros.hpp"

/**
 * @brief This is so we don't get linting warnings showing that the version macros
 * are undefined. By doing this the macros will always be defined
 */
#ifdef GEMSTONE_VER
#define GEM_MAJOR_VER GEMSTONE_MAJOR_VER
#define GEM_MINOR_VER GEMSTONE_MINOR_VER
#define GEM_PATCH_VER GEMSTONE_PATCH_VER
#else
#define GEM_MAJOR_VER -1
#define GEM_MINOR_VER -1
#define GEM_PATCH_VER -1
#endif

/**
 * @brief A macro allowing us to assert at compile time that we have information about Gemstone's
 *  version from preprocessor directives. This is essentially just a check to ensure that we were
 * compiled correctly with all of the information that we need.
 */
#ifdef DEBUG
#define ASSERT_GEM_VER() \
static_assert(GEM_MAJOR_VER >= 0); \
static_assert(GEM_MINOR_VER >= 0); \
static_assert(GEM_PATCH_VER >= 0); \
REQUIRE_SEMICOLON
#else
#define ASSERT_GEM_VER() \
REQUIRE_SEMICOLON
#endif