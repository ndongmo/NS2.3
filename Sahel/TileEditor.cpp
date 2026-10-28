#include "TileEditor.h"
#include "EditorGui.h"
#include "PathHelper.h"
#include "ObjectManager.h"
#include "TileRenderer.h"

TileEditor::TileEditor(EditorGui& parent) : 
	AbstractEditor(parent, m_tile)
{}

void TileEditor::create(const std::string& path)
{
	m_tile.setTexture(m_parent.getResourceManager().getTexture(TILE_ASSET));
	m_tile.setPath(path);
}

void TileEditor::update()
{
	const glm::vec2& center = m_parent.getScreenCenter();
	int x = (int)((center.x - (TILESIZE / 2.0f)) / TILESIZE);
	int y = (int)((center.y - (TILESIZE / 2.0f)) / TILESIZE);
	m_tile.setPosX(x);
	m_tile.setPosY(y);
}

void TileEditor::onDraw()
{
	drawColor(m_tile.m_color);
	drawEnum(m_tile.m_material, TypeHelper::materialTypes, "Material");
	drawTexture(m_tile.m_texture);
}

std::string TileEditor::getName() const
{
	return "Tile Editor";
}

bool TileEditor::load(const std::string& path)
{
	return m_parent.getObjectManager().loadUnique(path, m_tile);
}

bool TileEditor::save()
{
	return m_parent.getObjectManager().save(m_tile.getPath(), m_tile);
}

void TileEditor::draw(TileRenderer& tileRenderer, const glm::vec4& box)
{
	tileRenderer.render(m_tile);
}

void TileEditor::drawDebug(TileRenderer& tileRenderer, const glm::vec4& box)
{
	tileRenderer.renderDebug(m_tile);
}

void TileEditor::getInfo(std::string& name, glm::vec2& pos, glm::vec2& size,
	float& angle, float& scale)
{
	name = std::format("TILE [{}, {}]", m_tile.getPosX(), m_tile.getPosY());
	pos.x = m_tile.getX();
	pos.y = m_tile.getY();
	size.x = size.y = TILESIZE;
	angle = 0.0f;
	scale = 1.0f;
}
