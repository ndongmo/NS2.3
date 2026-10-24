#include "Tile.h"

Tile::Tile() : m_material(MaterialType::STONE) 
{
	m_color = NS2::ColorRGBA8(255, 255, 255, 255);
	m_uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	m_destRect = glm::vec4(0.0f, 0.0f, TILESIZE, TILESIZE);
}

void Tile::draw(NS2::SpriteBatch& spriteBatch) const
{
	if (m_texture != nullptr) {
		spriteBatch.draw(m_destRect, m_uvRect, 0, m_texture->id, m_color);
	}
}

void Tile::drawDebug(NS2::DebugRenderer& debugRenderer, const NS2::ColorRGBA8& color) const
{
	debugRenderer.drawBox(m_destRect, color);
}