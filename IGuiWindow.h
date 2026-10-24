/**
 * @file IGuiWindow.h
 * @brief Foundational abstract GUI window component with integrated style sheet serialization.
 * @author Ndongmo Silatsa Fabrice
 * @date Updated for NS2 Architecture
 */

#pragma once

#include "IGameScreen.h"

#include <string>
#include <glm/glm.hpp>
#include <imgui/imgui.h>

namespace NS2
{
	class IGuiWindow
	{
	public:
		IGuiWindow(IGameScreen& parent, const std::string& name);
		virtual ~IGuiWindow() = default;

		// Enforce single UI window instance tracking constraints
		IGuiWindow(const IGuiWindow&) = delete;
		IGuiWindow& operator=(const IGuiWindow&) = delete;

		/*
		 * @brief Pure virtual method to initialize your specific application-layer widget layouts.
		 * @param screenWidth The width of the screen in pixels.
		 * @param screenHeight The height of the screen in pixels.
		 */
		virtual void initGui();

		/**
		 * @brief Parses an engine configuration style sheet file to customize this window's theme palette.
		 * @param filePath Path to your layout or style configuration profile on disk.
		 * @return True if parsing succeeded, false if it fell back to default themes.
		 */
		bool loadStyleFromFile(const std::string& filePath);

		/**
		 * @brief The engine main loop updates and processes immediate-mode UI layouts here.
		 */
		void render();

		/** @brief Force immediate-mode visibility to snap to hidden. */
		void close() { m_visible = false; }
		/** @brief Snap window status to active visibility. */
		void open() { m_visible = true; }

		/* SETTERS */
		void setVisible(bool visible) { m_visible = visible; }
		void setPosition(const glm::vec2& p) { m_position = p; m_useCustomPos = true; }
		void setSize(const glm::vec2& s) { m_size = s; m_useCustomSize = true; }
		void setAlpha(float alpha) { m_alpha = glm::clamp(alpha, 0.0f, 1.0f); }

		/* GETTERS */
		const std::string& getName() const { return m_name; }
		bool isVisible() const { return m_visible; }
		bool isFocused() const { return m_isFocused; }
		float getAlpha() const { return m_alpha; }

	protected:
		/**
		 * @brief Pure virtual method containing your specific application-layer widget layouts.
		 * Automatically wrapped inside safety validation blocks.
		 */
		virtual void onDraw() = 0;

		std::string m_name;             /**< Unique ImGui system descriptor string identifier */
		bool m_visible = true;          /**< Visibility control toggle flag */
		bool m_isFocused = false;       /**< Focus tracking state cache */
		ImGuiWindowFlags m_flags = 0;   /**< Configuration bitmasks customized by your sub-systems */

		glm::vec2 m_position = glm::vec2(0.0f);
		glm::vec2 m_size = glm::vec2(300.0f, 400.0f);
		float m_alpha = 1.0f;           /**< Transparency multiplier overlay layer */

		bool m_useCustomPos = false;
		bool m_useCustomSize = false;

		// Individual custom window style modifications overriding global themes
		ImGuiStyle m_localStyle;
		bool m_hasCustomStyle = false;

		IGameScreen& m_parent; /**< Non-owning reference back to the parent screen context */
	};
}
