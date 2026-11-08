/*!
* \file Renderer.h
* \brief Renderer class. 
* It aims to render all games stuff through the GameRender class
* \author Ndongmo Silatsa Fabrice
* \date 12-10-2017
* \version 2.0
*/

#pragma once

#include <CoreTypes.h>

class Level;

namespace NS2 {
	class SpriteBatch;
	class DebugRenderer;
}

class Renderer
{
public:
	Renderer(NS2::SpriteBatch& spriteBatch, NS2::DebugRenderer& debugRenderer);

	/*!
	* \brief renders the stuffs contained in the given box.
	* \param level : current level
	* \param box : screen box
	*/
	virtual void render(Level& level, const glm::vec4& box) {}

	/*!
	* \brief renders the stuffs contained in the given box in debug mode.
	* \param debugRenderer : debug renderer
	* \param level : current level
	* \param box : screen box
	*/
	virtual void renderDebug(Level& level, const glm::vec4& box) {}

	NS2::SpriteBatch& getSpriteBatch() { return m_spriteBatch; }
	NS2::DebugRenderer& getDebugRenderer() { return m_debugRenderer; }
	const NS2::ColorRGBA8& getDebugColor() const { return m_debugColor; }

protected:
	NS2::ColorRGBA8 m_debugColor;				/* Color to draw in debug mode */
	NS2::SpriteBatch& m_spriteBatch;			/* Draws game stuff */
	NS2::DebugRenderer& m_debugRenderer;		/* Draws game stuff in debug mode */
};

