/**
 * @file MainMenuGui.cpp
 * @brief Application Layer: Main menu window layout rendering logic.
 */

#include "EditorGui.h"
#include "EditorScreen.h"
#include "PathHelper.h"
#include "AbstractEditor.h"
#include "TileRenderer.h"
#include "LightRenderer.h"
#include "EntityRenderer.h"
#include "TileEditor.h"
#include "LightEditor.h"
#include "PartEditor.h"
#include "BodyEditor.h"
#include "LevelEditor.h"

#include <IMainGame.h>
#include <imgui/imgui.h>

constexpr float TOOLBAR_HEIGHT = 90.0f;
constexpr float CAMERA_SCALE_STEP = 0.1f;
constexpr float CAMERA_SCALE_MIN = 0.1f;
constexpr float CAMERA_SCALE_MAX = 10.0f;

EditorGui::EditorGui(EditorScreen& parent) :
	NS2::IGuiWindow(parent, "Game Editor Window")
{
	m_alpha = 0.0f;
	m_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoBringToFrontOnFocus;
}

EditorGui::~EditorGui() = default;

void EditorGui::initGui()
{
	NS2::IGuiWindow::initGui();

	loadStyleFromFile("../Gui/Styles/main_menu.conf");
	setSize(getScreenSize());
	setPosition(glm::vec2(0.0f, 0.0f));

	m_editors.resize(GameType::TYPE_COUNT);
	m_editors[GameType::TYPE_TILE] = std::make_unique<TileEditor>(*this);
	m_editors[GameType::TYPE_LIGHT] = std::make_unique<LightEditor>(*this);
	m_editors[GameType::TYPE_PART] = std::make_unique<PartEditor>(*this);
	m_editors[GameType::TYPE_BODY] = std::make_unique<BodyEditor>(*this);

	m_objects.resize(GameType::TYPE_COUNT, std::vector<std::string>());

	for (const auto& [typeKey, typeName] : TypeHelper::gameTypes) {
		m_objectTypes.push_back({ typeKey, typeName });
	}
	std::sort(m_objectTypes.begin(), m_objectTypes.end(), [](const auto& a, const auto& b) {
		return a.second < b.second;
	});
	m_assets.emplace_back(STR_NONE, false); // add empty path
	NS2::IOManager::getRecDirectoryEntries(TEXTUREPATH, m_assets, false);
	m_debugging[DebugType::DEBUG] = true;
}

void EditorGui::clear()
{
	m_needSave = false;
	m_selectedTypeIndex = -1;
	m_selectedObjectIndex = -1;
	m_pendingTypeIndex = -1;
	m_pendingObjectIndex = -1;
	m_activeModal = ModalType::NONE;
	m_previousModal = ModalType::NONE;
	m_activeTool = ToolType::SELECT;
	m_levelEditor = nullptr;

	m_assets.clear();
	m_editors.clear();
	m_objects.clear();
	m_objectTypes.clear();

	for (int i = 0; i < DebugType::COUNT; i++) {
		m_debugging[i] = false;
	}
}

void EditorGui::onExit()
{
	if (m_needSave) {
		m_activeModal = ModalType::EXIT;
	}
	else {
		m_parent.setScreenState(NS2::ScreenState::CHANGE_PREVIOUS);
	}
}

void EditorGui::draw(LightRenderer& lightRenderer, const glm::vec4& box)
{
	AbstractEditor* currentEditor = getCurrentEditor();
	if (currentEditor != nullptr) {
		currentEditor->draw(lightRenderer, box);
	}
}

void EditorGui::draw(TileRenderer & tileRenderer, const glm::vec4& box)
{
	AbstractEditor* currentEditor = getCurrentEditor();
	if (currentEditor != nullptr) {
		currentEditor->draw(tileRenderer, box);
	}
}

void EditorGui::draw(EntityRenderer & entityRenderer, const glm::vec4& box)
{
	AbstractEditor* currentEditor = getCurrentEditor();
	if (currentEditor != nullptr) {
		currentEditor->draw(entityRenderer, box);
	}
}

void EditorGui::drawDebug(LightRenderer& lightRenderer, const glm::vec4& box)
{
	AbstractEditor* currentEditor = getCurrentEditor();
	if (currentEditor != nullptr) {
		currentEditor->drawDebug(lightRenderer, box);
	}
}

void EditorGui::drawDebug(TileRenderer& tileRenderer, const glm::vec4& box)
{
	AbstractEditor* currentEditor = getCurrentEditor();
	if (currentEditor != nullptr) {
		currentEditor->drawDebug(tileRenderer, box);
	}
}

void EditorGui::drawDebug(EntityRenderer& entityRenderer, const glm::vec4& box)
{
	AbstractEditor* currentEditor = getCurrentEditor();
	if (currentEditor != nullptr) {
		currentEditor->drawDebug(entityRenderer, box);
	}
}

