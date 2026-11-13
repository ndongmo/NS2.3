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

bool LightEditor::create(const std::string& path, unsigned int type)
{
	m_light.m_shape.initBox(0.0f, 0.0f, 400.0f, 400.0f);
	m_light.m_color = NS2::ColorRGBA8(255, 255, 255, 255);
	m_light.m_texture = m_parent.getResourceManager().getTexture(LIGHT_ASSET);
	m_light.setPath(path);
	return true;
}

void LightEditor::update()
{
	glm::vec2 size = m_parent.getScreenSize();
	m_tile.setPosX(0);
	m_tile.setPosY(0);
	m_tile.setTexWidth(size.x);
	m_tile.setTexHeight(size.y);
	m_tile.setTexture(m_parent.getResourceManager().getTexture(TILE_ASSET));

	m_light.m_shape.setCenter(m_parent.getScreenCenter());
	m_parent.getLevel().setAmbientColor(glm::vec3(0.1f));
}

void LightEditor::onDraw()
{
	glm::vec3 ambient = m_parent.getLevel().getAmbientColor();
	if (ImGui::ColorEdit3("Ambient", &ambient.x)) {
		m_parent.getLevel().setAmbientColor(ambient);
	}
	m_parent.drawColor(m_light.m_color);
	glm::vec2 dims = m_light.m_shape.getDims();
	if (ImGui::DragFloat2("Size", &dims.x, 1.0f, 1.0f, 0.0f, "%.2f")) {
		m_light.m_shape.setWidth(dims.x);
		m_light.m_shape.setHeight(dims.y);
		m_parent.setNeedToSave(true);
	}
	m_parent.drawTexture(m_light.m_texture);
	std::string previewText = TypeHelper::getShapeType(m_light.m_shape.getType());
	if (ImGui::BeginCombo("Shape", previewText.c_str())) {
		for (const auto& [typeKey, typeName] : TypeHelper::shapeTypes) {
			Shape2DType type = static_cast<Shape2DType>(typeKey);
			if (type != Shape2DType::BOX && type != Shape2DType::ROTATED_BOX) continue;
			bool isSelected = (m_light.m_shape.getType() == type);
			glm::vec2 pos = m_light.m_shape.getPos();
			if (ImGui::Selectable(typeName.c_str(), isSelected) && !isSelected) {
				m_light.m_shape.init(type, pos.x, pos.y, 
					m_light.m_shape.getWidth(), m_light.m_shape.getHeight());
				m_parent.setNeedToSave(true);
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
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

void LightEditor::getInfo(std::string& name, glm::vec2& pos, glm::vec2& size, glm::vec2& scale, float& angle)
{
	name = "LIGHT";
	pos = m_light.m_shape.getPos();
	size = m_light.m_shape.getDims();
	angle = m_light.m_shape.getAngle();
}
