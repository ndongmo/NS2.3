#pragma once

#include "ScreenIndices.h"
#include "MenuGui.h"

#include <IGameScreen.h>

namespace NS2{
	class IMainGame;
}

class MenuScreen : public NS2::IGameScreen
{
public:
	MenuScreen(NS2::IMainGame& game);

	virtual void build() override;
	virtual void destroy() override;

	virtual void onEntry() override;
	virtual void onExit() override;

	virtual void update() override;
	virtual void draw() override;

	virtual int getPreviousSCreenIndex() const override;

private:
	MenuGui m_menuGui; /**< Application Layer instance */
};