void EditorGui::onDraw()
{
	ImVec2 baseWindowPos = ImGui::GetWindowPos();
	ImVec2 baseWindowSize = ImGui::GetWindowSize();
	float contentTopOffset = TOOLBAR_HEIGHT + 10.0f;
	float sidebarWidth = baseWindowSize.x * 0.20f;
	float sidebarHeight = baseWindowSize.y - contentTopOffset - 15.0f;

	// Shared configuration flags for clean floating window panel layouts
	ImGuiWindowFlags overlayFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;

	// ---------------------------------------------------------------------
	// TOOL BAR LAYER: FLOATING HORIZONTAL TOOL BAR
	// ---------------------------------------------------------------------
	ImGui::SetNextWindowPos(ImVec2(baseWindowPos.x, baseWindowPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(baseWindowSize.x, TOOLBAR_HEIGHT), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(1.0f);

	ImGuiWindowFlags toolBarFlags = overlayFlags | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6.0f, 6.0f));

	if (ImGui::Begin("ToolBarOverlay##Top", nullptr, toolBarFlags))
	{
		drawToolBar();
	}
	ImGui::End();
	ImGui::PopStyleVar(); // Clear layout padding metrics stack variables

	// ---------------------------------------------------------------------
	// OVERLAY 1: FLOATING LEFT TOOLBOX SIDEBAR
	// ---------------------------------------------------------------------
	ImGui::SetNextWindowPos(ImVec2(baseWindowPos.x + 10.0f, baseWindowPos.y + contentTopOffset), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(sidebarWidth, sidebarHeight), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(1.0f);

	if (ImGui::Begin("Creation Panel##LeftOverlay", nullptr, overlayFlags))
	{
		if (m_onLevel && m_levelEditor != nullptr) {
			m_levelEditor->drawLeftSidebar();
		}
		else {
			drawLeftSidebar();
		}
	}
	ImGui::End(); // Close Left Overlay panel context

	// ---------------------------------------------------------------------
	// OVERLAY 2: FLOATING RIGHT PROPERTIES INSPECTOR
	// ---------------------------------------------------------------------
	sidebarWidth = baseWindowSize.x * 0.30f; // slightly wider than left panel
	float rightPanelX = baseWindowPos.x + baseWindowSize.x - sidebarWidth - 10.0f;
	ImGui::SetNextWindowPos(ImVec2(rightPanelX, baseWindowPos.y + contentTopOffset), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(sidebarWidth, sidebarHeight), ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(1.0f);

	std::string title = "Property Panel##RightOverlay";
	if (getCurrentEditor() != nullptr) {
		title = "Property Panel : " + getCurrentEditor()->getName() + "##RightOverlay";
	}
	if (ImGui::Begin(title.c_str(), nullptr, overlayFlags))
	{
		if (m_onLevel && m_levelEditor != nullptr) {
			m_levelEditor->drawRightSidebar();
		}
		else {
			drawRightSidebar();
		}
	}
	ImGui::End(); // Close Right Overlay panel context

	// ---------------------------------------------------------------------
	// DYNAMIC MODALS LAYER: Intercepts popup updates on root layer safely
	// ---------------------------------------------------------------------
	drawModals();

	// handle input event on non ui space
	handleInputEvent();
}

NS2::ResourceManager& EditorGui::getResourceManager() const
{
	return m_parent.getGame().getResourceManager();
}

ObjectManager& EditorGui::getObjectManager() const
{
	return static_cast<EditorScreen&>(m_parent).getObjectManager();
}

Level& EditorGui::getLevel() const
{
	return static_cast<EditorScreen&>(m_parent).getLevel();
}

glm::vec2 EditorGui::getScreenSize() const
{
	return glm::vec2(m_parent.getGame().getWindow().getScreenWidth(),
		m_parent.getGame().getWindow().getScreenHeight());
}

glm::vec2 EditorGui::getScreenCenter() const
{
	return glm::vec2(m_parent.getGame().getWindow().getScreenWidth() / 2.0f,
		m_parent.getGame().getWindow().getScreenHeight() / 2.0f);
}

AbstractEditor* EditorGui::getCurrentEditor()
{
	if (m_onLevel && m_levelEditor != nullptr) {
		return m_levelEditor.get();
	}
	else if (m_selectedObjectIndex >= 0 && m_selectedTypeIndex >= 0 &&
		m_editors[m_selectedTypeIndex].get() != nullptr) {
		return m_editors[m_selectedTypeIndex].get();
	}
	else {
		return nullptr;
	}
}

bool EditorGui::debug(DebugType type) const
{
	return m_debugging[type];
}

const std::vector<std::string>& EditorGui::getObjects(int typeIndex)
{
	loadObjectTypes(typeIndex);
	return m_objects[typeIndex];
}

void EditorGui::drawToolBar()
{
	float groupHeight = TOOLBAR_HEIGHT - (ImGui::GetStyle().WindowPadding.y * 2);
	/* Creation Mode Group */
	if (ImGui::BeginChild("Mode", ImVec2(100.0f, groupHeight), ImGuiChildFlags_Borders)) {
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().FramePadding.x);
		ImGui::Text("Edition Mode");
		ImGui::Separator();
		if (ImGui::RadioButton("Creation", &m_onLevel, 0)) {
			if (m_needSave) {
				m_activeModal = ModalType::CHANGE_MODE;
			}
			else {
				changeMode();
			}
		}
		if (ImGui::RadioButton("Level", &m_onLevel, 1)) {
			if (m_needSave) {
				m_activeModal = ModalType::CHANGE_MODE;
			}
			else {
				changeMode();
			}
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	/* Toolbox Group */
	if (ImGui::BeginChild("Toolbox", ImVec2(300.0f, groupHeight), ImGuiChildFlags_Borders)) {
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().FramePadding.x);
		ImGui::Text("Toolbox");
		ImGui::Separator();
		ImGui::RadioButton("Select", reinterpret_cast<int*>(&m_activeTool), static_cast<int>(ToolType::SELECT));
		ImGui::SameLine();
		ImGui::RadioButton("Move", reinterpret_cast<int*>(&m_activeTool), static_cast<int>(ToolType::MOVE));
		ImGui::SameLine();
		ImGui::RadioButton("Resize", reinterpret_cast<int*>(&m_activeTool), static_cast<int>(ToolType::RESIZE));
		ImGui::SameLine();
		ImGui::RadioButton("Rotate", reinterpret_cast<int*>(&m_activeTool), static_cast<int>(ToolType::ROTATE));

		if (m_onLevel && m_levelEditor != nullptr) {
			ImGui::RadioButton("Attach", reinterpret_cast<int*>(&m_activeTool), static_cast<int>(ToolType::ATTACH));
			ImGui::SameLine();
			ImGui::RadioButton("Copy", reinterpret_cast<int*>(&m_activeTool), static_cast<int>(ToolType::COPY));
			ImGui::SameLine();
			ImGui::RadioButton("Place", reinterpret_cast<int*>(&m_activeTool), static_cast<int>(ToolType::PLACE));
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	/* Debugging Group */
	if (ImGui::BeginChild("Debugging", ImVec2(220.0f, groupHeight), ImGuiChildFlags_Borders)) {
		const ImVec2 dims = ImVec2(70.0f, 0.0f);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().FramePadding.x);
		ImGui::Text("Debugging");
		ImGui::Separator();
		if (ImGui::Selectable("Debug", m_debugging[DebugType::DEBUG], 0, dims)) {
			m_debugging[DebugType::DEBUG] = !m_debugging[DebugType::DEBUG];
		}
		ImGui::SameLine();
		if (ImGui::Selectable("Selected", m_debugging[DebugType::SELECTED], 0, dims)) {
			m_debugging[DebugType::SELECTED] = !m_debugging[DebugType::SELECTED];
		}

		if (m_onLevel && m_levelEditor != nullptr) {
			ImGui::SameLine();
			if (ImGui::Selectable("Entities", m_debugging[DebugType::ENTITIES], 0, dims)) {
				m_debugging[DebugType::ENTITIES] = !m_debugging[DebugType::ENTITIES];
			}
			if (ImGui::Selectable("Tree", m_debugging[DebugType::TREE], 0, dims)) {
				m_debugging[DebugType::TREE] = !m_debugging[DebugType::TREE];
			}
			ImGui::SameLine();
			if (ImGui::Selectable("Node", m_debugging[DebugType::NODE], 0, dims)) {
				m_debugging[DebugType::NODE] = !m_debugging[DebugType::NODE];
			}
			ImGui::SameLine();
			if (ImGui::Selectable("Edge", m_debugging[DebugType::EDGE], 0, dims)) {
				m_debugging[DebugType::EDGE] = !m_debugging[DebugType::EDGE];
			}
			if (ImGui::Selectable("Tiles", m_debugging[DebugType::TILES], 0, dims)) {
				m_debugging[DebugType::TILES] = !m_debugging[DebugType::TILES];
			}
			ImGui::SameLine();
			if (ImGui::Selectable("Lights", m_debugging[DebugType::LIGHTS], 0, dims)) {
				m_debugging[DebugType::LIGHTS] = !m_debugging[DebugType::LIGHTS];
			}
			ImGui::SameLine();
			if (ImGui::Selectable("Sensors", m_debugging[DebugType::SENSORS], 0, dims)) {
				m_debugging[DebugType::SENSORS] = !m_debugging[DebugType::SENSORS];
			}
		}
		else {
			if (ImGui::Selectable("Bodies", m_debugging[DebugType::BODY], 0, dims)) {
				m_debugging[DebugType::BODY] = !m_debugging[DebugType::BODY];
			}
			ImGui::SameLine();
			if (ImGui::Selectable("D. Parts", m_debugging[DebugType::DRAW_PART], 0, dims)) {
				m_debugging[DebugType::DRAW_PART] = !m_debugging[DebugType::DRAW_PART];
			}
			ImGui::SameLine();
			if (ImGui::Selectable("C. Parts", m_debugging[DebugType::COLL_PART], 0, dims)) {
				m_debugging[DebugType::COLL_PART] = !m_debugging[DebugType::COLL_PART];
			}
		}
	}
	ImGui::EndChild();
	auto editor = getCurrentEditor();
	if (editor != nullptr && editor->hasAnim()) {
		ImGui::SameLine();
		/* Animation Group */
		if (ImGui::BeginChild("Animation", ImVec2(115.0f, groupHeight), ImGuiChildFlags_Borders)) {
			const ImVec2 dims = ImVec2(50.0f, 0.0f);
			const bool animOn = m_animOn;
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().FramePadding.x);
			ImGui::Text("Animation");
			ImGui::Separator();
			if (animOn) ImGui::BeginDisabled();
			if (ImGui::Button("Start", dims)) {
				editor->startAnim();
				m_animOn = true;
			}
			if (animOn) ImGui::EndDisabled();
			else		  ImGui::BeginDisabled();
			ImGui::SameLine();
			if (ImGui::Button("Stop", dims)) {
				editor->stopAnim();
				m_animOn = false;
			}
			if (!animOn) ImGui::EndDisabled();
			ImGui::PushItemWidth((dims.x * 2) + ImGui::GetStyle().ItemSpacing.x);
			ImGui::SliderFloat("##Factor", &m_animFactor, 0.1f, 10.0f, "%.2f");
			ImGui::PopItemWidth();
		}
		ImGui::EndChild();
	}
	ImGui::SameLine();
	/* Camera Group */
	if (ImGui::BeginChild("Camera", ImVec2(150.0f, groupHeight), ImGuiChildFlags_Borders)) {
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().FramePadding.x);
		ImGui::Text("Camera");
		ImGui::Separator();
		ImGui::PushItemWidth(100.0f);
		float cameraScale = static_cast<EditorScreen&>(m_parent).getCamera().getScale();
		if (ImGui::SliderFloat("Scale", &cameraScale, CAMERA_SCALE_MIN, CAMERA_SCALE_MAX, "%.2f")) {
			static_cast<EditorScreen&>(m_parent).getCamera().setScale(cameraScale);
		}
		glm::vec2 cameraPos = static_cast<EditorScreen&>(m_parent).getCamera().getPosition();
		if (ImGui::DragFloat2("[X Y]", &cameraPos.x, 10.0f, 0.0f, 0.0f, "%.2f")) {
			static_cast<EditorScreen&>(m_parent).getCamera().setPosition(cameraPos);
		}
		ImGui::PopItemWidth();
	}
	ImGui::EndChild();
	if (editor != nullptr) {
		ImGui::SameLine();
		/* Information Group */
		if (ImGui::BeginChild("Information", ImVec2(300.0f, groupHeight), ImGuiChildFlags_Borders)) {
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetStyle().FramePadding.x);
			ImGui::Text("Information");
			ImGui::Separator();
			float angle = 0.0f;
			std::string name;
			glm::vec2 pos = glm::vec2(0.0f), size = glm::vec2(0.0f), scale = glm::vec2(1.0f);
			editor->getInfo(name, pos, size, scale, angle);
			name = "Name: " + name;
			std::string str1 = std::format("X, Y, W, H: [{:.1f}, {:.1f}, {:.1f}, {:.1f}]", pos.x, pos.y, size.x, size.y);
			std::string str2 = std::format("Sx, Sy, A: [{:.1f}, {:.1f}, {:.1f}°]", scale.x, scale.y, RadToDeg(angle));
			ImGui::Text(name.c_str());
			ImGui::Text(str1.c_str());
			ImGui::Text(str2.c_str());
		}
		ImGui::EndChild();
	}
}

