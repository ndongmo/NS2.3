#pragma once
/*!
* \file FPSLimiter.h
* \brief Calculate and limite game FPS.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include <SDL3/SDL.h>

namespace NS2 {

#define NUM_SAMPLES 10

	class FPSLimiter {
	public:
		/*!
		* \brief Initialize with max FPS.
		* \param maxFPS: maximum FPS game can reach
		*/
		void init(float maxFPS);
		/*!
		* \brief Retrieve SDL ticks.
		*/
		void begin();
		/*!
		* \brief Calculate and return FPS value.
		*/
		void end();

		/* SETTERS */
		void setMaxFPS(float maxFPS);

		/* GETTERS */
		float getFPS() const { return m_fps; }
		float getFrameTime() const { return m_frameTime; }

	private:
		/*!
		* \brief Calculate FPS value.
		*/
		void calculateFPS();

		float m_fps;						/* FPS value */
		float m_maxFPS;						/* Maximum FPS value */
		float m_frameTime;					/* Frame time */
		Uint64 m_startTicks;				/* Keep ticks last value */
		float m_frameTimes[NUM_SAMPLES];	/* History array for historical moving average */
		int m_currentSample;				/* Ring buffer indexing locator marker */
	};
}