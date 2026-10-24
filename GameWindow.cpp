/**
 * @file GameWindow.cpp
 * @brief Window manager setup and hardware pixel context creation implementation.
 * @author Ndongmo Silatsa Fabrice
 * @date Updated for NS2 Architecture
 */

#include "GameWindow.h"
#include "Logger.h"
#include <iostream>

namespace NS2
{
	bool GameWindow::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		// 1. Instantiate property collections matching modern SDL3 specifications [1]
		SDL_PropertiesID props = SDL_CreateProperties();
		SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, windowName.c_str());
		SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, m_screenWidth);
		SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, m_screenHeight);
		SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, SDL_WINDOWPOS_CENTERED);
		SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, SDL_WINDOWPOS_CENTERED);
		SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN, true);

		// 2. Evaluate and append configuration masks directly to the property sheet properties
		if (currentFlags & INVISIBLE) {
			SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN, true);
		}
		if (currentFlags & FULLSCREEN) {
			SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, true);
		}
		if (currentFlags & BORDERLESS) {
			SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, true);
		}

		// 3. Construct the window system structure utilizing the property sheet parameters [1]
		m_window = SDL_CreateWindowWithProperties(props);
		SDL_DestroyProperties(props); // Safe to discard properties handle immediately after generation loop pass

		if (m_window == nullptr) {
			logE << "SDL window could not be created !" << std::endl;
			return false;
		}

		// 4. Generate the persistent device driver mapping context [1]
		m_glContext = SDL_GL_CreateContext(m_window);
		if (m_glContext == nullptr) {
			logE << "SDL_GL context could not be created!" << std::endl;
			destroy(); // Safe tear down if driver allocation falls through
			return false;
		}

		// 5. Connect function extension maps safely via GLEW [1]
		glewExperimental = GL_TRUE;
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			logE << "Could not initialize GLEW!" << std::endl;
			destroy();
			return false;
		}

		// Configure structural baseline hardware clear parameters [1]
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0);

		// Control hardware frame vertical refresh limits [1]
		SDL_GL_SetSwapInterval(0);

		// Activate baseline structural alpha color blending maps [1]
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return true;
	}

	void GameWindow::swapBuffer() {
		if (m_window != nullptr) {
			SDL_GL_SwapWindow(m_window);
		}
	}

	void GameWindow::destroy() {
		// Secure clean disconnection routines to protect system state paths from dangling traps
		if (m_glContext != nullptr) {
			SDL_GL_DestroyContext(m_glContext);
			m_glContext = nullptr;
		}

		if (m_window != nullptr) {
			SDL_DestroyWindow(m_window);
			m_window = nullptr;
		}
	}
}
