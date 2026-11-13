#include "BodyEditor.h"
#include "EditorGui.h"
#include "PathHelper.h"
#include "ObjectManager.h"
#include "TileRenderer.h"
#include "EntityRenderer.h"
#include <Logger.h>

BodyEditor::BodyEditor(EditorGui& parent) :
	AbstractEditor(parent, m_body)
{}

bool BodyEditor::create(const std::string& path, unsigned int type)
{
	m_body.m_type = static_cast<BodyType>(type);

	m_body.m_parts.clear();
	m_body.m_parts.resize(TypeHelper::bodyParts[type].size());

	m_body.m_joints.clear();
	m_body.m_joints.resize(TypeHelper::bodyJoints[type].size());
	
	const auto& partList = m_parent.getObjects(GameType::TYPE_PART);

	for (const auto& part : TypeHelper::bodyParts[type]) {
		bool partLoaded = false;
		for (const auto& partPath : partList) {
			if (PathHelper::getFileName(partPath).starts_with(m_parent.getNamePrefix(GameType::TYPE_PART, part.partType))) {
				m_parent.getObjectManager().loadUnique(partPath, m_body.m_parts[part.partIndex]);
				partLoaded = true;
				break;
			}
		}
		if (!partLoaded) {
			logE << "There is no instance of the part '" << part.name << "' created!" << std::endl <<
				"You must firt create at least one instance of all body parts before creating the body!" 
				<< std::endl;
			return false;
		}
	}
	for (const auto& joint : TypeHelper::bodyJoints[type]) {
		m_body.m_joints[joint.jointIndex].init(joint.jointIndex, joint.parentIndex, joint.childIndex);
		m_body.m_joints[joint.jointIndex].point = m_body.m_parts[joint.parentIndex].getDrawShape().getCenter();
	}
	m_body.updateShape();
	m_body.setPath(path);
	return true;
}

void BodyEditor::update()
{
	m_body.setCenter(m_parent.getScreenCenter());
	m_parent.setDebug(DebugType::BODY | DebugType::DRAW_PART | DebugType::COLL_PART | DebugType::SELECTED);
}

