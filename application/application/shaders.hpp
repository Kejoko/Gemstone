#pragma once

/**
 * @brief This file contains string literals of vertex shaders
 */

const char* vertexShaderSource =
#include "assets/shaders/literals/vertex.vert"
;

const char* strippedVertexShaderSource =
#include "assets/shaders/literals/stripped.vert"
;

/**
 * @brief This file contains string literals of fragment shaders
 */

const char* fragmentShaderSource = 
#include "assets/shaders/literals/fragment.frag"
;

const char* fragmentShader2Source = 
#include "assets/shaders/literals/fragment2.frag"
;

const char* objectLightingFragShaderSource =
#include "assets/shaders/literals/objectLighting.frag"
;

const char* lightLightingFragShaderSource =
#include "assets/shaders/literals/lightLighting.frag"
;