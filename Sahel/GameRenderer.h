/*!
* \file GameRenderer.h
* \brief Aims to render all game stuff.
* \author Ndongmo Silatsa Fabrice
* \date 21-05-2017
* \version 2.0
*/

#pragma once

#include "EntityRenderer.h"
#include "TileRenderer.h"
#include "LightRenderer.h"

#include <GLSLProgram.h>
#include <SpriteBatch.h>
#include <DebugRenderer.h>

class Level;
class PhysicsEngine;
class EditorGui;

namespace NS2 {
	class Camera;
}

class GameRenderer
{
public:
	GameRenderer(PhysicsEngine& physicsEngine);

	/*!
	* \brief Initializes all stuff for rendering.
	* \param screenWidth : screen width
	* \param screenHeight : screen height
	* \param debug : in debug mode
	*/
	void init(float screenWidth, float screenHeight, bool debug = true);

	/*!
	* \brief renders the given level in edition mode.
	* \param camera : game camera
	* \param editor : gui editor
	* \param level : level
	*/
	void render(NS2::Camera& camera, Level& level);
	void render(NS2::Camera& camera, Level& level, EditorGui& editor);

	/*!
	* \brief disposes of all render stuff.
	*/
	void dispose();

	/* GETTERS */
	bool isDebugging() const { return m_debug; }

	/* SETTERS */
	void setDebug(bool debug) { m_debug = debug; }
	void clearAmbientColor(const glm::vec4& color);
	void setLineWidth(float lineWidth) { m_lineWidth = lineWidth; }

private:
	bool m_debug = false;						/* In debug mode */
	float m_lineWidth = 0.0f;					/* Width of the line in debug mode */
	glm::vec4 m_screenBox = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

	NS2::GLSLProgram m_textureShader;			/* The texture shader program */
	NS2::SpriteBatch m_spriteBatch;				/* Draws game stuff */
	NS2::DebugRenderer m_debugRenderer;			/* Draws debugging shapes */

	EntityRenderer m_entityRenderer;			/* Entities renderer */
	TileRenderer m_tileRenderer;				/* Tiles renderer */
	LightRenderer m_lightRenderer;

	PhysicsEngine& m_physicEngine;				/* Physics engine */
};