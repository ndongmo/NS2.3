#pragma once

#include "ScreenIndices.h"
#include "GameRenderer.h"
#include "ObjectManager.h"
#include "PhysicsEngine.h"
#include "Level.h"

#include <IGameScreen.h>

namespace NS2 {
	class IMainGame;
}

class GameScreen : public NS2::IGameScreen
{
public:
	GameScreen(NS2::IMainGame& game);

	virtual void build() override;
	virtual void destroy() override;

	virtual void onEntry() override;
	virtual void onExit() override;

	virtual void update() override;
	virtual void draw() override;

	virtual int getPreviousSCreenIndex() const override;

private:
	void handleInputEvent(float deltaTime);

	Level m_level;
	ObjectManager m_objManager;
	PhysicsEngine m_physicEng;
	GameRenderer m_renderer;
};

