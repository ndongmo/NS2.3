#include "TileRenderer.h"
#include "Level.h"
#include "Tile.h"
#include "TileGrid.h"
#include <SpriteBatch.h>
#include <DebugRenderer.h>

TileRenderer::TileRenderer(NS2::SpriteBatch& spriteBatch, NS2::DebugRenderer& debugRenderer)
	: Renderer(spriteBatch, debugRenderer)
{
	m_debugColor.setColor(255, 0, 0, 255);
}

void TileRenderer::render(Level& level, const glm::vec4& box)
{
	const TileGrid& tileGrid = level.getTileGrid();
	int startX = static_cast<int>(box.x / TILESIZE);
	int endX = static_cast<int>((box.x + box.z) / TILESIZE);
	int startY = static_cast<int>(box.y / TILESIZE);
	int endY = static_cast<int>((box.y + box.w) / TILESIZE);

	m_lastFrameDrawn++; // Increment the last frame drawn counter

	for (int x = startX; x <= endX; ++x) {
		for (int y = startY; y <= endY; ++y) {
			const Tile* tile = tileGrid.getTile(x, y);
			if (tile && tile->getLastFrameDrawn() != m_lastFrameDrawn) {
				tile->draw(m_spriteBatch);
				tile->setLastFrameDrawn(m_lastFrameDrawn);
			}
		}
	}
}

void TileRenderer::renderDebug(Level& level, const glm::vec4& box)
{
	const TileGrid& tileGrid = level.getTileGrid();
	int startX = static_cast<int>(box.x / TILESIZE);
	int endX = static_cast<int>((box.x + box.z) / TILESIZE);
	int startY = static_cast<int>(box.y / TILESIZE);
	int endY = static_cast<int>((box.y + box.w) / TILESIZE);

	m_lastFrameDrawn++; // Increment the last frame drawn counter

	for (int x = startX; x <= endX; ++x) {
		for (int y = startY; y <= endY; ++y) {
			const Tile* tile = tileGrid.getTile(x, y);
			if (tile && tile->getLastFrameDrawn() != m_lastFrameDrawn) {
				tile->drawDebug(m_debugRenderer, m_debugColor);
				tile->setLastFrameDrawn(m_lastFrameDrawn);
			}
		}
	}
}

void TileRenderer::render(const Tile& tile)
{
	tile.draw(m_spriteBatch);
}

void TileRenderer::renderDebug(const Tile & tile)
{
	tile.drawDebug(m_debugRenderer, m_debugColor);
}
