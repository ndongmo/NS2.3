#include "App.h"

#include <ScreenManager.h>

App::App() : 
	IMainGame(), 
	m_menuScreen(*this), 
	m_gameScreen(*this), 
	m_editorScreen(*this)
{}

App::~App()
{

}

void App::onInit()
{
	
}

void App::onExit()
{

}

void App::addScreens()
{
	m_screenManager.addScreen(&m_menuScreen);
	m_screenManager.addScreen(&m_gameScreen);
	m_screenManager.addScreen(&m_editorScreen);
	m_screenManager.setScreen(m_menuScreen.getScreenIndex());
}
