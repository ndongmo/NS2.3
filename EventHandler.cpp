/**
 * @file EventHandler.cpp
 * @brief Multi-device input mapping and relative 3D camera coordinate translation implementation.
 * @author Ndongmo Silatsa Fabrice
 * @date Updated to SDL3/C++20 for NS2 3D Architecture
 */

#include "EventHandler.h"
#include "Logger.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cmath>

#define KEYS_CONF "padconfig.conf"
#define JOY_AXE_DEFAULT_SPEED 30
#define AXIS_TIMELAPSE 50.0f
#define KEY_HOLD_TIMER 1000.0f

namespace NS2
{
	void EventHandler::init(int screenWidth, int screenHeight)
	{
		int count = 0;
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;
		m_mouseCoords = glm::vec2(0.0f);
		m_joyAxeValue = glm::vec2(0.0f);
		m_playWith = PlayWith::KEYBOARD;
		m_axisTimeLapse = 0.0f;

		// 1. Identify and connect the primary physical Gamepad layout device via SDL3 APIs
		SDL_JoystickID* joysticks = SDL_GetJoysticks(&count);
		if (joysticks && count > 0) {
			SDL_JoystickID instance_id = joysticks[0];
			logI << "INPUT_SYSTEM: Opening Gamepad device [" << instance_id << "] -> " <<
				std::string(SDL_GetJoystickNameForID(instance_id)) << std::endl;

			m_gamepad = SDL_OpenGamepad(instance_id);
			SDL_free(joysticks);
		}

		// 2. Load custom input mapping properties files if existing on the storage disk
		if (!loadConfigFile()) {
			clearConfig(); // Pre-bake structural fallback defaults if configuration profile is absent
		}
	}

	void EventHandler::destroy()
	{
		if (m_gamepad != nullptr) {
			SDL_CloseGamepad(m_gamepad);
			m_gamepad = nullptr;
		}
		m_keyMap.clear();
		m_keyPressed.clear();
		m_keyTimer.clear();
	}

	void EventHandler::resetPressedTimer()
	{
		for (auto& [key, isDown] : m_keyMap) {
			if (!isDown) {
				m_keyTimer[key] = 0.0f;
			}
		}
	}

	void EventHandler::update(float deltaTime)
	{
		// 1. HIGH-PERFORMANCE HOT-PLUGGING: Track gamepad connection state changes cleanly.
		// Avoids executing allocations every single frame tick when a controller stays bound.
		int count = 0;
		SDL_JoystickID* joysticks = SDL_GetJoysticks(&count);

		if (joysticks && count > 0 && m_gamepad == nullptr)
		{
			SDL_JoystickID instance_id = joysticks[0];
			logI << "INPUT_SYSTEM: Hot-plugged Gamepad discovered! Opening [" << instance_id << "] " <<
				std::string(SDL_GetJoystickNameForID(instance_id)) << std::endl;

			m_gamepad = SDL_OpenGamepad(instance_id);
			m_playWith = PlayWith::GAMEPAD;
			SDL_free(joysticks);
		}
		else if (!joysticks && m_gamepad != nullptr)
		{
			logW << "INPUT_SYSTEM: Gamepad disconnected! Switching input modes to Keyboard fallback." << std::endl;
			SDL_CloseGamepad(m_gamepad);
			m_gamepad = nullptr;
			m_playWith = PlayWith::KEYBOARD;
		}
		else if (joysticks) {
			SDL_free(joysticks); // Release transient array lists buffer safely out of memory
		}

		// 2. Advance time decay metrics evaluating active keyboard buttons and input hold states
		for (auto& [key, isDown] : m_keyMap) {
			m_keyPressed[key] = false; // Reset transient frame tap logs
			if (isDown) {
				m_keyTimer[key] += deltaTime;
			}
			else {
				m_keyTimer[key] = 0.0f;
			}
		}

		if (m_gamepad != nullptr) {
			m_axisTimeLapse += deltaTime;
		}
	}

	void EventHandler::pressKey(uint32_t keyID) {
		m_keyMap[keyID] = true;
	}

	void EventHandler::releaseKey(uint32_t keyID) {
		// Preservation logic layout mapping your exact original code structural patterns
		m_keyPressed[keyID] = !isKeyHold(keyID);
		m_keyTimer[keyID] = 0.0f;
		m_keyMap[keyID] = false;
	}

	void EventHandler::releaseEvent(uint32_t eventID) {
		const uint32_t keyID = getKeyID(eventID);
		m_keyPressed[keyID] = false;
		m_keyMap[keyID] = false;
		m_keyTimer[keyID] = 0.0f;
	}

	bool EventHandler::isEventDown(uint32_t eventID) { return isKeyDown(getKeyID(eventID)); }
	bool EventHandler::isEventPressed(uint32_t eventID) { return isKeyPressed(getKeyID(eventID)); }
	bool EventHandler::isEventHold(uint32_t eventID) { return isKeyHold(getKeyID(eventID)); }

