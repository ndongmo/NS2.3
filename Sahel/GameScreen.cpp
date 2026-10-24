/**
 * @file GameScreen.cpp
 * @brief Game simulation viewport handling high-speed navigation loops and lighting overrides.
 */

#include "GameScreen.h"
#include <Logger.h>
#include <ScreenManager.h>
#include <IGameScreen.h>
#include <IMainGame.h>
#include <ImageLoader.h>

inline constexpr float DESIRED_FPS = 60.0f;
inline constexpr int MAX_PHYSICS_STEPS = 6;

inline constexpr float CAM_SCALE = 0.5f;
inline constexpr float CAM_SCALE_INCR = 0.1f;
inline constexpr float CAM_MIN_SCALE = 0.2f;
inline constexpr float CAM_MAX_SCALE = 2.0f;

GameScreen::GameScreen(NS2::IMainGame& game) 
	: NS2::IGameScreen(game), m_objManager(m_game.getResourceManager()), 
	m_renderer(m_physicEng)
{
	m_screenIndex = SCREEN_INDEX_GAME;
	m_nextScreenIndex = SCREEN_INDEX_NO_SCREEN;
}

void GameScreen::build() {}
void GameScreen::destroy() {}

void GameScreen::onEntry()
{
	float screenWidth = (float)m_game.getWindow().getScreenWidth();
	float screenHeight = (float)m_game.getWindow().getScreenHeight();

	m_game.setMaxFPS(DESIRED_FPS);

	m_camera.init(screenWidth, screenHeight);
	m_camera.setScale(CAM_SCALE);
	m_camera.setPosition(glm::vec2(screenWidth, screenHeight));
	m_camera.update();

	m_renderer.init(screenWidth, screenHeight, false);
	m_physicEng.init(MAX_PHYSICS_STEPS);
	//m_level.init(&m_camera, &m_physicEng);

	m_objManager.loadDirect("../Objects/Level/level_1.json", m_level);

	m_state = NS2::ScreenState::RUNNING;
}

void GameScreen::onExit()
{
	m_level.clear();
	m_objManager.purgeCache();
	m_physicEng.dispose();
	m_renderer.dispose();
	m_game.getGui().clear();
}

void GameScreen::update()
{
	float deltaTime = m_game.getElapsedTime();

	handleInputEvent(deltaTime);
	m_physicEng.step(m_level, deltaTime);
	m_camera.update();
}

void GameScreen::draw()
{
	m_renderer.render(m_camera, m_level);
}

int GameScreen::getPreviousSCreenIndex() const 
{ 
	return SCREEN_INDEX_MENU;
}

void GameScreen::handleInputEvent(float deltaTime)
{
	float stepVelocity = 200.0f * deltaTime;
	glm::vec2 currentPos = m_camera.getPosition();

	// Translate position parameters relative to your orientation angles
	if (getEventHandler().isKeyDown(SDLK_W)) currentPos.y += stepVelocity; // Move Forward
	if (getEventHandler().isKeyDown(SDLK_S)) currentPos.y -= stepVelocity; // Move Backward
	if (getEventHandler().isKeyDown(SDLK_A)) currentPos.x -= stepVelocity;   // Strafe Left
	if (getEventHandler().isKeyDown(SDLK_D)) currentPos.x += stepVelocity;   // Strafe Right

	// Update positions tracking and notify the camera to recompute matrices
	m_camera.setPosition(currentPos);
	m_camera.update();

	// Release cursor handles safely if hitting Escape to navigate back to menus
	if (getEventHandler().isKeyPressed(SDLK_ESCAPE)) {
		setScreenState(NS2::ScreenState::CHANGE_PREVIOUS);
	}
}
