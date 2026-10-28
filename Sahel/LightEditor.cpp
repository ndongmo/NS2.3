#include "LightEditor.h"
#include "EditorGui.h"
#include "PathHelper.h"
#include "ObjectManager.h"
#include "LightRenderer.h"
#include "TileRenderer.h"
#include "MathUtils.h"
#include "Level.h"

LightEditor::LightEditor(EditorGui& parent) :
	AbstractEditor(parent, m_light)
{}

void LightEditor::create(const std::string& path)
{
	m_light.m_destRect.w = m_light.m_destRect.z = 400.0f;
	m_light.m_color = NS2::ColorRGBA8(255, 255, 255, 255);
	m_light.m_texture = m_parent.getResourceManager().getTexture(LIGHT_ASSET);
	m_light.setPath(path);
}

void LightEditor::update()
{
	const glm::vec2& size = m_parent.getScreenSize();
	const glm::vec2& center = m_parent.getScreenCenter();
	m_tile.setPosX(0);
	m_tile.setPosY(0);
	m_tile.setTexWidth(size.x);
	m_tile.setTexHeight(size.y);
	m_tile.setTexture(m_parent.getResourceManager().getTexture(TILE_ASSET));

	m_light.m_destRect.x = center.x - (m_light.m_destRect.z / 2.0f);
	m_light.m_destRect.y = center.y - (m_light.m_destRect.w / 2.0f);
	m_parent.getLevel().setAmbientColor(glm::vec3(0.1f));
}

void LightEditor::onDraw()
{
	drawColor(m_light.m_color);
	if (ImGui::DragFloat2("Size", &m_light.m_destRect.z, 1.0f, 1.0f, 0.0f, "%.2f")) {
		m_parent.setNeedToSave(true);
	}
	drawTexture(m_light.m_texture);
}

std::string LightEditor::getName() const
{
	return "Light Editor";
}

bool LightEditor::load(const std::string& path)
{
	return m_parent.getObjectManager().loadUnique(path, m_light);
}

bool LightEditor::save()
{
	return m_parent.getObjectManager().save(m_light.getPath(), m_light);
}

void LightEditor::draw(LightRenderer& lightRenderer, const glm::vec4& box)
{
	lightRenderer.render(m_light);
}

void LightEditor::draw(TileRenderer& tileRenderer, const glm::vec4& box)
{
	tileRenderer.render(m_tile);
}

void LightEditor::drawDebug(LightRenderer& lightRenderer, const glm::vec4& box)
{
	lightRenderer.renderDebug(m_light);
}

void LightEditor::getInfo(std::string& name, glm::vec2& pos, glm::vec2& size,
	float& angle, float& scale)
{
	name = "LIGHT";
	pos.x = m_light.m_destRect.x;
	pos.y = m_light.m_destRect.y;
	size.x = m_light.m_destRect.z;
	size.y = m_light.m_destRect.w;
	angle = 0.0f;
	scale = 1.0f;
}
