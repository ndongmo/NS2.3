/*!
* \file AudioEngine.h
* \brief Simple Sound and Music manager.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include <string>

struct MIX_Mixer;

namespace NS2
{
	class Audio;

	/**
	 * @class AudioEngine
	 * @brief A stateless hardware management interface manipulating sound device contexts.
	 *
	 * Storage maps are removed from this object entirely. It acts purely as a generator
	 * tool creating sound assets and passing absolute memory ownership up to the resource cache.
	 */
	class AudioEngine {
	public:
		/** @brief Sets up hardware mixer lines and opens default audio endpoints. */
		bool init();

		/** @brief Securely closes active mixer outputs and shuts down system frameworks. */
		void shutdown();

		/** @brief Mutates global music bus output gains. */
		void setVolumeMusic(float vol);

		/** @brief Mutates global short sound effect bus output gains. */
		void setVolumeSound(float vol);

		/** @brief Allocation tool: Decodes a audio file into RAM as an un-looped sound sample. */
		Audio* allocateSound(const std::string& path);

		/** @brief Allocation tool: Decodes a file onto music mixer channels as a long streaming track. */
		Audio* allocateMusic(const std::string& path);

		/**  @brief Destroy the given audio resource */
		void destroy(Audio* audio);

	private:
		MIX_Mixer* m_soundMixer = nullptr; /**< Independent sound effects bus routing controller */
		MIX_Mixer* m_musicMixer = nullptr; /**< Independent stream music track routing controller */
		bool m_initialized = false;        /**< System state validation protection check tracker */
	};
}
