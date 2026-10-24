#include "ScreenManager.h"
#include "IGameScreen.h"
#include "IMainGame.h"

namespace NS2
{
	ScreenManager::ScreenManager(IMainGame& game)
		: m_game(game)	
	{}

	IGameScreen* ScreenManager::moveNext()
	{
		IGameScreen* currentScreen = getCurrentScreen();
		if (currentScreen->getNextSCreenIndex() != SCREEN_INDEX_NO_SCREEN)
		{
			m_currentScreenIndex = currentScreen->getNextSCreenIndex();
		}

		return getCurrentScreen();
	}

	IGameScreen* ScreenManager::movePrevious()
	{
		IGameScreen* currentScreen = getCurrentScreen();
		if (currentScreen->getPreviousSCreenIndex() != SCREEN_INDEX_NO_SCREEN)
		{
			m_currentScreenIndex = currentScreen->getPreviousSCreenIndex();
		}

		return getCurrentScreen();
	}

	void ScreenManager::addScreen(IGameScreen* screen)
	{
		screen->build();
		screen->setScreenIndex(static_cast<int>(m_screens.size()));
		m_screens.push_back(screen);
	}
	void ScreenManager::setScreen(int screenIndex)
	{
		m_currentScreenIndex = screenIndex;
	}

	void ScreenManager::destroy()
	{
		for (size_t i = 0; i < m_screens.size(); i++)
		{
			m_screens[i]->destroy();
		}

		m_screens.clear();
		m_currentScreenIndex = SCREEN_INDEX_NO_SCREEN;
	}

	IGameScreen* ScreenManager::getCurrentScreen()
	{
		if (m_currentScreenIndex != SCREEN_INDEX_NO_SCREEN)
			return m_screens[m_currentScreenIndex];
		else
			return nullptr;
	}
}