void EditorGui::drawLeftSidebar()
{
	ImGui::Text("Choose Object type:");
	ImGui::SetNextItemWidth(-1.0f);

	std::string previewText;
	if (m_selectedTypeIndex < 0) {
		previewText = "-- Select Object Type --";
	}
	else {
		previewText = TypeHelper::gameTypes[m_selectedTypeIndex];
	}

	if (ImGui::BeginCombo("##SpawnTypeMap", previewText.c_str())) {
		for (const auto& [typeKey, typeName] : m_objectTypes) {
			bool isSelected = m_selectedTypeIndex == typeKey;
			if (ImGui::Selectable(typeName.c_str(), isSelected) && !isSelected) {
				if (m_needSave) {
					m_pendingTypeIndex = typeKey;
					m_activeModal = ModalType::CHANGE_TYPE;
				}
				else {
					changeType(typeKey);
				}
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	// INTERACTIVE MODAL CREATION PROMPT
	bool typeIsSelected = (m_selectedTypeIndex >= 0 && m_editors[m_selectedTypeIndex] != nullptr);
	if (!typeIsSelected) ImGui::BeginDisabled();

	if (ImGui::Button("CREATE NEW INSTANCE", ImVec2(-1.0f, 40.0f))) {
		std::string newName = PathHelper::newObjectName(static_cast<GameType>(m_selectedTypeIndex));
		snprintf(m_buffer, sizeof(m_buffer), newName.c_str());
		m_activeModal = ModalType::CREATE;
	}
	if (!typeIsSelected) ImGui::EndDisabled();

	ImGui::Separator();

	// MULTI-ACTION ROW BUTTONS FOR CURRENT HIGHLIGHTED INSTANCE
	bool needSave = m_needSave;
	bool hasSelectedInstance = (m_selectedObjectIndex >= 0 && typeIsSelected);
	if (!hasSelectedInstance || !needSave) ImGui::BeginDisabled();

	// Calculate a perfect three-way fractional width column sizing balance
	float actionBtnWidth = (ImGui::GetContentRegionAvail().x - (ImGui::GetStyle().ItemSpacing.x * 2.0f)) / 4.0f;

	if (ImGui::Button("SAVE", ImVec2(actionBtnWidth, 26.0f))) {
		AbstractEditor* currentEditor = getCurrentEditor();
		if (currentEditor && currentEditor->save()) {
			m_needSave = false;
		}
		else {
			snprintf(m_buffer, sizeof(m_buffer), "Could not save the current object");
			m_activeModal = ModalType::ERROR;
		}
	}
	if (!hasSelectedInstance || !needSave) ImGui::EndDisabled();
	if (!hasSelectedInstance) ImGui::BeginDisabled();
	ImGui::SameLine();

	if (ImGui::Button("COPY", ImVec2(actionBtnWidth, 26.0f))) {
		std::string filename = PathHelper::getFileName(m_objects[m_selectedTypeIndex][m_selectedObjectIndex]);
		snprintf(m_buffer, sizeof(m_buffer), "%s", filename.c_str());
		m_activeModal = ModalType::COPY;
	}
	ImGui::SameLine();

	if (ImGui::Button("RENAME", ImVec2(actionBtnWidth, 26.0f))) {
		std::string filename = PathHelper::getFileName(m_objects[m_selectedTypeIndex][m_selectedObjectIndex]);
		snprintf(m_buffer, sizeof(m_buffer), "%s", filename.c_str());
		m_activeModal = ModalType::RENAME;
	}
	ImGui::SameLine();

	if (ImGui::Button("DELETE", ImVec2(actionBtnWidth, 26.0f))) {
		m_activeModal = ModalType::DELETE;
	}

	if (!hasSelectedInstance) ImGui::EndDisabled();

	ImGui::Separator();
	ImGui::Text("Choose Object instance:");

	if (typeIsSelected) {
		if (ImGui::BeginListBox("##HierarchyList", ImVec2(-1.0f, -1.0f))) {
			for (int i = 0; i < (int)m_objects[m_selectedTypeIndex].size(); i++) {
				bool isSelected = (m_selectedObjectIndex == i);
				std::string listLabel = PathHelper::getFileName(m_objects[m_selectedTypeIndex][i]);
				if (ImGui::Selectable(listLabel.c_str(), isSelected) && !isSelected) {
					if (m_needSave) {
						m_pendingObjectIndex = i;
						m_activeModal = ModalType::CHANGE_OBJECT;
					}
					else {
						changeObject(i);
					}
				}
			}
			ImGui::EndListBox();
		}
	}
	else {
		ImGui::TextDisabled("Select an object type first.");
	}
}

void EditorGui::drawRightSidebar()
{
	if (m_selectedObjectIndex >= 0 && m_selectedTypeIndex >= 0) {
		AbstractEditor* currentEditor = getCurrentEditor();
		if (currentEditor) {
			currentEditor->onDraw();
		}
	}
	else {
		ImGui::TextDisabled("Select an object instance.");
	}
}

void EditorGui::drawModals()
{
	// 1. OPEN TRIGGERS (Executed outside immediate window bounds to prevent stack corruption)
	if (m_activeModal == ModalType::CREATE) { ImGui::OpenPopup("Create Instance"); m_activeModal = ModalType::NONE; m_modalIndex = 0; }
	if (m_activeModal == ModalType::COPY) { ImGui::OpenPopup("Copy Instance"); m_activeModal = ModalType::NONE; }
	if (m_activeModal == ModalType::RENAME) { ImGui::OpenPopup("Rename Instance"); m_activeModal = ModalType::NONE; }
	if (m_activeModal == ModalType::DELETE) { ImGui::OpenPopup("Confirm Deletion"); m_activeModal = ModalType::NONE; }
	if (m_activeModal == ModalType::ERROR) { ImGui::OpenPopup("Error"); m_activeModal = ModalType::NONE; }
	if (m_activeModal == ModalType::EXIT || m_activeModal == ModalType::CHANGE_MODE 
		|| m_activeModal == ModalType::CHANGE_TYPE || m_activeModal == ModalType::CHANGE_OBJECT) { 
		ImGui::OpenPopup("Unsaved Changes"); 
		m_previousModal = m_activeModal;
		m_activeModal = ModalType::NONE; 
	}

	// 2. CREATE INSTANCE MODAL DIALOG
	if (ImGui::BeginPopupModal("Create Instance", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::TextUnformatted("Enter name for the new Instance:");
		std::string namePrefix = getNamePrefix(m_selectedTypeIndex, m_modalIndex);
		if (m_selectedTypeIndex == GameType::TYPE_PART) {
			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted("Part Type");
			ImGui::SameLine();
			if (ImGui::BeginCombo("##Part Type", TypeHelper::partTypes[m_modalIndex].second.c_str())) {
				for (const auto& [typeKey, typeName] : TypeHelper::partTypes) {
					bool isSelected = (m_modalIndex == typeKey);
					if (ImGui::Selectable(typeName.second.c_str(), isSelected) && !isSelected) {
						m_modalIndex = typeKey;
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}
		else if (m_selectedTypeIndex == GameType::TYPE_BODY) {
			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted("Body Type");
			ImGui::SameLine();
			if (ImGui::BeginCombo("##Body Type", TypeHelper::bodyTypes[m_modalIndex].c_str())) {
				for (const auto& [typeKey, typeName] : TypeHelper::bodyTypes) {
					bool isSelected = (m_modalIndex == typeKey);
					if (ImGui::Selectable(typeName.c_str(), isSelected) && !isSelected) {
						m_modalIndex = typeKey;
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}
		if (!namePrefix.empty()) {
			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted(namePrefix.c_str());
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
		}
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::InputText("##NewName", m_buffer, IM_ARRAYSIZE(m_buffer));
		ImGui::Separator();
		if (ImGui::Button("OK", ImVec2(120, 0))) {
			std::string newPath = PathHelper::getObjectPath(namePrefix + std::string(m_buffer), static_cast<GameType>(m_selectedTypeIndex));
			if (NS2::IOManager::exists(newPath.c_str())) {
				sprintf(m_buffer, "Cannot create new instance '%s'.\nthis file name already exists!", newPath.c_str());
				m_activeModal = ModalType::ERROR;
			}
			else {
				AbstractEditor* currentEditor = nullptr;
				if (m_selectedTypeIndex >= 0 && m_editors[m_selectedTypeIndex].get() != nullptr) {
					currentEditor = m_editors[m_selectedTypeIndex].get();
				}
				if (currentEditor) {
					getLevel().setAmbientColor(glm::vec3(1.0f));
					currentEditor->create(newPath, m_modalIndex);
					currentEditor->update();
					if (currentEditor->save()) {
						m_objects[m_selectedTypeIndex].push_back(newPath);
						m_selectedObjectIndex = (int)m_objects[m_selectedTypeIndex].size() - 1;
						m_needSave = false; // create will also save
					}
					else {
						sprintf(m_buffer, "Failed to create new instance: %s", newPath.c_str());
						m_activeModal = ModalType::ERROR;
					}
				}
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}

	// 3. RENAME INSTANCE MODAL DIALOG
	if (ImGui::BeginPopupModal("Rename Instance", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::TextUnformatted("Enter new name for the selected instance:");
		std::string namePrefix = getNamePrefix(m_selectedTypeIndex, m_modalIndex);
		if (!namePrefix.empty()) {
			std::string name(m_buffer);
			size_t pos = name.find(namePrefix);
			if (pos != std::string::npos) {
				name = name.substr(pos + namePrefix.size());
				snprintf(m_buffer, sizeof(m_buffer), "%s", name.c_str());
			}
			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted(namePrefix.c_str());
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
		}
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::InputText("##RenameBuf", m_buffer, IM_ARRAYSIZE(m_buffer));
		ImGui::Separator();
		if (ImGui::Button("OK", ImVec2(120, 0))) {
			std::string newPath = PathHelper::getObjectPath(namePrefix + std::string(m_buffer), static_cast<GameType>(m_selectedTypeIndex));
			if (NS2::IOManager::exists(newPath.c_str())) {
				sprintf(m_buffer, "Cannot rename the instance '%s'.\nthis file name already exists!", newPath.c_str());
				m_activeModal = ModalType::ERROR;
			}
			else {
				AbstractEditor* currentEditor = getCurrentEditor();
				if (currentEditor && currentEditor->rename(newPath)) {
					m_objects[m_selectedTypeIndex][m_selectedObjectIndex] = newPath;
					m_needSave = false; // rename will also save
				}
				else {
					sprintf(m_buffer, "Failed to rename instance: %s", newPath.c_str());
					m_activeModal = ModalType::ERROR;
				}
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}

	// 4. CONFIRM DELETE MODAL DIALOG
	if (ImGui::BeginPopupModal("Confirm Deletion", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		std::string activeName = PathHelper::getFileName(m_objects[m_selectedTypeIndex][m_selectedObjectIndex]);
		ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Are you absolute sure you want to completely delete:\n %s?", activeName.c_str());
		ImGui::Separator();
		if (ImGui::Button("YES, DELETE", ImVec2(120, 0))) {
			AbstractEditor* currentEditor = getCurrentEditor();
			if (currentEditor && currentEditor->remove()) {
				m_objects[m_selectedTypeIndex].erase(m_objects[m_selectedTypeIndex].begin() + m_selectedObjectIndex);
				m_selectedObjectIndex = -1; // Deselect instance cleanly
			}
			else {
				sprintf(m_buffer, "Failed to delete instance: %s", activeName.c_str());
				m_activeModal = ModalType::ERROR;
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("NO", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}

	// 5. COPY INSTANCE MODAL DIALOG
	if (ImGui::BeginPopupModal("Copy Instance", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::TextUnformatted("Enter name for the copied object configuration file:");
		ImGui::InputText("##CopyName", m_buffer, IM_ARRAYSIZE(m_buffer));
		ImGui::Separator();
		if (ImGui::Button("OK", ImVec2(120, 0))) {
			std::string newPath = PathHelper::getObjectPath(std::string(m_buffer), static_cast<GameType>(m_selectedTypeIndex));
			if (NS2::IOManager::exists(newPath.c_str())) {
				sprintf(m_buffer, "Cannot copy the instance '%s'.\nthis file name already exists!", newPath.c_str());
				m_activeModal = ModalType::ERROR;
			}
			else {
				AbstractEditor* currentEditor = getCurrentEditor();
				if (currentEditor && currentEditor->copy(newPath)) {
					m_objects[m_selectedTypeIndex].push_back(newPath);
				}
				else {
					sprintf(m_buffer, "Failed to copy instance: %s", newPath.c_str());
					m_activeModal = ModalType::ERROR;
				}
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}

	// 6. ERROR MODAL DIALOG
	if (ImGui::BeginPopupModal("Error", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::TextUnformatted("An error occurred:");
		ImGui::Text("%s", m_buffer);
		ImGui::Separator();
		if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}

	// 7. UNSAVED CHANGE MODE MODAL DIALOG
	if (ImGui::BeginPopupModal("Unsaved Changes", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::TextUnformatted("You have unsaved changes.");
		ImGui::Separator();
		if (ImGui::Button("SAVE", ImVec2(120, 0))) {
			AbstractEditor* currentEditor = getCurrentEditor();
			if (currentEditor && currentEditor->save()) {
				if (m_previousModal == ModalType::CHANGE_TYPE) {
					changeType(m_pendingTypeIndex);
				}
				else if (m_previousModal == ModalType::CHANGE_OBJECT) {
					changeObject(m_pendingObjectIndex);
				}
				else if (m_previousModal == ModalType::CHANGE_MODE) {
					changeMode();
				}
				else if (m_previousModal == ModalType::EXIT) {
					m_parent.setScreenState(NS2::ScreenState::CHANGE_PREVIOUS);
				}
				m_needSave = false;
				m_pendingObjectIndex = -1;
				m_pendingTypeIndex = -1;
				m_previousModal = ModalType::NONE;
			}
			else {
				sprintf(m_buffer, "Failed to save current instance!");
				m_activeModal = ModalType::ERROR;
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("DISCARD", ImVec2(120, 0))) { 
			if (m_previousModal == ModalType::CHANGE_TYPE) {
				changeType(m_pendingTypeIndex);
			}
			else if (m_previousModal == ModalType::CHANGE_OBJECT) {
				changeObject(m_pendingObjectIndex);
			}
			else if (m_previousModal == ModalType::CHANGE_MODE) {
				changeMode();
			}
			else if (m_previousModal == ModalType::EXIT) {
				m_parent.setScreenState(NS2::ScreenState::CHANGE_PREVIOUS);
			}
			m_needSave = false;
			m_pendingObjectIndex = -1;
			m_pendingTypeIndex = -1;
			m_previousModal = ModalType::NONE;
			ImGui::CloseCurrentPopup(); 
		}
		ImGui::EndPopup();
	}
}

void EditorGui::changeMode()
{
	if (m_onLevel) {
		if (!m_levelEditor) {
			m_levelEditor = std::make_unique<LevelEditor>(*this);
		}
		m_selectedTypeIndex = -1;
		m_selectedObjectIndex = -1;
	}
	else {

	}
	m_needSave = false;
	m_activeTool = ToolType::SELECT;
	for (int i = 0; i < DebugType::COUNT; i++) {
		m_debugging[i] = false;
	}
}

void EditorGui::changeType(int index)
{
	if (index < 0 && index >= m_editors.size()) {
		return;
	}
	m_selectedTypeIndex = index;
	m_selectedObjectIndex = -1; // Reset selection index when type changes

	if (m_editors[index] != nullptr && m_objects[index].empty()) {
		loadObjectTypes(index);
	}
}

void EditorGui::changeObject(int index)
{
	m_selectedObjectIndex = index;
	AbstractEditor* currentEditor = getCurrentEditor();
	if (currentEditor) {
		const std::string path = m_objects[m_selectedTypeIndex][index];
		bool flag = currentEditor->load(path);
		if (!flag) {
			snprintf(m_buffer, sizeof(m_buffer), "Could not load file %s", path.c_str());
			m_activeModal = ModalType::ERROR;
		}
		else {
			getLevel().setAmbientColor(glm::vec3(1.0f));
			currentEditor->update();
		}
	}
}

void EditorGui::loadObjectTypes(int typeIndex)
{
	if (typeIndex >= 0 && typeIndex < m_objects.size() && m_objects[typeIndex].empty()) {
		std::string path = PathHelper::getObjectTypePath(TypeHelper::gameTypes[typeIndex]);
		PathHelper::createDir(path);
		std::vector<NS2::DirEntry> entries;
		NS2::IOManager::getRecDirectoryEntries(path.c_str(), entries, false);
		for (auto& e : entries) {
			m_objects[typeIndex].push_back(e.path);
		}
	}
}

void EditorGui::handleInputEvent()
{
	AbstractEditor* currentEditor = getCurrentEditor();
	bool isOverBackground = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow);

	if (!isOverBackground || currentEditor == nullptr) {
		return;
	}

	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
			if (m_activeTool == ToolType::MOVE || m_activeTool == ToolType::RESIZE
				|| m_activeTool == ToolType::PLACE || m_activeTool == ToolType::COPY
				|| m_activeTool == ToolType::ROTATE) {
				ImVec2 dragDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
				currentEditor->drag(glm::vec2(dragDelta.x, -dragDelta.y));
				ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
			}
		}
		else {
			currentEditor->select(getWorldCoord(ImGui::GetMousePos()));
		}
	}
	else if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
		currentEditor->dragOver(getWorldCoord(ImGui::GetMousePos()));
	}
	else if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
		currentEditor->action(getWorldCoord(ImGui::GetMousePos()));
	}
	else if (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f) {
		if (m_activeTool == ToolType::PLACE || m_activeTool == ToolType::COPY) {
			currentEditor->move(glm::vec2(io.MouseDelta.x, -io.MouseDelta.y));
		}
	}
	else if (io.MouseWheel != 0.0f) {
		float wheel = io.MouseWheel;
		float cameraScale = static_cast<EditorScreen&>(m_parent).getCamera().getScale();
		if ((wheel > 0.0f && cameraScale + CAMERA_SCALE_STEP < CAMERA_SCALE_MAX) ||
			(wheel < 0.0f && cameraScale - CAMERA_SCALE_STEP > CAMERA_SCALE_MIN)) {
			cameraScale += wheel * 0.1f;
			static_cast<EditorScreen&>(m_parent).getCamera().setScale(cameraScale);
		}
	}
}

std::string EditorGui::getNamePrefix(int typeIndex, int objectIndex) const
{
	if (typeIndex == GameType::TYPE_PART) {
		return TypeHelper::partTypes[objectIndex].second + "_";
	}
	else if (typeIndex == GameType::TYPE_BODY) {
		return TypeHelper::bodyTypes[objectIndex] + "_";
	}
	return "";
}

glm::vec2 EditorGui::getWorldCoord(ImVec2 coords)
{
	glm::vec2 screenCoords(coords.x, coords.y);
	return static_cast<EditorScreen&>(m_parent).getCamera().screenToWorld(screenCoords);
}

bool EditorGui::drawColor(NS2::ColorRGBA8& color, const char* label)
{
	bool result = false;
	float temp_rgba[4] = {
		color.r / 255.0f,
		color.g / 255.0f,
		color.b / 255.0f,
		color.a / 255.0f
	};
	if (ImGui::ColorEdit4(label, temp_rgba)) {
		color.r = static_cast<GLubyte>(temp_rgba[0] * 255.0f + 0.5f);
		color.g = static_cast<GLubyte>(temp_rgba[1] * 255.0f + 0.5f);
		color.b = static_cast<GLubyte>(temp_rgba[2] * 255.0f + 0.5f);
		color.a = static_cast<GLubyte>(temp_rgba[3] * 255.0f + 0.5f);
		m_needSave = true;
		result = true;
	}
	return result;
}

bool EditorGui::drawTexture(NS2::GLTexture*& texture, const char* label)
{
	bool result = false;
	std::string previewText = "-- Select Texture Image --";
	if (texture != nullptr && !texture->filePath.empty()) {
		previewText = texture->filePath;
	}
	if (ImGui::BeginCombo(label, previewText.c_str())) {
		for (const auto& e : m_assets) {
			bool isSelected = texture != nullptr && texture->filePath == e.path;
			if (ImGui::Selectable(e.path.c_str(), isSelected) && !isSelected) {
				if (e.path.empty() || e.path == STR_NONE) {
					texture = nullptr;
				}
				else {
					texture = getResourceManager().getTexture(e.path);
				}
				m_needSave = true;
				result = true;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	return result;
}
