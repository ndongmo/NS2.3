/*!
* \file Audio.h
* \brief Simple Sound and Music wrapper.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include <string>
#include <glm/glm.hpp>

struct MIX_Mixer;
struct MIX_Audio;
struct MIX_Track;

namespace NS2
{
	/**
	 * @class Audio
	 * @brief Wrapper tracking an active, cached sound element loaded inside RAM structures.
	 */
	class Audio {
	public:
		Audio(MIX_Mixer* mixer, MIX_Audio* audio, const std::string& path) : m_mixer(mixer), m_audio(audio), m_filePath(path) {}
		~Audio() = default; // Destruction lifecycle is safely isolated within the ResourceManager.

		/**
		 * @brief Invokes spatial placement attributes to trigger playback commands via SDL3_mixer.
		 * @param position Global world 3D position vector context (X, Y, Z).
		 * @param loops Repeat instruction loops (0 plays once, -1 triggers infinite loops).
		 */
		void play(const glm::vec3& position = glm::vec3(0.0f), int loops = 0);

		/** @brief Controls track attenuation adjustments. */
		void setVolume(float vol);

		/** @brief Unwraps the internal hardware sound chunk handle pointer. */
		MIX_Audio* getRawAudio() const { return m_audio; }

	private:
		MIX_Mixer* m_mixer = nullptr;  /**< Linked mixer device controller pipeline */
		MIX_Audio* m_audio = nullptr;  /**< Native audio waveform buffer chunk pointer */
		MIX_Track* m_track = nullptr;  /**< Unique active track stream tracker slot */
		std::string m_filePath;       /**< Reference validation file path string */
	};
}
