#pragma once

#include <string>

#include <glad/glad.h>

namespace GEM {
    class Texture;
}

class GEM::Texture {
public: // public static variables
    static const std::string LOGGER_NAME;

public: // public member functions
    Texture(const std::string& filename, const uint32_t index);
    ~Texture();

    void activate() const;

    uint32_t getID() const { return m_id; }
    uint32_t getIndex() const { return m_index; }

private: // private static functions
    static GLenum getInputFormat(const std::string& filename);

    static uint32_t createTexture(const std::string& filename);

private: // private member variables
    const uint32_t m_id;
    const uint32_t m_index;
};