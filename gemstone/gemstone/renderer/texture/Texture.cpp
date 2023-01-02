#include <string>

#include <glad/glad.h>

#include <stb/stb_image.h>

#include "util/logger/Logger.hpp"

#include "gemstone/renderer/texture/logger.hpp"
#include "gemstone/renderer/texture/Texture.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the Texure class uses
 */
const std::string GEM::Texture::LOGGER_NAME = TEXTURE_LOGGER_NAME;

/* ------------------------------ private static variables ------------------------------ */

/* ------------------------------ public static functions ------------------------------ */

/* ------------------------------ private static functions ------------------------------ */

/**
 * @brief Get the format of the input texture's pixels based on the file type
 * 
 * @param filename The filename of the texture we are loading
 * @return GLenum The input data format. GL_RGB if jpg image or GL_RGBA if png image
 */
GLenum GEM::Texture::getInputFormat(const std::string& filename) {
    // Get the file extension (everything after the last .)
    const std::string extension = filename.substr(
        filename.find_last_of(".") + 1
    );

    LOG_TRACE("Filename {} has extension {}", filename, extension);

    if (extension == "jpg") {
        return GL_RGB;
    } else if (extension == "png") {
        return GL_RGBA;
    }

    return GL_RGB;
}

/**
 * @brief Create an opengl texture and get its id
 * 
 * @note This function will throw if the texture file cannot be loaded
 * 
 * @param filename The filename from which to load the texture
 * @return uint32_t The id of the newly created texture
 */
uint32_t GEM::Texture::createTexture(const std::string& filename) {
    LOG_FUNCTION_CALL_INFO("filename {}", filename);

    // Create the texture in open gl and bind it so the subsequent configuration options affect it
    uint32_t textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set the texture wrapping method
    // Don't forget to set the border color with glTexParameterfv if we use GL_CLAMP_TO_BORDER for wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set the texture filtering method (nearest or linear interpolation) for both magnifying (scaling up the
    // texture) and minifying (scaling down the texture) operations
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Create and use the mipmaps. This isn't importatnt for the mag filter because it only applies when upscaling
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load the texture
    // For the issue with loading pngs:
    // https://stackoverflow.com/questions/23150123/loading-png-with-stb-image-for-opengl-texture-gives-wrong-colors
    int textureWidth;
    int textureHeight;
    int textureChannelCount;
    stbi_set_flip_vertically_on_load(true);
    uint8_t* p_textureData = stbi_load(filename.c_str(), &textureWidth, &textureHeight, &textureChannelCount, 0);
    if (!p_textureData) {
        const std::string errorMessage = "Failed to stbi_load texture at " + filename;
        LOG_CRITICAL(errorMessage);
        throw std::invalid_argument(errorMessage);
    }

    glTexImage2D(
        GL_TEXTURE_2D,                          // The texture target (we are bound to 2d due to the glBindTexture call)
        0,                                      // The mipmap level for which we want to create a texture for
        GL_RGB,                                 // What kind of format we want to store the texture
        textureWidth,                           // Set the width of the resulting texture
        textureHeight,                          // Set the height of the resulting texture
        0,                                      // Should always be 0 -- legacy stuff
        GEM::Texture::getInputFormat(filename), // Format of the source image (include alpha for png images)
        GL_UNSIGNED_BYTE,                       // Data type of the source image
        p_textureData                           // The actual image data
    );
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free the image memory now that we have generated the texture
    stbi_image_free(p_textureData);

    LOG_DEBUG("Successfully created texture with id {}", textureID);

    return textureID;
}

/* ------------------------------ public member functions ------------------------------ */

/**
 * @brief Construct a new GEM::Texture::Texture object given a filename and the specified
 * parameters
 * 
 * @param filename The filename of the texture to load
 * @param index The index we are assigning this texture to
 */
GEM::Texture::Texture(const std::string& filename, const uint32_t index) :
    m_id(GEM::Texture::createTexture(filename)),
    m_index(index)
{}

/**
 * @brief Destroy the GEM::Texture::Texture object by deleting the opengl texture
 */
GEM::Texture::~Texture() {
    LOG_FUNCTION_CALL_TRACE("id {}", m_id);
    glDeleteTextures(1, &m_id);
}

/**
 * @brief Make this texture active and use it
 */
void GEM::Texture::activate() const {
    glActiveTexture(GL_TEXTURE0 + m_index);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

/* ------------------------------ private member functions ------------------------------ */
