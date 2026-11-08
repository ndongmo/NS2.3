#include "PartEditor.h"
#include "EditorGui.h"
#include "PathHelper.h"
#include "ObjectManager.h"
#include "TileRenderer.h"
#include "EntityRenderer.h"

constexpr float POINT_RADIUS = 10.0f;

PartEditor::PartEditor(EditorGui& parent) :
	AbstractEditor(parent, m_part)
{}

void PartEditor::create(const std::string& path, unsigned int type)
{
	m_part.init(static_cast<PartType>(type), m_parent.getResourceManager().getTexture(TILE_ASSET));
	m_part.setPath(path);
}

void PartEditor::update()
{
	m_part.setCenter(m_parent.getScreenCenter());
	m_parent.setDebug(DebugType::DRAW_PART, true);
	m_parent.setDebug(DebugType::COLL_PART, true);
}

void PartEditor::onDraw()
{
	/* Creation Basics Group */
	if (ImGui::BeginChild("Basics", ImVec2(0.0f, 50.0f), ImGuiChildFlags_Borders)) {
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().FramePadding.x);
		ImGui::Text("Basics");
		ImGui::Separator();
		const ImVec2 dims = ImVec2(80.0f, 0.0f);
		if (ImGui::Selectable("Collision", m_part.m_collide, 0, dims)) {
			m_part.m_collide = !m_part.m_collide;
			m_parent.setNeedToSave(true);
		}
		ImGui::SameLine();
		if (ImGui::Selectable("Scalable", m_part.m_scalable, 0, dims)) {
			m_part.m_scalable = !m_part.m_scalable;
			m_parent.setNeedToSave(true);
		}
		ImGui::SameLine();
		if (ImGui::Selectable("Hidden", m_part.m_hidden, 0, dims)) {
			m_part.m_hidden = !m_part.m_hidden;
			m_parent.setNeedToSave(true);
		}
	}
	ImGui::EndChild();
	/* Creation Sprite Group */
	if (ImGui::BeginChild("Sprite", ImVec2(0.0f, 170.0f), ImGuiChildFlags_Borders)) {
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().FramePadding.x);
		ImGui::Text("Sprite");
		ImGui::Separator();
		const ImVec2 widgetDims = ImVec2(120.0f, 0.0f);
		if (ImGui::Selectable("Inverse X axis", m_part.m_sprite.getInvX(), 0, widgetDims)) {
			m_part.m_sprite.inverseX(!m_part.m_sprite.getInvX());
			m_parent.setNeedToSave(true);
		}
		ImGui::SameLine();
		if (ImGui::Selectable("Inverse Y axis", m_part.m_sprite.getInvY(), 0, widgetDims)) {
			m_part.m_sprite.inverseY(!m_part.m_sprite.getInvY());
			m_parent.setNeedToSave(true);
		}
		ImGui::PushItemWidth(80.0f);
		glm::ivec2 uv{ m_part.m_sprite.getUvRect().z, m_part.m_sprite.getUvRect().w };
		if (ImGui::DragInt2("UV [W, H]", &uv.x, 1, 1, 10)) {
			m_part.m_sprite.setUvRect(static_cast<float>(uv.x), static_cast<float>(uv.y));
			m_parent.setNeedToSave(true);
		}
		ImGui::PopItemWidth();
		ImGui::PushItemWidth(80.0f);
		glm::ivec2 dims{ m_part.m_sprite.getDimsX(), m_part.m_sprite.getDimsY()};
		if (ImGui::DragInt2("##Multi [W, H]", &dims.x, 1, 1, 10)) {
			m_part.m_sprite.setDims(dims.x, dims.y);
			m_parent.setNeedToSave(true);
		}
		ImGui::PopItemWidth();
		ImGui::PushItemWidth(40.0f);
		ImGui::SameLine();
		int index = m_part.m_sprite.getIndex();
		if (ImGui::SliderInt("Multi [W, H] [Index]", &index, 0, m_part.m_sprite.getDim() - 1)) {
			m_part.m_sprite.setIndex(index);
			m_parent.setNeedToSave(true);
		}
		ImGui::PopItemWidth();
		m_parent.drawColor(m_part.m_sprite.color);
		m_parent.drawEnum(m_part.m_sprite.depth, TypeHelper::depthTypes, "Depth");
		if (m_parent.drawTexture(m_part.m_sprite.texture) && m_part.m_sprite.texture != nullptr) {
			m_part.m_drawShape.setWidth((float)m_part.m_sprite.texture->width);
			m_part.m_drawShape.setHeight((float)m_part.m_sprite.texture->height);
			Shape2DType tmp = m_part.m_shape.getType();
			m_part.m_shape = m_part.m_drawShape;
			m_part.m_shape.setType(tmp);
		}
	}
	ImGui::EndChild();
	/* Creation Drawing Shapes Group */
	if (ImGui::BeginChild("Drawing Shape", ImVec2(0.0f, 80.0f), ImGuiChildFlags_Borders)) {
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().FramePadding.x);
		ImGui::Text("Drawing Shape");
		ImGui::Separator();
		glm::vec2 pos = m_part.m_drawShape.getPos();
		glm::vec2 dims = m_part.m_drawShape.getDims();
		if (ImGui::DragFloat2("[X, Y]", &pos.x, 1.0f, 0.0f, 0.0f, "%.f")) {
			m_part.m_drawShape.setPos(pos);
			m_parent.setNeedToSave(true);
		}
		if (ImGui::DragFloat2("[W, H]", &dims.x, 1.0f, 0.0f, 0.0f, "%.f")) {
			m_part.m_drawShape.setWidth(dims.x);
			m_part.m_drawShape.setHeight(dims.y);
			m_parent.setNeedToSave(true);
		}
	}
	ImGui::EndChild();
	/* Creation Collision Shapes Group */
	if (ImGui::BeginChild("Collision Shape", ImVec2(0.0f, 262.0f), ImGuiChildFlags_Borders)) {
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().FramePadding.x);
		ImGui::Text("Collision Shape");
		ImGui::Separator();
		glm::vec2 pos = m_part.m_shape.getPos();
		glm::vec2 dims = m_part.m_shape.getDims();
		if (ImGui::DragFloat2("[X, Y]", &pos.x, 1.0f, 0.0f, 0.0f, "%.f")) {
			m_part.m_shape.setPos(pos);
			m_parent.setNeedToSave(true);
		}
		if (ImGui::DragFloat2("[W, H]", &dims.x, 1.0f, 0.0f, 0.0f, "%.f")) {
			if (dims.x != m_part.m_shape.getDimX()) {
				m_part.m_shape.setWidth(dims.x);
			} else {
				m_part.m_shape.setHeight(dims.y);
			}
			m_parent.setNeedToSave(true);
		}
		Shape2DType type = m_part.m_shape.getType();
		if (m_parent.drawEnum(type, TypeHelper::shapeTypes, "Shape")) {
			m_part.m_shape.setType(type);
			if (type == Shape2DType::BOX || type == Shape2DType::ROTATED_BOX) {
				m_part.m_drawShape.setType(type);
			}
		}
		if (ImGui::BeginChild("Customize points", ImVec2(0.0f, 135.0f), ImGuiChildFlags_Borders)) {
			bool isPolygon = m_part.m_shape.m_type == Shape2DType::POLYGON;
			bool customizable = isPolygon || m_part.m_shape.m_type == Shape2DType::TRIANGLE;
			for (unsigned int i = 0; i < m_part.m_shape.m_points.size(); ++i) {
				ImGui::PushID(static_cast<int>(i));
				std::string label = "Point #" + std::to_string(i);
				if (!customizable) ImGui::BeginDisabled();
				glm::vec2 point = m_part.m_shape.m_points[i];
				if (ImGui::DragFloat2(label.c_str(), &point.x, 1.0f, 0.0f, 0.0f, "%.f")) {
					m_part.m_shape.setPoint(i, point);
					m_parent.setNeedToSave(true);
				}
				if (!customizable) ImGui::EndDisabled();
				if (isPolygon && m_part.m_shape.m_points.size() > POLY_MIN_POINTS) {
					ImGui::SameLine();
					if (ImGui::Button("-")) {
						m_part.m_shape.deletePoint(i);
						m_parent.setNeedToSave(true);
						--i; // Adjust index since we shunted elements backwards
					}
				}
				ImGui::PopID(); // Always close your PushID block
			}
			if (isPolygon && m_part.m_shape.m_points.size() < POLY_MAX_POINTS) {
				ImGui::Separator();
				if (ImGui::Button("Add New Point")) {
					glm::vec2 point = m_part.m_shape.m_points.back() + glm::vec2(50.0f, 50.0f);
					m_part.m_shape.addPoint(point.x, point.y);
					m_parent.setNeedToSave(true);
				}
			}
		}
		ImGui::EndChild();
		if (ImGui::Button("Fit Shapes")) {
			Shape2DType tmp = m_part.m_shape.getType();
			m_part.m_shape = m_part.m_drawShape;
			m_part.m_shape.setType(tmp);
			m_parent.setNeedToSave(true);
		}
		ImGui::SameLine();
		if (ImGui::Button("Equilibrate Shapes")) {
			m_part.m_shape.setCenter(m_part.m_drawShape.getCenter());
			m_parent.setNeedToSave(true);
		}
	}
	ImGui::EndChild();
}

