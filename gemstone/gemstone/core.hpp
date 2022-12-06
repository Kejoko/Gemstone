#pragma once

#include "util/macros.hpp"

/**
 * @brief This is so we don't get linting warnings showing that the version macros
 * are undefined. By doing this the macros will always be defined
 */
#ifdef GEMSTONE_VERSION
#define GEM_MAJOR_VERSION GEMSTONE_MAJOR_VERSION
#define GEM_MINOR_VERSION GEMSTONE_MINOR_VERSION
#define GEM_PATCH_VERSION GEMSTONE_PATCH_VERSION
#else
#define GEM_MAJOR_VERSION -1
#define GEM_MINOR_VERSION -1
#define GEM_PATCH_VERSION -1
#endif

/**
 * @brief A macro allowing us to assert at compile time that we have information about Gemstone's
 *  version from preprocessor directives. This is essentially just a check to ensure that we were
 * compiled correctly with all of the information that we need.
 */
#ifdef DEBUG
#define ASSERT_GEM_VERSION() \
static_assert(GEM_MAJOR_VERSION >= 0); \
static_assert(GEM_MINOR_VERSION >= 0); \
static_assert(GEM_PATCH_VERSION >= 0); \
REQUIRE_SEMICOLON
#else
#define ASSERT_GEM_VERSION() \
REQUIRE_SEMICOLON
#endif

#define GEM_GLFW_MAJOR_VERSION 3
#define GEM_GLFW_MINOR_VERSION 3