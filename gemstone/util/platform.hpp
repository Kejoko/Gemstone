#pragma once

/**
 * @brief Macros to tell you what operating system you are on
 * 
 * @link https://sourceforge.net/p/predef/wiki/OperatingSystems/
 */

/**
 * @brief Mac OS 9
 */
#if defined macintosh || defined Macintosh

#ifndef ON_MAC
#define ON_MAC
#endif

#ifdef MAC_VERSION
#undef MAC_VERSION
#endif
#define MAC_VERSION 9

#endif
