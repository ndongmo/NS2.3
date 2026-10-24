/**
 * @file MainMenuGui.cpp
 * @brief Application Layer: Main menu window layout rendering logic.
 */

#include "MenuGui.h"
#include "ScreenIndices.h"
#include <IMainGame.h>
#include <imgui/imgui.h>

MenuGui::MenuGui(NS2::IGameScreen& parent) : NS2::IGuiWindow(parent, "Game Main Menu Window")
{
	m_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
}

void MenuGui::initGui()
{
	NS2::IGuiWindow::initGui();

	int screenWidth = (int)m_parent.getGame().getWindow().getScreenWidth();
	int screenHeight = (int)m_parent.getGame().getWindow().getScreenHeight();

	loadStyleFromFile("../Gui/Styles/main_menu.conf");
	setSize(glm::vec2(screenWidth, screenHeight));
	setPosition(glm::vec2(0.0f, 0.0f));
}

void MenuGui::onDraw()
{
	ImVec2 buttonDimensions(200.0f, 48.0f);
	float centeredButtonX = (ImGui::GetWindowSize().x - buttonDimensions.x) * 0.5f;
	float centeredButtonY = (ImGui::GetWindowSize().y - buttonDimensions.y) * 0.5f;

	ImGui::SetCursorPos(ImVec2(centeredButtonX, centeredButtonY));
	ImGui::Text("WELCOME TO THE ADVENTURE");
	ImGui::Separator();

	centeredButtonY += 60.0f;

	// Start Game Button
	ImGui::SetCursorPos(ImVec2(centeredButtonX, centeredButtonY));
	if (ImGui::Button("START GAME", buttonDimensions)) {
		m_parent.setNextScreenIndex(SCREEN_INDEX_GAME);
		m_parent.setScreenState(NS2::ScreenState::CHANGE_NEXT);
	}

	centeredButtonY += 60.0f;

	// Options Button
	ImGui::SetCursorPos(ImVec2(centeredButtonX, centeredButtonY));
	if (ImGui::Button("OPTIONS", buttonDimensions)) {
		// Handle game options panel here
	}

	centeredButtonY += 60.0f;

	// Start Editor Button
	ImGui::SetCursorPos(ImVec2(centeredButtonX, centeredButtonY));
	if (ImGui::Button("EDIT GAME", buttonDimensions)) {
		m_parent.setNextScreenIndex(SCREEN_INDEX_EDITOR);
		m_parent.setScreenState(NS2::ScreenState::CHANGE_NEXT);
	}

	centeredButtonY += 60.0f;

	// Quit Button
	ImGui::SetCursorPos(ImVec2(centeredButtonX, centeredButtonY));
	if (ImGui::Button("QUIT TO DESKTOP", buttonDimensions)) {
		m_parent.setScreenState(NS2::ScreenState::EXIT_GAME);
	}
}
