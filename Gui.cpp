/**
 * @file Gui.cpp
 * @brief Immediate-mode interface manager framework implementation.
 * @author Ndongmo Silatsa Fabrice
 * @date Updated for NS2 Architecture
 */

#include "Gui.h"
#include "Logger.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_opengl3.h>
#include <GL/glew.h>
#include <iostream>

namespace NS2
{
	bool Gui::init(SDL_Window* window, SDL_GLContext glContext)
	{
		if (m_initialized) {
			logW << "GUI_WARNING: Attempted to initialize ImGui context twice." << std::endl;
			return true;
		}

		m_windowRef = window;

		// 1. Allocate and assert the main Immediate Mode execution structure context shell
		IMGUI_CHECKVERSION();
		m_context = ImGui::CreateContext();
		if (m_context == nullptr) {
			logE << "GUI_ERROR: Structural assignment failed creating central ImGui runtime context." << std::endl;
			return false;
		}

		// 2. Establish uniform styling schemes matching modern engine dark layout states
		ImGui::StyleColorsDark();

		// 3. Bind hardware system abstraction bridges mapping device contexts seamlessly
		if (!ImGui_ImplSDL3_InitForOpenGL(window, glContext)) {
			logE << "GUI_ERROR: Failed to establish link to SDL3 platform input backends." << std::endl;
			return false;
		}

		// Initializing the programmable shader string layout configuration pipeline.
		// "#version 330 core" targets standard OpenGL 3.3 environments.
		if (!ImGui_ImplOpenGL3_Init("#version 330 core")) {
			logE << "GUI_ERROR: Failed to bind programmable GLSL draw rendering layers." << std::endl;
			return false;
		}

		return m_initialized = true;
	}

	void Gui::clear()
	{
		// Left blank intentionally: Immediate-mode interfaces completely reset their visibility states every single update frame!
	}

	void Gui::destroy()
	{
		if (m_initialized) {
			// Clean up and disconnect drivers in inverse allocation order to block segmentation traps
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplSDL3_Shutdown();
			ImGui::DestroyContext(m_context);

			m_context = nullptr;
			m_windowRef = nullptr;
			m_initialized = false;
		}
	}

	void Gui::update()
	{
		if (!m_initialized) return;

		// Time injection pulse sequences are automated entirely inside the official ImGui backends!
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
	}

	void Gui::draw()
	{
		if (!m_initialized) return;

		// 1. Finalize tracking maps and flatten active widgets down into continuous vertex commands arrays
		ImGui::Render();

		// 2. Draw geometric triangles index elements directly onto the viewport window layout frame buffer
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// 3. Turn off scissor clipping spaces to prevent interface changes from distorting regular engine 3D layers
		glDisable(GL_SCISSOR_TEST);
	}

	bool Gui::onSDLEvent(SDL_Event& event)
	{
		if (!m_initialized) return false;

		// 1. Instantly forward incoming raw events directly to the backend layer.
		ImGui_ImplSDL3_ProcessEvent(&event);

		// 2. Route joystick controller attachments into internal engine structures natively
		if (event.type == SDL_EVENT_JOYSTICK_BUTTON_DOWN || 
			event.type == SDL_EVENT_JOYSTICK_BUTTON_UP || 
			event.type == SDL_EVENT_JOYSTICK_HAT_MOTION) {
			sdlJoystickTOimgui(event);
		}

		// 3. Query the UI engine to see if the cursor is interacting with an interface window.
		// If true, return true so your outer game loops can ignore these inputs and prevent duplicate actions.
		ImGuiIO& io = ImGui::GetIO();
		if (event.type >= SDL_EVENT_MOUSE_MOTION && event.type <= SDL_EVENT_MOUSE_WHEEL) {
			return io.WantCaptureMouse;
		}
		if (event.type >= SDL_EVENT_KEY_DOWN && event.type <= SDL_EVENT_KEY_UP) {
			return io.WantCaptureKeyboard;
		}

		return false;
	}

	void Gui::sdlJoystickTOimgui(const SDL_Event& event)
	{
		ImGuiIO& io = ImGui::GetIO();

		if (event.type == SDL_EVENT_JOYSTICK_BUTTON_DOWN || event.type == SDL_EVENT_JOYSTICK_BUTTON_UP) {
			bool isDown = (event.type == SDL_EVENT_JOYSTICK_BUTTON_DOWN);
			Uint8 button = event.jbutton.button;

			// Translate joystick button down/up events into native ImGui virtual key maps directly
			// Replace numerical constants with your layout enum configurations (e.g., Joystick::A)
			if (button == 0)      io.AddKeyEvent(ImGuiKey_GamepadFaceDown, isDown);  // Button A -> Accept
			else if (button == 1) io.AddKeyEvent(ImGuiKey_GamepadFaceRight, isDown); // Button B -> Cancel
			else if (button == 2) io.AddKeyEvent(ImGuiKey_GamepadFaceLeft, isDown);  // Button X
			else if (button == 3) io.AddKeyEvent(ImGuiKey_GamepadFaceUp, isDown);    // Button Y
		}
		else if (event.type == SDL_EVENT_JOYSTICK_HAT_MOTION) {
			if (event.jhat.hat == 0) {
				Uint8 value = event.jhat.value;

				// Map Directional D-Pad hats straight into ImGui navigation keys
				io.AddKeyEvent(ImGuiKey_GamepadDpadUp, (value & SDL_HAT_UP) != 0);
				io.AddKeyEvent(ImGuiKey_GamepadDpadDown, (value & SDL_HAT_DOWN) != 0);
				io.AddKeyEvent(ImGuiKey_GamepadDpadLeft, (value & SDL_HAT_LEFT) != 0);
				io.AddKeyEvent(ImGuiKey_GamepadDpadRight, (value & SDL_HAT_RIGHT) != 0);
			}
		}
	}

	void Gui::showMouseCursor()
	{
		SDL_ShowCursor();
	}

	void Gui::hideMouseCursor()
	{
		SDL_HideCursor();
	}

	void Gui::setFontScale(float scale)
	{
		if (!m_initialized) return;
		ImGuiIO& io = ImGui::GetIO();
		io.FontGlobalScale = scale;
	}
}
