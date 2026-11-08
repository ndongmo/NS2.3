#pragma once

#include "ScreenIndices.h"
#include "GameRenderer.h"
#include "ObjectManager.h"
#include "PhysicsEngine.h"
#include "Level.h"
#include "EditorGui.h"

#include <IGameScreen.h>

namespace NS2 {
	class IMainGame;
}

class EditorScreen : public NS2::IGameScreen
{
public:
	EditorScreen(NS2::IMainGame& game);

	virtual void build() override;
	virtual void destroy() override;

	virtual void onEntry() override;
	virtual void onExit() override;

	virtual void update() override;
	virtual void draw() override;
	virtual void requestExit() override;

	virtual int getPreviousSCreenIndex() const override;

	Level& getLevel() { return m_level; }
	NS2::Camera& getCamera() { return m_camera; }
	GameRenderer& getGameRenderer() { return m_renderer; }
	ObjectManager& getObjectManager() { return m_objManager; }

private:
	void handleInputEvent(float deltaTime);

	bool m_dragging = false;
	bool m_onUi = false;
	glm::vec2 m_selectOffset = glm::vec3(0.0f);
	glm::vec2 m_dragInit = glm::vec3(0.0f);

	EditorGui m_editorGui; /**< Editor GUI instance */

	Level m_level;
	ObjectManager m_objManager;
	PhysicsEngine m_physicEng;
	GameRenderer m_renderer;
};

