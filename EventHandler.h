#pragma once
/*!
* \file EventHandler.h
* \brief Event handler class, it checks and stores events using SDL3.
* \author Ndongmo Silatsa Fabrice
* \date 24-10-2016 (Updated to SDL3/C++20)
* \version 2.0
*/

#pragma once

#include <string>
#include <unordered_map>
#include <cstdint>
#include <glm/glm.hpp>
#include <SDL3/SDL.h> // Retained based on your design preference

namespace NS2
{

#define JOY_DEAD_ZONE 2000
#define JOY_AXE_MAX  32767
#define MARGIN 500

	struct EventConfig {
		EventConfig() : keyID(0), joyID(0) {}
		EventConfig(uint32_t key, uint32_t joy) : keyID(key), joyID(joy) {}
		uint32_t keyID;
		uint32_t joyID;
	};

	/*!
	* \brief Allow to pick what user wants to play with.
	*/
	enum class PlayWith { KEYBOARD, GAMEPAD };

	/*!
	* \brief Standarized Gamepad Axis & Buttons (Aligned with modern SDL3 Gamepad architecture).
	*/
	enum Joystick : uint32_t
	{
		A = 10 + MARGIN,
		B = 11 + MARGIN,
		X = 12 + MARGIN,
		Y = 13 + MARGIN,
		HOME = 14 + MARGIN,
		LB = 8 + MARGIN,
		RB = 9 + MARGIN,
		L3 = 6 + MARGIN,
		R3 = 7 + MARGIN,
		SELECT = 5 + MARGIN, 
		START = 4 + MARGIN,
		DPAD_UP = 0 + MARGIN,
		DPAD_DOWN = 1 + MARGIN,
		DPAD_LEFT = 2 + MARGIN,
		DPAD_RIGHT = 3 + MARGIN,
		LT = 15 + MARGIN,
		RT = 16 + MARGIN,
		AXE1_UP = 17 + MARGIN,
		AXE1_DOWN = 18 + MARGIN,
		AXE1_LEFT = 19 + MARGIN,
		AXE1_RIGHT = 20 + MARGIN,
		AXE2_UP = 21 + MARGIN,
		AXE2_DOWN = 22 + MARGIN,
		AXE2_LEFT = 23 + MARGIN,
		AXE2_RIGHT = 24 + MARGIN,
		NOTHING = 25 + MARGIN
	};

	class EventHandler
	{
	public:
		void init(int screenWidth, int screenHeight);
		void destroy();
		void resetPressedTimer();
		void update(float deltaTime);
		void updateJoystickAxis(int axe, int value, SDL_Window* window);
		void updateJoystickHats(int value);

		void pressKey(uint32_t keyID);
		void releaseKey(uint32_t keyID);
		bool isKeyDown(uint32_t keyID);
		bool isKeyPressed(uint32_t keyID);
		bool isKeyHold(uint32_t keyID);

		bool isEventDown(uint32_t eventID);
		bool isEventPressed(uint32_t eventID);
		bool isEventPressedRel(uint32_t eventID);
		bool isEventHold(uint32_t eventID);
		void releaseEvent(uint32_t eventID);

		bool isJoyConnect() { return m_gamepad != nullptr; }

		void addEvent(uint32_t eventID, uint32_t keyID, uint32_t joyID)
		{
			m_eventConfig[eventID] = EventConfig(keyID, joyID);
		}

		void updateMapping(uint32_t eventID, PlayWith util);
		std::string getMapping(uint32_t eventID, PlayWith util);
		std::string getMapping(uint32_t eventID);
		uint32_t getKeyID(uint32_t eventID);

		void updateConfig();
		void saveConfig();
		void clearConfig();
		bool loadConfigFile();
		bool saveConfigFile();

		/* SETTERS */
		void setMouseCoords(float x, float y);
		void setPlayWith(PlayWith util) { m_playWith = util; }

		/* GETTERS */
		const glm::vec2& getMouseCoords() const { return m_mouseCoords; }
		PlayWith getPlayWith() { return m_playWith; }
		EventConfig getEventKey(uint32_t eventID)
		{
			return m_eventConfig[eventID];
		}

	private:
		std::string getMapping(uint32_t eventID, PlayWith util, std::unordered_map<uint32_t, EventConfig>& map);

		std::unordered_map<uint32_t, EventConfig> m_eventConfig;
		std::unordered_map<uint32_t, EventConfig> m_eventConfigTemp;
		std::unordered_map<uint32_t, bool> m_keyMap;
		std::unordered_map<uint32_t, float> m_keyTimer;
		std::unordered_map<uint32_t, bool> m_keyPressed;
		glm::vec2 m_mouseCoords = glm::vec2(0);
		glm::vec2 m_joyAxeValue = glm::vec2(0);
		PlayWith m_playWith = PlayWith::KEYBOARD;

		SDL_Gamepad* m_gamepad = nullptr;
		int m_screenWidth, m_screenHeight;
		float m_axisTimeLapse = 0.0f;
	};
}