	bool EventHandler::isEventPressedRel(uint32_t eventID)
	{
		const uint32_t keyID = getKeyID(eventID);
		const bool isPressed = isKeyPressed(keyID);
		if (isPressed) {
			m_keyPressed[keyID] = false;
			m_keyMap[keyID] = false;
			m_keyTimer[keyID] = 0.0f;
		}
		return isPressed;
	}

	bool EventHandler::isKeyDown(uint32_t keyID) {
		auto it = m_keyMap.find(keyID);
		return (it != m_keyMap.end()) ? it->second : false;
	}

	bool EventHandler::isKeyPressed(uint32_t keyID) {
		auto it = m_keyPressed.find(keyID);
		return (it != m_keyPressed.end()) ? it->second : false;
	}

	bool EventHandler::isKeyHold(uint32_t keyID)
	{
		auto it = m_keyMap.find(keyID);
		if (it != m_keyMap.end()) {
			return it->second && m_keyTimer[it->first] >= KEY_HOLD_TIMER;
		}
		return false;
	}

	void EventHandler::setMouseCoords(float x, float y) {
		m_mouseCoords.x = x;
		m_mouseCoords.y = y;
	}

	void EventHandler::updateJoystickAxis(int axe, int value, SDL_Window* window)
	{
		// Axis 0: Left Analog Thumbstick (Horizontal axis monitoring)
		if (axe == 0) {
			if (value < -JOY_DEAD_ZONE)      pressKey(Joystick::AXE1_LEFT);
			else                            releaseKey(Joystick::AXE1_LEFT);
			if (value > JOY_DEAD_ZONE)       pressKey(Joystick::AXE1_RIGHT);
			else                            releaseKey(Joystick::AXE1_RIGHT);
		}
		// Axis 1: Left Analog Thumbstick (Vertical axis monitoring)
		else if (axe == 1) {
			if (value < -JOY_DEAD_ZONE)      pressKey(Joystick::AXE1_UP);
			else                            releaseKey(Joystick::AXE1_UP);
			if (value > JOY_DEAD_ZONE)       pressKey(Joystick::AXE1_DOWN);
			else                            releaseKey(Joystick::AXE1_DOWN);
		}
		// Axis 5: Gamepad Right Trigger
		else if (axe == 5) {
			if (value > JOY_DEAD_ZONE)       pressKey(Joystick::RT);
			else                            releaseKey(Joystick::RT);
		}
		// 🚀 Axis 2 & 4: MODERN 3D PERSPECTIVE VIEW CONTROLS
		// Captures the Right Analog Stick inputs to enable continuous 3D camera look operations.
		else if (axe == 2 || axe == 4) {
			if (axe == 2) {
				m_joyAxeValue.x = static_cast<float>(value);
			}
			else if (axe == 4) {
				m_joyAxeValue.y = static_cast<float>(value);
			}

			// Perform safe bounds check validations to simulate relative camera viewing rotations
			if ((m_joyAxeValue.x != 0 || m_joyAxeValue.y != 0) &&
				(m_mouseCoords.x >= 0 && m_mouseCoords.x <= m_screenWidth) &&
				(m_mouseCoords.y >= 0 && m_mouseCoords.y <= m_screenHeight))
			{
				if (m_axisTimeLapse > AXIS_TIMELAPSE)
				{
					m_axisTimeLapse = 0.0f;

					// Apply standard normalized joystick calculation parameters smoothly
					m_mouseCoords.x += JOY_AXE_DEFAULT_SPEED * (m_joyAxeValue.x / JOY_AXE_MAX);
					m_mouseCoords.y += JOY_AXE_DEFAULT_SPEED * (m_joyAxeValue.y / JOY_AXE_MAX);

					// Warp the hardware system cursor pointer safely to maintain seamless rotation sequences
					SDL_WarpMouseInWindow(window, m_mouseCoords.x, m_mouseCoords.y);

					m_mouseCoords.x = std::clamp(m_mouseCoords.x, 0.0f, static_cast<float>(m_screenWidth));
					m_mouseCoords.y = std::clamp(m_mouseCoords.y, 0.0f, static_cast<float>(m_screenHeight));
				}
			}
		}
	}

	void EventHandler::updateJoystickHats(int value)
	{
		// Clean and flush previous D-Pad settings to guarantee clean directional states
		releaseKey(Joystick::DPAD_LEFT);
		releaseKey(Joystick::DPAD_RIGHT);
		releaseKey(Joystick::DPAD_UP);
		releaseKey(Joystick::DPAD_DOWN);

		// Flag the new directional layout position using the formatted identity value
		pressKey(value);
	}

