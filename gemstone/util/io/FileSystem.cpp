#include "util/io/FileSystem.hpp"

#include <string>

#include "util/platform.hpp"
#include "util/io/logger.hpp"
#include "util/logger/Logger.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the Texure class uses
 */
const std::string GEM::util::FileSystem::LOGGER_NAME = IO_LOGGER_NAME;

/* ------------------------------ private static variables ------------------------------ */

/* ------------------------------ public static functions ------------------------------ */

/**
 * @brief Get the full file path for a file within the gemstone project. This basically
 * just prepends the absolute path to the gemstone project directory.
 * 
 * @param path The path to the file within gemstone (ex: application/assets/textures/missing_texture.png,
 * or ex: gemstone/gemstone/shader/logger.hpp)
 * @return std::string The absolute path to the file within the gemstone project
 */
std::string GEM::util::FileSystem::getFullPath(const std::string& path) {
    return std::string(PROJECT_ROOT_DIR) + std::string("/") + path;
}

/* ------------------------------ private static functions ------------------------------ */

/* ------------------------------ public member functions ------------------------------ */

/* ------------------------------ private member functions ------------------------------ */
