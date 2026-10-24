/**
* @file Light.h
* @brief Structural definition for high-performance multi-type dynamic light objects.
* @author Ndongmo Silatsa Fabrice
*/

#pragma once

#include <glm/glm.hpp>

/**
* @brief Strongly typed enumeration representing the structural layout logic of the shader pass.
*/
enum LightType
{
    Point,       // Standard radial light (e.g., torch, lightbulb)
    Directional, // Global light source with no position (e.g., sun, moon)
    Cone         // Flashlight / spotlight effects
};

/**
 * @brief Lighting struct.
 */
struct Light
{
    LightType type = LightType::Point;
	int index = -1;			                // Unique light index
    std::string path;			            // File path

    // Transform
    glm::vec2 position{ 0.0f, 0.0f };       // 2D World or Screen Space position
    glm::vec2 direction{ 1.0f, 0.0f };      // Direction vector (Used for Cone lights)
    float height = 0.05f;                   // Z-distance above the 2D plane (Prevents division by zero in shading)

    // Color & Appearance
    glm::vec3 color{ 1.0f, 1.0f, 1.0f };    // RGB Intensity
    float intensity = 1.0f;                 // Brighter multiplier
    float radius = 500.0f;                  // Max distance this light can reach in pixels/units

    // Cone Light specific
    float innerAngle = 0.5f; // Radians
    float outerAngle = 0.7f; // Radians (For edge smoothing)

    bool active = true;
};
