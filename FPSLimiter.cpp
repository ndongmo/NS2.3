#include "FPSLimiter.h"

#include <algorithm>

namespace NS2 {

	void FPSLimiter::init(float maxFPS) {
		setMaxFPS(maxFPS);
		m_fps = 0.0f;
		m_frameTime = 0.0f;
		m_currentSample = 0;
		std::fill(std::begin(m_frameTimes), std::end(m_frameTimes), 0.0f);
	}

	void FPSLimiter::setMaxFPS(float maxFPS) {
		m_maxFPS = maxFPS;
	}

	void FPSLimiter::begin() {
		m_startTicks = SDL_GetTicks();
	}

	void FPSLimiter::end() {
		calculateFPS();

		float targetFrameTime = 1000.0f / m_maxFPS;
		if (m_frameTime < targetFrameTime) {
			float delayTime = targetFrameTime - m_frameTime;
			SDL_Delay(static_cast<Uint32>(delayTime));
		}
		// update the frame time so that it reflects the real elapsed time
		m_frameTime = static_cast<float>(SDL_GetTicks() - m_startTicks);
	}

	void FPSLimiter::calculateFPS() {
		uint64_t currentTicks = SDL_GetTicks();
		m_frameTime = static_cast<float>(currentTicks - m_startTicks);

		m_frameTimes[m_currentSample % NUM_SAMPLES] = m_frameTime;
		m_currentSample++;

		int count = std::min(m_currentSample, NUM_SAMPLES);
		float frameTimeAverage = 0.0f;

		for (int i = 0; i < count; i++) {
			frameTimeAverage += m_frameTimes[i];
		}
		frameTimeAverage /= count;

		if (frameTimeAverage > 0.0f) {
			m_fps = 1000.0f / frameTimeAverage;
		}
		else {
			m_fps = m_maxFPS;
		}
	}
}