void BodyEditor::onDraw()
{
	/* Creation Basic Group */
	if (ImGui::BeginChild("Basic", ImVec2(0.0f, 125.0f), ImGuiChildFlags_Borders)) {
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().FramePadding.x);
		ImGui::Text("Basic");
		ImGui::Separator();
		if (ImGui::DragFloat("Angular Speed", &m_body.m_angularSpeed, 0.1f, 0.0f, 0.0f, "%.2f")) {
			m_parent.setNeedToSave(true);
		}
		if (ImGui::DragFloat("Mass", &m_body.m_mass, 1.0f, 1.0f, 0.0f, "%.2f")) {
			m_parent.setNeedToSave(true);
		}
		m_parent.drawEnum(m_body.m_material, TypeHelper::materialTypes, "Material");
		Shape2DType type = m_body.m_shape.getType();
		if (ImGui::BeginCombo("Collision Shape", TypeHelper::shapeTypes[type].c_str())) {
			for (const auto& [typeKey, typeName] : TypeHelper::shapeTypes) {
				if (typeKey != Shape2DType::POLYGON && typeKey != Shape2DType::TRIANGLE) {
					bool isSelected = type == typeKey;
					if (ImGui::Selectable(typeName.c_str(), isSelected) && !isSelected) {
						m_body.m_shape.setType(static_cast<Shape2DType>(typeKey));
						m_body.updateShape();
						m_parent.setNeedToSave(true);
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
			}
			ImGui::EndCombo();
		}
	}
	ImGui::EndChild();
	/* Creation Parts Group */
	if (ImGui::BeginChild("Parts", ImVec2(0.0f, 200.0f), ImGuiChildFlags_Borders)) {
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().FramePadding.x);
		ImGui::Text("Parts     ");
		ImGui::Separator();
		if (ImGui::BeginListBox("##HierarchyList", ImVec2(140.0f, -1.0f))) {
			for (unsigned int i = 0; i < TypeHelper::bodyParts[m_body.m_type].size(); ++i) {
				BodyPart& part = TypeHelper::bodyParts[m_body.m_type][i];
				bool isSelected = (m_selectedPartIndex == part.partIndex);
				if (ImGui::Selectable(part.name.c_str(), isSelected) && !isSelected) {
					m_selectedPartIndex = part.partIndex;
					m_selectedJointIndex = m_body.m_parts[m_selectedPartIndex].getParentJoint();
				}
			}
			ImGui::EndListBox();
		}
		if (m_selectedPartIndex >= 0) {
			ImGui::SameLine();
			if (ImGui::BeginChild("Part Instance", ImVec2(-1.0f, -1.0f), ImGuiChildFlags_Borders)) {
				Part& part = m_body.m_parts[m_selectedPartIndex];
				glm::vec2 scale = part.getScale();
				glm::vec2 pos = part.getPos();
				float angle = RadToDeg(part.getAngle());
				if (ImGui::SliderFloat("Angle°", &angle, -360.0f, 360.0f, "%.f")) {
					part.setAngle(DegToRad(angle));
					m_body.updateShape();
					m_parent.setNeedToSave(true);
				}
				if (ImGui::DragFloat2("Pos", &pos.x, 1.0f, 0.0f, 0.0f, "%.f")) {
					m_body.setPos(m_selectedPartIndex, pos);
					m_body.updateShape();
					m_parent.setNeedToSave(true);
				}
				if (ImGui::DragFloat2("Scale", &scale.x, 0.1f, 0.1f, 10.0f, "%.1f")) {
					m_body.setScale(m_selectedPartIndex, scale);
					m_body.updateShape();
					m_parent.setNeedToSave(true);
				}
				const auto& partList = m_parent.getObjects(GameType::TYPE_PART);
				std::string previewText = "- Select Part -";
				if (!part.getPath().empty()) {
					previewText = PathHelper::getFileName(part.getPath());
				}
				if (ImGui::BeginCombo("Part", previewText.c_str())) {
					for (int i = 0; i < (int)partList.size(); i++) {
						std::string listLabel = PathHelper::getFileName(partList[i]);
						if (listLabel.starts_with(m_parent.getNamePrefix(GameType::TYPE_PART, part.getType()))) {
							bool isSelected = (partList[i] == part.getPath());
							if (ImGui::Selectable(listLabel.c_str(), isSelected) && !isSelected) {
								m_parent.getObjectManager().loadUnique(partList[i], part);
								if (isSelected) {
									part.setAngle(DegToRad(angle));
									m_body.setScale(m_selectedPartIndex, scale);
									m_body.setPos(m_selectedPartIndex, pos);
								}
								else {
									part.setCenter(m_parent.getScreenCenter());
								}
								m_body.updateShape();
								m_parent.setNeedToSave(true);
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
	/* Creation joints Group */
	if (ImGui::BeginChild("Joints", ImVec2(0.0f, 200.0f), ImGuiChildFlags_Borders)) {
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().FramePadding.x);
		ImGui::Text("Joints");
		ImGui::Separator();
		if (ImGui::BeginListBox("##HierarchyList", ImVec2(140.0f, -1.0f))) {
			for (unsigned int i = 0; i < TypeHelper::bodyJoints[m_body.m_type].size(); ++i) {
				BodyJoint& joint = TypeHelper::bodyJoints[m_body.m_type][i];
				bool isSelected = (m_selectedJointIndex == joint.jointIndex);
				if (ImGui::Selectable(joint.name.c_str(), isSelected) && !isSelected) {
					m_selectedJointIndex = joint.jointIndex;
					m_selectedPartIndex = m_body.m_joints[m_selectedJointIndex].parent;
				}
			}
			ImGui::EndListBox();
		}
		if (m_selectedJointIndex >= 0) {
			ImGui::SameLine();
			if (ImGui::BeginChild("Joint", ImVec2(-1.0f, -1.0f), ImGuiChildFlags_Borders)) {
				Joint2D& joint = m_body.m_joints[m_selectedJointIndex];
				glm::vec3 cmove {joint.value.pos.x, joint.value.pos.y, RadToDeg(joint.value.a)};
				glm::vec3 umove{ joint.upper.pos.x, joint.upper.pos.y, RadToDeg(joint.upper.a) };
				glm::vec3 lmove{ joint.lower.pos.x, joint.lower.pos.y, RadToDeg(joint.lower.a) };
				
				if (ImGui::DragFloat2("Pos", &joint.point.x, 1.0f, 0.0f, 0.0f, "%.f")) {
					m_parent.setNeedToSave(true);
				}
				if (ImGui::DragFloat3("Value", &cmove.x, 1.0f, 0.0f, 0.0f, "%.f")) {
					float x = cmove.x - joint.value.pos.x;
					float y = cmove.y - joint.value.pos.y;
					float a = DegToRad(cmove.z) - joint.value.a;
					m_body.move(m_selectedJointIndex, x, y, a);
				}
				if (ImGui::DragFloat3("Upper", &umove.x, 1.0f, 0.0f, 0.0f, "%.f")) {
					if (umove.x >= lmove.x && umove.y >= lmove.y && umove.z >= lmove.z) {
						joint.upper.pos = glm::vec2(umove.x, umove.y);
						joint.upper.a = DegToRad(umove.z);
						m_parent.setNeedToSave(true);
					}
				}
				if (ImGui::DragFloat3("Lower", &lmove.x, 1.0f, 0.0f, 0.0f, "%.f")) {
					if (umove.x >= lmove.x && umove.y >= lmove.y && umove.z >= lmove.z) {
						joint.lower.pos = glm::vec2(lmove.x, lmove.y);
						joint.lower.a = DegToRad(lmove.z);
						m_parent.setNeedToSave(true);
					}
				}
			}
			ImGui::EndChild();
		}
	}
	ImGui::EndChild();
	if (ImGui::Button("Left settings to opposite parts")) {
		Part& mainPart = m_body.m_parts[0];
		const glm::vec2 center = m_body.m_parts[0].getDrawShape().getCenter();

		for (auto& bp : m_body.m_parts) {
			if (bp.m_oppositePart >= 0 && TypeHelper::partTypes[bp.m_type].find("LEFT") != std::string::npos) {
				Part& oppPart = m_body.m_parts[bp.m_oppositePart];

				float diffY1 = center.y - bp.getDrawShape().getCenterY();
				float diffY2 = oppPart.getDrawShape().getCenterY() - center.y;
				float diffY = diffY1 - diffY2;
				oppPart.incrPos(glm::vec2(0.0f, diffY));
				oppPart.setPos(glm::vec2(bp.getPos().x, oppPart.getPos().y));

				if (bp.m_parentJoint >= 0 && oppPart.m_parentJoint >= 0) {
					diffY1 = center.y - m_body.m_joints[bp.m_parentJoint].point.y;
					diffY2 = m_body.m_joints[oppPart.m_parentJoint].point.y - center.y;
					diffY = diffY1 - diffY2;
					m_body.m_joints[oppPart.m_parentJoint].point.y += diffY;
					m_body.m_joints[oppPart.m_parentJoint].point.x = m_body.m_joints[bp.m_parentJoint].point.x;
					m_body.m_joints[oppPart.m_parentJoint].angle = -m_body.m_joints[bp.m_parentJoint].angle;

					m_body.m_joints[oppPart.m_parentJoint].lower.a = -m_body.m_joints[bp.m_parentJoint].upper.a;
					m_body.m_joints[oppPart.m_parentJoint].lower.pos.x = m_body.m_joints[bp.m_parentJoint].lower.pos.x;
					m_body.m_joints[oppPart.m_parentJoint].lower.pos.y = -m_body.m_joints[bp.m_parentJoint].upper.pos.y;

					m_body.m_joints[oppPart.m_parentJoint].upper.a = -m_body.m_joints[bp.m_parentJoint].lower.a;
					m_body.m_joints[oppPart.m_parentJoint].upper.pos.x = m_body.m_joints[bp.m_parentJoint].upper.pos.x;
					m_body.m_joints[oppPart.m_parentJoint].upper.pos.y = -m_body.m_joints[bp.m_parentJoint].lower.pos.y;

					m_body.m_joints[oppPart.m_parentJoint].value.a = -m_body.m_joints[bp.m_parentJoint].value.a;
					m_body.m_joints[oppPart.m_parentJoint].value.pos.x = m_body.m_joints[bp.m_parentJoint].value.pos.x;
					m_body.m_joints[oppPart.m_parentJoint].value.pos.y = -m_body.m_joints[bp.m_parentJoint].value.pos.y;
				}
				m_parent.setNeedToSave(true);
			}
		}
	}
}

std::string BodyEditor::getName() const
{
	return "Body Editor";
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
		for (auto& j : m_body.getJoints()) {
			renderer.getDebugRenderer().drawCircle(j.point, renderer.getDebugColor(), 2.0f);
		}
	}
	if (m_parent.debug(DebugType::SELECTED) && m_selectedPartIndex >= 0) {
		m_body.m_parts[m_selectedPartIndex].getShape().drawDebug(renderer.getDebugRenderer(), m_selectedColor);
	}
}

void BodyEditor::getInfo(std::string& name, glm::vec2& pos, glm::vec2& size, glm::vec2& scale, float& angle)
{
	name = "Body";
	pos = m_body.getPos();
	size = m_body.getShape().getDims();
	scale = m_body.getScale();
	angle = RadToDeg(m_body.getAngle());
}

void BodyEditor::select(glm::vec2 point)
{
	int selected = -1;
	int depth = 0;
	for (unsigned int i = 0; i < m_body.m_parts.size(); ++i) {
		if (MathUtils::isPointInsideShape(point, m_body.m_parts[i].getDrawShape()) && 
			m_body.m_parts[i].getSprite().depth >= depth) {
			depth = m_body.m_parts[i].getSprite().depth;
			selected = i;
		}
	}
	if (selected >= 0) {
		m_selectedPartIndex = selected;
		m_selectedJointIndex = m_body.m_parts[m_selectedPartIndex].getParentJoint();
	}
}

void BodyEditor::drag(glm::vec2 delta)
{
	if (m_selectedPartIndex >= 0) {
		if (m_parent.isActiveTool(ToolType::MOVE)) {
			m_body.incrPos(m_selectedPartIndex, delta);
			m_body.updateShape();
			m_parent.setNeedToSave(true);
		}
		else if (m_parent.isActiveTool(ToolType::ROTATE)) {
			float value = (std::abs(delta.x) > std::abs(delta.y)) ? delta.x : delta.y;
			m_body.rotate(m_selectedPartIndex, DegToRad(value));
			m_body.updateShape();
			m_parent.setNeedToSave(true);
		}
		else if (m_parent.isActiveTool(ToolType::RESIZE)) {
			m_body.incrScale(m_selectedPartIndex, delta * 0.01f);
			m_body.updateShape();
			m_parent.setNeedToSave(true);
		}
	}
}

void BodyEditor::action(glm::vec2 point)
{
	if (m_selectedJointIndex >= 0) {
		m_body.m_joints[m_selectedJointIndex].point = point;
		m_parent.setNeedToSave(true);
	}
}
