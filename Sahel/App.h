/*!
* \file App.h
* \brief Start class of the game.
* \author Ndongmo Silatsa Fabrice
* \date 21-05-2017
* \version 2.0
*/

#pragma once

#include "MenuScreen.h"
#include "GameScreen.h"
#include "EditorScreen.h"

#include <memory>
#include <IMainGame.h>

class App : public NS2::IMainGame
{
public:
	App();
	~App();

	void onInit() override;
	void onExit() override;
	void addScreens() override;

private:
	MenuScreen m_menuScreen;
	GameScreen m_gameScreen;
	EditorScreen m_editorScreen;
};

