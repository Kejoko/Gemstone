#include <iostream>

#include "gemstone/core.hpp"

#include "application/core.hpp"

int main(int argc, char* argv[]) {
    ASSERT_GT_VER();
    ASSERT_APP_VER();

    std::cout << "hello world\n";

#ifdef DEBUG
    std::cout << "in debug mode\n";
#endif

#ifdef NDEBUG
    std::cout << "actually in debug mode!!!!\n";
#endif

    std::cout << "Gemstone Version: " << GT_MAJOR_VER << "." << GT_MINOR_VER << "." << GT_PATCH_VER << "\n";
    std::cout << "Application Version: " << APP_MAJOR_VER << "." << APP_MINOR_VER << "." << APP_PATCH_VER << "\n";

    return 0;
}