	void EventHandler::updateMapping(uint32_t eventID, PlayWith util)
	{
		for (auto& [key, isDown] : m_keyMap) {
			if (isKeyPressed(key) && key != SDL_BUTTON_LEFT)
			{
				if (util == PlayWith::KEYBOARD)
					m_eventConfigTemp[eventID].keyID = key;
				else
					m_eventConfigTemp[eventID].joyID = key;
			}
		}
	}

	uint32_t EventHandler::getKeyID(uint32_t eventID)
	{
		auto itEvent = m_eventConfig.find(eventID);
		if (itEvent != m_eventConfig.end()) {
			eventID = (m_playWith == PlayWith::KEYBOARD) ? itEvent->second.keyID : itEvent->second.joyID;
		}
		return eventID;
	}

	void EventHandler::updateConfig()
	{
		for (auto& [id, config] : m_eventConfig) {
			m_eventConfigTemp[id] = config;
		}
	}

	void EventHandler::saveConfig()
	{
		for (auto& [id, config] : m_eventConfigTemp) {
			m_eventConfig[id] = config;
		}
		saveConfigFile();
	}

	bool EventHandler::loadConfigFile()
	{
		std::ifstream file(KEYS_CONF);
		if (file.fail()) {
			return false;
		}

		size_t size = 0;
		int playwithMode = 0;

		file >> playwithMode;
		file >> size;

		for (size_t i = 0; i < size; i++) {
			uint32_t id = 0, keyID = 0, joyID = 0;
			file >> id >> keyID >> joyID;
			m_eventConfig[id] = EventConfig(keyID, joyID);
		}
		file.close();

		if (static_cast<PlayWith>(playwithMode) == PlayWith::GAMEPAD && m_gamepad != nullptr)
			m_playWith = PlayWith::GAMEPAD;
		else
			m_playWith = PlayWith::KEYBOARD;

		updateConfig();
		return true;
	}

	bool EventHandler::saveConfigFile()
	{
		std::ofstream file(KEYS_CONF);
		if (file.fail()) {
			return false;
		}

		file << static_cast<int>(m_playWith) << '\n';
		file << m_eventConfig.size() << '\n';

		for (auto& [id, config] : m_eventConfig) {
			file << id << ' ' << config.keyID << ' ' << config.joyID << '\n';
		}
		file.close();
		return true;
	}

	void EventHandler::clearConfig()
	{
		m_eventConfigTemp.clear();
		m_eventConfig.clear();
	}

	std::string EventHandler::getMapping(uint32_t eventID)
	{
		return getMapping(eventID, m_playWith, m_eventConfig);
	}

	std::string EventHandler::getMapping(uint32_t eventID, PlayWith util)
	{
		return getMapping(eventID, util, m_eventConfigTemp);
	}

	std::string EventHandler::getMapping(uint32_t eventID, PlayWith util, std::unordered_map<uint32_t, EventConfig>& map)
	{
		uint32_t keyID;

		if (util == PlayWith::KEYBOARD)
		{
			keyID = map[eventID].keyID;
			switch (keyID)
			{
			case SDL_BUTTON_LEFT: return "Mouse Left button";
			case SDL_BUTTON_RIGHT: return "Mouse Right button";
			default:
				return SDL_GetKeyName(keyID);
			}
		}
		else
		{
			keyID = map[eventID].joyID;
			switch (keyID)
			{
			case Joystick::A: return "A button";
			case Joystick::B: return "B button";
			case Joystick::X: return "X button";
			case Joystick::Y: return "Y button";
			case Joystick::HOME: return "HOME button";
			case Joystick::LB: return "Left bumper";
			case Joystick::RB: return "Right bumper";
			case Joystick::L3: return "Stick Left button";
			case Joystick::R3: return "Stick Right button";
			case Joystick::SELECT: return "Select button";
			case Joystick::START: return "Start button";
			case Joystick::DPAD_UP: return "Directional pad UP";
			case Joystick::DPAD_DOWN: return "Directional pad DOWN";
			case Joystick::DPAD_LEFT: return "Directional pad LEFT";
			case Joystick::DPAD_RIGHT: return "Directional pad RIGHT";
			case Joystick::LT: return "Left trigger";
			case Joystick::RT: return "Right trigger";
			case Joystick::AXE1_UP: return "Left stick UP";
			case Joystick::AXE1_DOWN: return "Left stick DOWN";
			case Joystick::AXE1_LEFT: return "Left stick LEFT";
			case Joystick::AXE1_RIGHT: return "Left stick RIGHT";
			case Joystick::AXE2_UP: return "Right stick UP";
			case Joystick::AXE2_DOWN: return "Right stick DOWN";
			case Joystick::AXE2_LEFT: return "Right stick LEFT";
			case Joystick::AXE2_RIGHT: return "Right stick RIGHT";
			default:
				return "";
			}
		}
	}

}
