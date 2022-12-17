#include "gemstone/mesh/Mesh.hpp"

#include <string>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "util/logger/Logger.hpp"

#include "gemstone/mesh/logger.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the Mesh class uses
 */
const std::string GEM::Mesh::LOGGER_NAME = MESH_LOGGER_NAME;

/* ------------------------------ private static variables ------------------------------ */

/* ------------------------------ public static functions ------------------------------ */

/* ------------------------------ private static functions ------------------------------ */

/**
 * @brief Load the vertices for this mesh
 * 
 * @return std::vector<float> The vector of vertices making up this mesh
 */
std::vector<float> GEM::Mesh::loadVertices() {
    LOG_FUNCTION_ENTRY_TRACE("{}", nullptr);

    return {
        // position             // color            // texture coord
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,   0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 0.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,

         0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 1.0f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 1.0f,   0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 1.0f,   1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 1.0f,   1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 1.0f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f
    };
}

/**
 * @brief Create a vertex array object to store all of our vertex attribute's informations
 * 
 * @return uint32_t The id of the VAO
 */
uint32_t GEM::Mesh::createVertexArrayObject() {
    LOG_FUNCTION_ENTRY_TRACE("{}", nullptr);

    uint32_t vertexArrayObjectID;
    glGenVertexArrays(1, &vertexArrayObjectID);
    glBindVertexArray(vertexArrayObjectID);

    return vertexArrayObjectID;
}

/**
 * @brief Create the vertex buffer object and bind it so we can configure it with
 * subsequent calls to GL_ARRAY_BUFFER
 * 
 * @param vertices The vector of vertices, color values, and texture coords
 * @return uint32_t The id of the VBO
 */
uint32_t GEM::Mesh::createVertexBufferObject(const std::vector<float>& vertices) {
    LOG_FUNCTION_ENTRY_TRACE("vertices size {}", vertices.size());

    uint32_t vertexBufferObjectID;
    glGenBuffers(1, &vertexBufferObjectID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectID);

    // Copy the vertices into the currently bound vertex buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    return vertexBufferObjectID;
}

/**
 * @brief Create the EBO similarly to creating a VBO
 * 
 * @param vertices The vector of vertices, color values, and texture coords
 * @return uint32_t The id of the EBO
 */
uint32_t GEM::Mesh::createElementBufferObject(const std::vector<float>& vertices) {
    LOG_FUNCTION_ENTRY_TRACE("vertices size {}", vertices.size());

    uint32_t elementBufferObjectID;
    glGenBuffers(1, &elementBufferObjectID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObjectID);

    // Copy the vertices into the buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    return elementBufferObjectID;
}

/**
 * @brief Configure the vertex attribute pointers for position, color, and texture coordinate information
 */
void GEM::Mesh::configureVertexAttributePointers() {
    LOG_FUNCTION_ENTRY_TRACE("{}", nullptr);

    // Tell OpenGL how it should interpret the vertex data
    const int p_vertexPositionAttribute = 0;
    glVertexAttribPointer(
        p_vertexPositionAttribute,  // The vertex attribute we want to configure (0 because we used 'location = 0' in our vertex shader, we want to pass this to our vertex shader)
        3,                          // The size of the vertex attribute (vec3 has 3 values)
        GL_FLOAT,                   // Data type (vec<N> in GLSL consists of floating point values)
        GL_FALSE,                   // To normalize or not to normalize (when using int types this sets the data to -1, 0, or 1 upon conversion to float)
        8 * sizeof(float),          // The stride length, the space between consecutive vertex attributes (each vertex and color is 3 floats so the starts of each vertex differs by 6 floats)
        static_cast<void*>(0)       // The offset of where the position data begins in the buffer (0 because position data starts at the beggining of the buffer)
    );
    glEnableVertexAttribArray(p_vertexPositionAttribute);

    // Configure the color vertex attribute the same way we did the positions
    const int p_vertexColorAttribute = 1;
    glVertexAttribPointer(
        p_vertexColorAttribute,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),          // Stride length of 8 becasue 3 position + 3 color + 2 texture
        (void*)(3 * sizeof(float))  // Offset 3 floats into the array because the first 3 values are position data
    );
    glEnableVertexAttribArray(p_vertexColorAttribute);

    // Configure the texture vertex attribute like the color and positions
    const int p_vertexTextureAttribute = 2;
    glVertexAttribPointer(
        p_vertexTextureAttribute,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),          // Stride length of 8 becasue 3 position + 3 color + 2 texture
        (void*)(6 * sizeof(float))  // Offset 6 floats into the array because 3 position and 3 color preceeding this
    );
    glEnableVertexAttribArray(p_vertexTextureAttribute);
}

/* ------------------------------ public member functions ------------------------------ */

/**
 * @brief Generate and configure the VAO, the VBO, the EBO, and attribute pointers to be
 * stored for later use
 */
GEM::Mesh::Mesh() :
    m_vertices(GEM::Mesh::loadVertices()),
    m_vertexArrayObjectID(GEM::Mesh::createVertexArrayObject()),
    m_vertexBufferObjectID(GEM::Mesh::createVertexBufferObject(m_vertices)),
    m_elementBufferObjectID(GEM::Mesh::createElementBufferObject(m_vertices))
{
    GEM::Mesh::configureVertexAttributePointers();

    // Unbind our VAO, VBO, and EBO so the next objects loaded in can handle it
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GEM::Mesh::~Mesh() {
    LOG_FUNCTION_CALL_TRACE("vertices size {}, VAO id {}, VBO id {}, EBO id {}", m_vertices.size(), m_vertexArrayObjectID, m_vertexBufferObjectID, m_elementBufferObjectID);
    glDeleteVertexArrays(1, &m_vertexArrayObjectID);
    glDeleteBuffers(1, &m_vertexBufferObjectID);
    glDeleteBuffers(1, &m_elementBufferObjectID);
}

/**
 * @brief Bind the corresponding VAO, draw it, then unbind the VAO
 */
void GEM::Mesh::draw() {
    glBindVertexArray(m_vertexArrayObjectID);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}

/* ------------------------------ private member functions ------------------------------ */
