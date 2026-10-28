/**
 * @file EditorGui.h
 * @brief Editor GUI overlay layout for the game application.
 */

#pragma once

#include <IGuiWindow.h>
#include <IOManager.h>
#include <memory>
#include <vector>
#include <string>

class EditorScreen;
class AbstractEditor;
class ObjectManager;
class TileRenderer;
class LightRenderer;
class EntityRenderer;
class Level;
class LevelEditor;

namespace NS2 {
	class ResourceManager;
}

enum class ModalType {
	CREATE, COPY, DELETE, RENAME, CHANGE_MODE, CHANGE_TYPE, CHANGE_OBJECT, ERROR, EXIT, NONE
};
enum ToolType {
	SELECT = 0, MOVE = 1, RESIZE = 2, ROTATE = 3, PLACE = 4, ATTACH = 5, COPY = 6
};
enum DebugType {
	DEBUG = 0, SELECTED = 1, BODY = 2, DRAW_PART = 3, COLL_PART = 4, 
	TREE = 5, NODE = 6, EDGE = 7, TILES = 8, LIGHTS = 9, SENSORS = 10, ENTITIES = 11, COUNT = 12
};

class EditorGui : public NS2::IGuiWindow
{
public:
	EditorGui(EditorScreen& parent);
	~EditorGui();

	void clear();
	void onExit();

	void draw(LightRenderer& lightRenderer, const glm::vec4& box);
	void draw(TileRenderer& tileRenderer, const glm::vec4& box);
	void draw(EntityRenderer& entityRenderer, const glm::vec4& box);
	void drawDebug(TileRenderer& tileRenderer, const glm::vec4& box);
	void drawDebug(LightRenderer& lightRenderer, const glm::vec4& box);
	void drawDebug(EntityRenderer& entityRenderer, const glm::vec4& box);

	void initGui() override;
	void onDraw() override;

	void setNeedToSave(bool needSave) { m_needSave = needSave; }

	NS2::ResourceManager& getResourceManager() const;
	ObjectManager& getObjectManager() const;
	Level& getLevel() const;
	glm::vec2 getScreenSize() const;
	glm::vec2 getScreenCenter() const;
	AbstractEditor* getCurrentEditor();
	bool debug(DebugType type) const;
	const std::vector<NS2::DirEntry>& getAssets() const { return m_assets; }

private:
	void drawToolBar();
	void drawLeftSidebar();
	void drawRightSidebar();
	void drawModals();
	void changeMode();
	void changeType(int index);
	void changeObject(int index);

	bool m_needSave = false;
	bool m_animOn = false;
	int m_onLevel = 0;
	int m_selectedTypeIndex = -1;
	int m_selectedObjectIndex = -1;
	int m_pendingTypeIndex = -1;
	int m_pendingObjectIndex = -1;
	float m_animFactor = 1.0f;
	float m_cameraScale = 1.0f;
	glm::vec2 m_cameraPos = glm::vec2(0.0f);

	std::vector<NS2::DirEntry> m_assets;
	std::vector<std::vector<std::string>> m_objects;
	std::vector<std::unique_ptr<AbstractEditor>> m_editors;
	std::vector<std::pair<unsigned char, std::string>> m_objectTypes;

	// --- Interactive Modal Dialogue Assets ---
	char m_buffer[256] = "";  // Text storage for input prompts
	ModalType m_activeModal = ModalType::NONE;
	ToolType m_activeTool = ToolType::SELECT;
	bool m_debugging[DebugType::COUNT] = {};
	std::unique_ptr<LevelEditor> m_levelEditor = nullptr;
};
