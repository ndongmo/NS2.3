/**
 * @file IGuiWindow.cpp
 * @brief Abstract structural frame wrapper and basic theme parsing rules implementation.
 */

#include "IGuiWindow.h"
#include "Logger.h"
#include <fstream>
#include <sstream>
#include <algorithm>

namespace NS2
{
	IGuiWindow::IGuiWindow(IGameScreen& parent, const std::string& name) : 
		m_parent(parent), m_name(name) { }

	void IGuiWindow::initGui()
	{
		m_localStyle = ImGui::GetStyle();
	}

	void IGuiWindow::render()
	{
		// 1. Skip entire execution loops if visibility flag parameters are turned off
		if (!m_visible) return;

		// 2. Inject positional runtime layout overrides if requested by software controls
		if (m_useCustomPos) {
			ImGui::SetNextWindowPos(ImVec2(m_position.x, m_position.y), ImGuiCond_Always);
		}
		if (m_useCustomSize) {
			ImGui::SetNextWindowSize(ImVec2(m_size.x, m_size.y), ImGuiCond_Always);
		}

		// Apply custom visibility alpha values
		ImGui::SetNextWindowBgAlpha(m_alpha);

		// 3. Inject local theme overrides before opening immediate mode containers
		if (m_hasCustomStyle) {
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, m_localStyle.WindowRounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, m_localStyle.FrameRounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, m_localStyle.FramePadding);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, m_localStyle.ItemSpacing);
			// Override specific color maps
			for (int i = 0; i < ImGuiCol_COUNT; ++i) {
				ImGui::PushStyleColor(i, m_localStyle.Colors[i]);
			}
		}

		// 4. Open the structural immediate mode panel bounds
		if (ImGui::Begin(m_name.c_str(), &m_visible, m_flags))
		{
			m_isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

			// 5. Fire your application's derived widget compilation layout structures safely
			onDraw();
		}
		ImGui::End();

		// 6. Cleanly clean up stack modifications to avoid polluting surrounding widgets
		if (m_hasCustomStyle) {
			ImGui::PopStyleVar(4);
			ImGui::PopStyleColor(ImGuiCol_COUNT);
		}
	}

	bool IGuiWindow::loadStyleFromFile(const std::string& filePath)
	{
		std::ifstream file(filePath);
		if (!file.is_open()) {
			logE << "GUI_WINDOW: Failed to parse style layout profile path: " << filePath << std::endl;
			return false;
		}

		std::string line;
		while (std::getline(file, line))
		{
			// Eliminate structural comment vectors or spaces lines
			if (line.empty() || line[0] == '#') continue;

			std::istringstream stream(line);
			std::string parameterKey;
			stream >> parameterKey;

			// Map fundamental structural boundary attributes cleanly
			if (parameterKey == "WindowRounding:") {
				stream >> m_localStyle.WindowRounding;
			}
			else if (parameterKey == "FrameRounding:") {
				stream >> m_localStyle.FrameRounding;
			}
			// Map specific vector components directly into active palette arrays
			else if (parameterKey == "ColorButton:") {
				float r, g, b, a;
				stream >> r >> g >> b >> a;
				m_localStyle.Colors[ImGuiCol_Button] = ImVec4(r, g, b, a);
			}
			else if (parameterKey == "ColorText:") {
				float r, g, b, a;
				stream >> r >> g >> b >> a;
				m_localStyle.Colors[ImGuiCol_Text] = ImVec4(r, g, b, a);
			}
			else if (parameterKey == "FramePadding:") {
				float padX, padY;
				stream >> padX >> padY;
				m_localStyle.FramePadding = ImVec2(padX, padY);
			}
			else if (parameterKey == "ItemSpacing:") {
				float spaceX, spaceY;
				stream >> spaceX >> spaceY;
				m_localStyle.ItemSpacing = ImVec2(spaceX, spaceY);
			}
		}

		file.close();
		m_hasCustomStyle = true;
		return true;
	}
}
