/*!
* \file TileRenderer.h
* \brief Tile Renderer class.
* It aims to render tiles.
* \author Ndongmo Silatsa Fabrice
* \date 12-10-2017
* \version 2.0
*/

#pragma once

#include "Renderer.h"

class Level;
class Tile;

namespace NS2 {
	class SpriteBatch;
	class DebugRenderer;
}

class TileRenderer : public Renderer
{
public:
	TileRenderer(NS2::SpriteBatch& spriteBatch, NS2::DebugRenderer& debugRenderer);
	virtual void render(Level& level, const glm::vec4& box)override;
	virtual void renderDebug(Level& level, const glm::vec4& box)override;
	void render(const Tile& tile);
	void renderDebug(const Tile& tile);

private:
	unsigned int m_lastFrameDrawn = 0;		/* Last frame drawn */
};

