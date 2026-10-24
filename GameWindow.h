/**
 * @file GameWindow.h
 * @brief Main game window context layout handling SDL_Window and SDL_GLContext handles.
 * @author Ndongmo Silatsa Fabrice
 * @date 24-10-2016
 * @version 1.2
 */

#pragma once

#include <SDL3/SDL.h>
#include <GL/glew.h>
#include <string>

namespace NS2
{
	/**
	 * @enum WindowFlags
	 * @brief Structural flags configuration masks handling desktop application windows.
	 */
	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class GameWindow
	{
	public:
		GameWindow() = default;
		~GameWindow() { destroy(); }

		// Enforce single resource ownership semantics over active system display handles
		GameWindow(const GameWindow&) = delete;
		GameWindow& operator=(const GameWindow&) = delete;

		/**
		 * @brief Creates the main desktop application display and configures the active OpenGL context.
		 * @param windowName Title text displayed on the window border.
		 * @param screenWidth Width dimension constraint in pixels.
		 * @param screenHeight Height dimension constraint in pixels.
		 * @param currentFlags Configuration masks managing window styles (e.g., FULLSCREEN).
		 * @return True if window and OpenGL contexts create successfully, false otherwise.
		 */
		bool create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);

		/**
		 * @brief De-allocates graphics contexts and window handles safely from system memory.
		 */
		void destroy();

		/**
		 * @brief Swap the OpenGL backbuffers to present the drawn frame to the screen display.
		 */
		void swapBuffer();

		/* GETTERS */
		int getScreenWidth() const { return m_screenWidth; }
		int getScreenHeight() const { return m_screenHeight; }
		SDL_Window* getSDLWindow() const { return m_window; }
		SDL_GLContext getGLContext() const { return m_glContext; } /**< Persistent getter linking to ImGui backend setups */

	private:
		SDL_Window* m_window = nullptr;      /**< Pointer reference managing raw hardware window handles */
		SDL_GLContext m_glContext = nullptr;  /**< Active OpenGL graphics core execution context driver slot */
		int m_screenWidth = 0;              /**< Tracked layout bounding resolution width */
		int m_screenHeight = 0;             /**< Tracked layout bounding resolution height */
	};
}
