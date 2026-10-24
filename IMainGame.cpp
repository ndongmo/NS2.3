/**
 * @file IMainGame.cpp
 * @brief Core engine system loop coordinator framework implementation.
 * @author Ndongmo Silatsa Fabrice
 * @date Updated for NS2 Architecture (Dear ImGui, SDL3, and Safe Input Cut)
 */

#include "IMainGame.h"
#include "IGameScreen.h"
#include "Logger.h"

#include <SDL3/SDL.h>
#include <GL/glew.h>
#include <iostream>

namespace NS2
{
	IMainGame::IMainGame() : m_screenManager(*this)
	{}

	bool IMainGame::init(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
	{
#ifndef _DEBUG
		// Redirect output streams to local log files securely inside production shipping distributions
		FILE* stream;
		freopen_s(&stream, "log.log", "w", stdout);
#endif

		// 1. Boot up platform input subsystems, audio pipelines, and device event monitors
		if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS)) {
			logE << "CORE_ERROR: Failed to initialize baseline SDL3 subsystems." << std::endl;
			return false;
		}

		// 2. Set strict pixel data formats and depth bounds tracking attributes for modern OpenGL
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // Upgraded from 16 to 24 bits
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		// 3. Create our desktop application window structure container
		if (!m_window.create(windowName, screenWidth, screenHeight, currentFlags)) {
			logE << "CORE_ERROR: Failed to allocate rendering window components." << std::endl;
			return false;
		}

		// 4. Bring up modern hardware text layout processing channels natively
		SDL_StartTextInput(m_window.getSDLWindow());

		// 5. Initialize your system input mapper mechanics grid coordinates
		m_eventHandler.init(screenWidth, screenHeight);

		// 6. Safely pass the active window descriptor context addresses to initialize Dear ImGui
		// Replaces legacy string schema file path pointers completely
		if (!m_gui.init(m_window.getSDLWindow(), m_window.getGLContext())) {
			logE << "CORE_ERROR: Immediate-mode GUI interface failed to compile shader pipelines." << std::endl;
			return false;
		}

		// 7. Run custom initialization rules and build out canvas state lists
		onInit();
		addScreens();

		m_currentScreen = nullptr;
		updateScreen();

		return true;
	}

	void IMainGame::updateScreen()
	{
		IGameScreen* oldScreen = m_currentScreen;

		if (m_currentScreen == nullptr) {
			m_currentScreen = m_screenManager.getCurrentScreen();
		}
		else if (m_currentScreen->geScreenState() == ScreenState::CHANGE_NEXT) {
			m_currentScreen = m_screenManager.moveNext();
		}
		else if (m_currentScreen->geScreenState() == ScreenState::CHANGE_PREVIOUS) {
			m_currentScreen = m_screenManager.movePrevious();
		}

		if (m_currentScreen != nullptr) {
			if (oldScreen != nullptr) {
				oldScreen->onExit();
			}
			m_currentScreen->onEntry();
			m_currentScreen->setScreenState(ScreenState::RUNNING);
		}
	}

	void IMainGame::handleEvents()
	{
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			// A. Pass the raw event directly to Dear ImGui FIRST!
			// It checks if the cursor is interacting with a panel widget.
			bool caughtByUI = m_gui.onSDLEvent(ev);

			// B. If the event is captured by the UI panel layout grid, return immediately.
			// This blocks inputs from leaking through into your game simulation lines.
			if (caughtByUI) {
				return;
			}

			// C. UI didn't want it; fall back to standard engine input logic maps safely
			if (m_eventHandler.getPlayWith() == PlayWith::KEYBOARD)
			{
				switch (ev.type) {
				case SDL_EVENT_QUIT:
					if (m_currentScreen) m_currentScreen->requestExit();
					break;
				case SDL_EVENT_MOUSE_MOTION:
					m_eventHandler.setMouseCoords((float)ev.motion.x, (float)ev.motion.y);
					break;
				case SDL_EVENT_KEY_DOWN:
					m_eventHandler.pressKey(ev.key.key);
					break;
				case SDL_EVENT_KEY_UP:
					m_eventHandler.releaseKey(ev.key.key);
					break;
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
					m_eventHandler.pressKey(ev.button.button);
					break;
				case SDL_EVENT_MOUSE_BUTTON_UP:
					m_eventHandler.releaseKey(ev.button.button);
					break;
				}
			}
			else
			{
				switch (ev.type) {
				case SDL_EVENT_QUIT:
					if (m_currentScreen) m_currentScreen->requestExit();
					break;
				case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
					m_eventHandler.pressKey(ev.jbutton.button + MARGIN);
					break;
				case SDL_EVENT_JOYSTICK_BUTTON_UP:
					m_eventHandler.releaseKey(ev.jbutton.button + MARGIN);
					break;
				case SDL_EVENT_JOYSTICK_AXIS_MOTION:
					m_eventHandler.updateJoystickAxis(ev.jaxis.axis, ev.jaxis.value, m_window.getSDLWindow());
					break;
				case SDL_EVENT_JOYSTICK_HAT_MOTION:
					m_eventHandler.updateJoystickHats(ev.jhat.value + MARGIN);
					break;
				}
			}
		}
	}

	void IMainGame::update()
	{
		if (m_currentScreen)
		{
			switch (m_currentScreen->geScreenState())
			{
			case ScreenState::RUNNING:
				m_currentScreen->update();
				// HINT: Immediate mode updates are now called right inside the rendering loop pass 
				// to ensure that new windows map out layout metrics cleanly.
				break;
			case ScreenState::CHANGE_NEXT:
			case ScreenState::CHANGE_PREVIOUS:
				updateScreen();
				break;
			case ScreenState::EXIT_GAME:
				m_isRunning = false;
				break;
			default:
				break;
			}
		}
		else
		{
			m_isRunning = false;
		}
	}

	void IMainGame::draw()
	{
		// Ensure drawing boundaries frame viewports wrap your full graphics windows dimensions
		glViewport(0, 0, m_window.getScreenWidth(), m_window.getScreenHeight());

		// 1. Spin up a new ImGui workspace frame grid layout tick
		m_gui.update();

		// 2. Render core game world layers (Terrains, meshes, characters, particles)
		if (m_currentScreen != nullptr) {
			m_currentScreen->draw();
		}

		// 3. Draw active ImGui panels straight over the top of your scenes
		m_gui.draw();
	}

	void IMainGame::run(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
	{
		m_isRunning = init(windowName, screenWidth, screenHeight, currentFlags);

		m_limiter.setMaxFPS(MAX_FPS);

		// --- MAIN TICK ROUTINE LOOP ---
		while (m_isRunning)
		{
			m_limiter.begin();
			{
				m_eventHandler.update(m_limiter.getFrameTime());

				handleEvents();

				update();

				draw();

				m_window.swapBuffer();
			}
			m_limiter.end();
		}

		exit();
	}

	void IMainGame::exit()
	{
		if (m_currentScreen != nullptr) {
			m_currentScreen->onExit();
		}

		// Tear down backend structures in safe allocation order to prevent dangling reference traps
		m_screenManager.destroy();
		m_eventHandler.destroy();
		m_gui.destroy();
		m_window.destroy();

		SDL_Quit();
	}

	void IMainGame::showCursor()
	{
		if (!m_eventHandler.isJoyConnect()) {
			m_gui.showMouseCursor();
		}
	}

	void IMainGame::hideCursor()
	{
		m_gui.hideMouseCursor();
	}
}
