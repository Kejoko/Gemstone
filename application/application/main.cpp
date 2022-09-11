#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "util/logger/Logger.hpp"
#include "gemstone/core.hpp"
#include "application/core.hpp"

int main(int argc, char* argv[]) {
    ASSERT_GEM_VER();
    ASSERT_APP_VER();

    GEM::Logger::init();

    return 0;
}