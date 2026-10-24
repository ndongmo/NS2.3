#pragma once
/*!
* \file IGameScreen.h
* \brief Abstract game screen.
* \author Ndongmo Silatsa Fabrice
* \date 24-10-2016
* \version 1.0
*/

#pragma once

#include "Camera.h"

namespace NS2
{
	class IMainGame;
	class EventHandler;

	/*!
	* \brief Different states of screens.
	*/
	enum class ScreenState
	{
		NONE,
		RUNNING,
		EXIT_GAME,
		CHANGE_NEXT,
		CHANGE_PREVIOUS
	};

	class IGameScreen
	{
	public:
		IGameScreen(IMainGame& game);
		/*!
		* \brief Build screen design.
		*/
		virtual void build() = 0;
		/*!
		* \brief Destroy all screen stuffs.
		*/
		virtual void destroy() = 0;
		/*!
		* \brief Called when screen enters focus.
		*/
		virtual void onEntry() = 0;
		/*!
		* \brief Called when screen exits focus.
		*/
		virtual void onExit() = 0;

		/*!
		* \brief Update dynamic screen objects. Called in the main game loop.
		*/
		virtual void update() = 0;
		/*!
		* \brief Draw screen stuffs.
		*/
		virtual void draw() = 0;
		/*!
		* \brief Request to exit the current screen.
		*/
		virtual void requestExit() { 
			setScreenState(ScreenState::CHANGE_PREVIOUS);
		}
		/*!
		* \brief Return the index of previous screen when changing screens.
		* \return previous screen index.
		*/
		virtual int getPreviousSCreenIndex() const = 0;
		/*!
		* \brief Return the index of next screen when changing screens.
		* \return next screen index.
		*/
		int getNextSCreenIndex() const { return m_nextScreenIndex; }
		/*!
		* \brief Return current screen index.
		*/
		int getScreenIndex() const { return m_screenIndex; }
		/*!
		* \brief Return current screen state.
		*/
		ScreenState geScreenState() const { return m_state; }

		/* GETTERS */
		EventHandler& getEventHandler();
		IMainGame& getGame() { return m_game; }

		// SETTERS
		void setScreenState(ScreenState state)
		{
			if (m_screenIndex == 0 && state == ScreenState::CHANGE_PREVIOUS)
				m_state = ScreenState::EXIT_GAME;
			else
				m_state = state;
		}
		void setScreenIndex(int index) { m_screenIndex = index; }
		void setNextScreenIndex(int index) { m_nextScreenIndex = index; }

	protected:
		int m_screenIndex = -1;						/* Current screen index */
		int m_nextScreenIndex = -1;					/* Next screen index */
		ScreenState m_state = ScreenState::NONE;	/* State of current screen */
		IMainGame& m_game;							/* Pointer to main game class */
		Camera m_camera;							/* Inherited perspective camera system instance */
	};
}