std::string PartEditor::getName() const
{
	return "Part Editor";
}

bool PartEditor::load(const std::string& path)
{
	return m_parent.getObjectManager().loadUnique(path, m_part);
}

bool PartEditor::save()
{
	return m_parent.getObjectManager().save(m_part.getPath(), m_part);
}

void PartEditor::draw(EntityRenderer& renderer, const glm::vec4& box)
{
	m_part.draw(renderer.getSpriteBatch());
}

void PartEditor::drawDebug(TileRenderer& renderer, const glm::vec4& box)
{
	if (m_parent.debug(DebugType::DRAW_PART)) {
		m_part.getDrawShape().drawDebug(renderer.getDebugRenderer(), renderer.getDebugColor());
	}
}

void PartEditor::drawDebug(EntityRenderer& renderer, const glm::vec4& box)
{
	if (m_parent.debug(DebugType::COLL_PART)) {
		m_part.getShape().drawDebug(renderer.getDebugRenderer(), renderer.getDebugColor());
		if (m_part.m_shape.getType() != Shape2DType::CIRCLE) {
			for (const auto& pt : m_part.m_shape.getPoints()) {
				renderer.getDebugRenderer().drawCircle(pt, renderer.getDebugColor(), POINT_RADIUS);
			}
		}
	}
}

void PartEditor::getInfo(std::string& name, glm::vec2& pos, glm::vec2& size, glm::vec2& scale, float& angle)
{
	name = "Body Part";
	angle = m_part.getAngle();
	pos = m_part.getPos();
	size = m_part.getDrawShape().getDims();
	scale = m_part.getDrawShape().getScale();
}

void PartEditor::select(glm::vec2 point)
{
	m_selected = -1;

	for (unsigned int i = 0; i < m_part.m_shape.m_points.size(); i++) {
		if (MathUtils::isPointInsideCircle(point, m_part.m_shape.m_points[i], POINT_RADIUS)) {
			m_selected = i;
			break;
		}
	}
}

void PartEditor::drag(glm::vec2 delta)
{
	if (m_selected >= 0 && m_selected < m_part.m_shape.m_points.size()) {
		m_part.m_shape.setPoint(m_selected, m_part.m_shape.m_points[m_selected] + delta);
	}
}

void PartEditor::dragOver(glm::vec2 point)
{
	m_selected = -1;
}
