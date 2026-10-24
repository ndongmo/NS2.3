/**
 * @file IMainGame.h
 * @brief Main engine application wrapper updated for modern NS2 structures.
 * @author Ndongmo Silatsa Fabrice
 * @date Updated for NS2 Architecture (Dear ImGui & Synchronous Caching Integration)
 */

#pragma once

#include "FPSLimiter.h"
#include "GameWindow.h"
#include "EventHandler.h"
#include "ScreenManager.h"
#include "ResourceManager.h"
#include "Gui.h"

#include <SDL3/SDL.h>
#include <string>
#include <memory>

namespace NS2
{
#define MAX_FPS 60.0f
#define MARGIN 500 /* Avoid intersection between keyboard ID and gamepad ID */

	class IGameScreen;

	/**
	 * @class IMainGame
	 * @brief Foundational engine controller base architecture managing window contexts and systems loops.
	 */
	class IMainGame
	{
	public:
		IMainGame();
		virtual ~IMainGame() = default;

		// Disable copy operations to protect active hardware driver subsystem components
		IMainGame(const IMainGame&) = delete;
		IMainGame& operator=(const IMainGame&) = delete;

		/**
		 * @brief Starts core frameworks, creates the graphics context, and enters the main engine loop.
		 * @param windowName Title displayed on the created desktop window border.
		 * @param screenWidth Width dimension constraint in pixels.
		 * @param screenHeight Height dimension constraint in pixels.
		 * @param currentFlags Configuration masks passed to the underlying window driver layer.
		 */
		void run(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags = 0);

		/**
		 * @brief Signals the engine simulation loop to spin down cleanly on the next frame calculation pass.
		 */
		void exit();

		/**
		 * @brief Routes underlying window inputs down to the interface manager and core input listeners.
		 * @param ev Raw input frame event packet structure passed directly from SDL.
		 */
		void handleEvents();

		/** @brief Custom initialization hook executed immediately after basic engine subsystems come online. */
		virtual void onInit() = 0;

		/** @brief Custom shutdown hook executed before window and graphics contexts are torn down. */
		virtual void onExit() = 0;

		/** @brief Allocates and links scene states inside our central screen tracking manager layers. */
		virtual void addScreens() = 0;

		/** @brief Restores the visibility of the primary desktop system pointer cursor. */
		void showCursor();

		/** @brief Restores the invisibility of the primary desktop system pointer cursor. */
		void hideCursor();

		/* SETTERS */
		void setMaxFPS(float max) { m_limiter.setMaxFPS(max); }

		/* GETTERS */
		EventHandler& getEventHandler() { return m_eventHandler; }
		ResourceManager& getResourceManager() { return m_resourceManager; }
		GameWindow& getWindow() { return m_window; }
		Gui& getGui() { return m_gui; }
		float getFps() const { return m_limiter.getFPS(); }
		float getElapsedTime() const { return m_limiter.getFrameTime() / 1000.0f; }

	protected:
		/** @brief Steps game state trackers forward by utilizing frame delta values. */
		virtual void update();

		/** @brief Orchestrates clearing buffers and running draw calls for active engine render channels. */
		virtual void draw();

		/** @brief Runs foundational structural allocations bringing windows and graphics hardware pipelines online. */
		bool init(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);

		/** @brief Validates display state shifts inside screen managers to update screen pointers safely. */
		void updateScreen();

		ScreenManager m_screenManager;              /**< Central scene and level tracking layout state system */
		IGameScreen* m_currentScreen = nullptr;     /**< Observer tracking our active scene state frame context */
		GameWindow m_window;                        /**< Core viewport window holder containing hardware handles */
		EventHandler m_eventHandler;                /**< Native keyboard, mouse, and game controller input hub */
		ResourceManager m_resourceManager;			/**< The unique, non-static Asset Engine Instance */
		Gui m_gui;                                  /**< Modern Dear ImGui overlay engine component manager */
		bool m_isRunning = false;                   /**< Global simulation loop execution state tracking flag */

	private:
		FPSLimiter m_limiter;                       /**< Clock system used to restrict frame execution times */
	};
}
