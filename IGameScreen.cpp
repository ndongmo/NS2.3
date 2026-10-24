#include "IGameScreen.h"
#include "IMainGame.h"

namespace NS2
{
	EventHandler& IGameScreen::getEventHandler()
	{
		return m_game.getEventHandler();
	}
	IGameScreen::IGameScreen(IMainGame& game)
		: m_game(game)
	{}
}