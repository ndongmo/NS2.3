#include "EditorScreen.h"

#include <ScreenManager.h>
#include <IGameScreen.h>
#include <IMainGame.h>

const int MOUSE_LEFT = 0;
const int MOUSE_RIGHT = 1;
const float SENSITIVITY = 0.12f;
const float MOVE_SPEED = 10.0f;

EditorScreen::EditorScreen(NS2::IMainGame& game) : 
	NS2::IGameScreen(game),
	m_objManager(m_game.getResourceManager()),
	m_renderer(m_physicEng), m_editorGui(*this)
{
	m_screenIndex = SCREEN_INDEX_EDITOR;
	m_nextScreenIndex = SCREEN_INDEX_NO_SCREEN;
}

void EditorScreen::build() { }

void EditorScreen::destroy() { }

void EditorScreen::onEntry()
{
	float screenWidth = (float)m_game.getWindow().getScreenWidth();
	float screenHeight = (float)m_game.getWindow().getScreenHeight();

	m_camera.init(screenWidth, screenHeight);
	m_camera.setPosition(glm::vec3(screenWidth / 2.0f, screenHeight / 2.0f, 0.0f));
	m_camera.update();

	TypeHelper::init();

	m_editorGui.initGui();

	m_renderer.init(screenWidth, screenHeight, true);
	m_physicEng.init(0);

	m_state = NS2::ScreenState::RUNNING;
}

void EditorScreen::onExit()
{
	m_editorGui.clear();
	m_level.clear();
	m_objManager.purgeCache();
	m_physicEng.dispose();
	m_renderer.dispose();
	m_game.getGui().clear();
	TypeHelper::clear();
}

void EditorScreen::update()
{
	m_camera.update();
}

void EditorScreen::draw()
{
	m_editorGui.render();
	m_renderer.render(m_camera, m_level, m_editorGui);
}

void EditorScreen::requestExit()
{
	m_editorGui.onExit();
}

int EditorScreen::getPreviousSCreenIndex() const
{
	return SCREEN_INDEX_MENU;
}

void EditorScreen::handleInputEvent(float deltaTime)
{
	
}