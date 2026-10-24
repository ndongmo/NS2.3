/**
 * @file MenuGui.h
 * @brief Application Layer: Immediate-Mode Main Menu widget overlay layout.
 */

#pragma once

#include <IGuiWindow.h>

 /**
  * @class MenuGui
  * @brief Handles drawing the specific button layout for this game application.
  */
class MenuGui : public NS2::IGuiWindow
{
public:
	MenuGui(NS2::IGameScreen& parent);
	~MenuGui() = default;

	void initGui() override;
	void onDraw() override;

private:
};
