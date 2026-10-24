#include "MenuScreen.h"

#include <ScreenManager.h>
#include <IGameScreen.h>
#include <IMainGame.h>

#define CAM_FOV 60.f

MenuScreen::MenuScreen(NS2::IMainGame& game) : 
	NS2::IGameScreen(game), m_menuGui(*this)
{
	m_screenIndex = SCREEN_INDEX_MENU;
	m_nextScreenIndex = SCREEN_INDEX_GAME;
}

void MenuScreen::build()
{}

void MenuScreen::destroy()
{
	
}

void MenuScreen::onEntry()
{
	float screenWidth = (float)m_game.getWindow().getScreenWidth();
	float screenHeight = (float)m_game.getWindow().getScreenHeight();

	m_camera.init(screenWidth, screenHeight);
	m_camera.setPosition(glm::vec3(screenWidth / 2.0f, screenHeight / 2.0f, 0.0f));
	m_camera.update();

	m_menuGui.initGui();

	m_state = NS2::ScreenState::RUNNING;
}

void MenuScreen::onExit()
{
	m_game.getGui().clear();
}

void MenuScreen::update()
{
	m_camera.update();

	// Access engine input system seamlessly
	if (getEventHandler().isKeyPressed(SDLK_ESCAPE)) {
		setScreenState(NS2::ScreenState::EXIT_GAME);
	}
}

void MenuScreen::draw()
{
	m_menuGui.render();
}

int MenuScreen::getPreviousSCreenIndex() const
{
	return SCREEN_INDEX_NO_SCREEN;
}
