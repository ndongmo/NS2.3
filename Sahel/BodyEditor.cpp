#include "BodyEditor.h"
#include "EditorGui.h"
#include "PathHelper.h"
#include "ObjectManager.h"
#include "TileRenderer.h"
#include "EntityRenderer.h"

BodyEditor::BodyEditor(EditorGui& parent) :
	AbstractEditor(parent, m_body)
{}

void BodyEditor::create(const std::string& path, unsigned int type)
{
	size_t partCount = TypeHelper::bodyParts[type].size();
	m_body.m_parts.clear();
	m_body.m_parts.resize(partCount);
	m_body.m_type = static_cast<BodyType>(type);
	
	for (const auto& part : TypeHelper::bodyParts[type]) {
		m_body.m_parts[part.partIndex].init(part.partType, m_parent.getResourceManager().getTexture(TILE_ASSET));
	}
	m_body.updateShape();
	m_body.setPath(path);
}

void BodyEditor::update()
{
	const glm::vec2& center = m_parent.getScreenCenter();
	//m_part.setCenter(center.x, center.y);
	m_parent.setDebug(DebugType::DRAW_PART, true);
	m_parent.setDebug(DebugType::COLL_PART, true);
}

void BodyEditor::onDraw()
{
	/* Creation Basic Group */
	if (ImGui::BeginChild("Basic", ImVec2(0.0f, 105.0f), ImGuiChildFlags_Borders)) {
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().FramePadding.x);
		ImGui::Text("Basic");
		ImGui::Separator();
		ImGui::DragFloat("Angular Speed", &m_body.m_angularSpeed, 0.1f, 0.0f, 00.0f, "%.2f");
		ImGui::DragFloat("Mass", &m_body.m_mass, 1.0f, 1.0f, 0.0f, "%.2f");
		m_parent.drawEnum(m_body.m_material, TypeHelper::materialTypes, "Material");
	}
	ImGui::EndChild();
	/* Creation Parts Group */
	if (ImGui::BeginChild("Parts", ImVec2(0.0f, 200.0f), ImGuiChildFlags_Borders)) {
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().FramePadding.x);
		ImGui::Text("Parts");
		ImGui::Separator();
		if (ImGui::BeginListBox("##HierarchyList", ImVec2(140.0f, -1.0f))) {
			for (unsigned int i = 0; i < TypeHelper::bodyParts[m_body.m_type].size(); ++i) {
				BodyPart& part = TypeHelper::bodyParts[m_body.m_type][i];
				bool isSelected = (m_selectedPartType == part.partType);
				if (ImGui::Selectable(part.name.c_str(), isSelected) && !isSelected) {
					m_selectedPartObject = -1;
					m_selectedPartType = part.partType;
				}
			}
			ImGui::EndListBox();
		}
		if (m_selectedPartType >= 0) {
			ImGui::SameLine();
			if (ImGui::BeginChild("Part", ImVec2(-1.0f, -1.0f), ImGuiChildFlags_Borders)) {
				const BodyPart& bodypart = TypeHelper::getBodyPart(m_body.m_type, m_selectedPartType);
				Part& part = m_body.m_parts[bodypart.partIndex];
				glm::vec2 scale = part.getScale();
				glm::vec2 pos = part.getPos();
				float angle = RadToDeg(part.getAngle());
				if (ImGui::SliderFloat("Angle", &angle, 0.0f, 360.0f, "%.f")) {
					part.setAngle(DegToRad(angle));
				}
				if (ImGui::DragFloat2("Pos", &pos.x, 1.0f, 0.0f, 0.0f, "%.f")) {
					part.setPos(pos);
				}
				if (ImGui::DragFloat2("Scale", &scale.x, 0.1f, 0.1f, 10.0f, "%.1f")) {
					scale = scale / part.getScale();
					part.scale(scale);
				}
				const auto& partList = m_parent.getObjects(GameType::TYPE_PART);
				std::string previewText = "- Select Part -";
				if (m_selectedPartObject >= 0 && m_selectedPartObject < (int)partList.size()) {
					previewText = PathHelper::getFileName(partList[m_selectedPartObject]);
				}
				if (ImGui::BeginCombo("Part", previewText.c_str())) {
					for (int i = 0; i < (int)partList.size(); i++) {
						std::string listLabel = PathHelper::getFileName(partList[i]);
						if (listLabel.starts_with(m_parent.getNamePrefix(GameType::TYPE_PART, m_selectedPartType))) {
							bool isSelected = (m_selectedPartObject == i);
							if (ImGui::Selectable(listLabel.c_str(), isSelected) && !isSelected) {
								m_selectedPartObject = i;
								m_parent.getObjectManager().loadUnique(partList[i], part);
								part.setPos(m_parent.getScreenCenter());
							}
						}
					}
					ImGui::EndCombo();
				}
			}
			ImGui::EndChild();
		}
	}
	ImGui::EndChild();
}

std::string BodyEditor::getName() const
{
	return "Part Editor";
}

bool BodyEditor::load(const std::string& path)
{
	return m_parent.getObjectManager().loadUnique(path, m_body);
}

bool BodyEditor::save()
{
	return m_parent.getObjectManager().save(m_body.getPath(), m_body);
}

void BodyEditor::draw(EntityRenderer& renderer, const glm::vec4& box)
{
	m_body.draw(renderer.getSpriteBatch());
}

void BodyEditor::drawDebug(TileRenderer& renderer, const glm::vec4& box)
{
	if (m_parent.debug(DebugType::DRAW_PART)) {
		for (const auto& part : m_body.m_parts) {
			part.getDrawShape().drawDebug(renderer.getDebugRenderer(), renderer.getDebugColor());
		}
	}
}

void BodyEditor::drawDebug(EntityRenderer& renderer, const glm::vec4& box)
{
	if (m_parent.debug(DebugType::BODY)) {
		m_body.drawDebug(renderer.getDebugRenderer(), renderer.getDebugColor());
	}
	if (m_parent.debug(DebugType::COLL_PART)) {
		for (const auto& part : m_body.m_parts) {
			part.getShape().drawDebug(renderer.getDebugRenderer(), renderer.getDebugColor());
		}
	}
}

void BodyEditor::getInfo(std::string& name, glm::vec2& pos, glm::vec2& size, glm::vec2& scale, float& angle)
{
	name = "Body";
}

void BodyEditor::select(glm::vec2 point)
{
	
}

void BodyEditor::drag(glm::vec2 delta)
{
	
}

void BodyEditor::dragOver(glm::vec2 point)
{
	
}
