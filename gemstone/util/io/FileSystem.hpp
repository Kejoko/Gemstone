#pragma once

#include <string>

namespace GEM {
namespace util {
    class FileSystem;
}
}

class GEM::util::FileSystem {
public: // public static variables
    const static std::string LOGGER_NAME;

public: // public static functions
    static std::string getFullPath(const std::string& path);

public: // public member functions
    FileSystem() = delete;
};