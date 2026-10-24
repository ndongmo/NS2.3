#include "AudioEngine.h"
#include "Audio.h"

#include <SDL3_mixer/SDL_mixer.h>

namespace NS2 {
	bool AudioEngine::init() {
		if (!MIX_Init()) return false;

		// Setup separate sound and music lines to let us set distinct global volume controls
		m_musicMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
		m_soundMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
		return m_initialized = (m_musicMixer && m_soundMixer);
	}

	void AudioEngine::shutdown() {
		if (m_initialized) {
			MIX_DestroyMixer(m_musicMixer);
			MIX_DestroyMixer(m_soundMixer);
			MIX_Quit();
			m_initialized = false;
		}
	}

	void AudioEngine::setVolumeMusic(float vol) { 
		MIX_SetMixerGain(m_musicMixer, vol); 
	}

	void AudioEngine::setVolumeSound(float vol) { 
		MIX_SetMixerGain(m_soundMixer, vol); 
	}

	Audio* AudioEngine::allocateSound(const std::string& path) {
		MIX_Audio* audio = MIX_LoadAudio(m_soundMixer, path.c_str(), false);
		return audio ? new Audio(m_soundMixer, audio, path) : nullptr;
	}

	Audio* AudioEngine::allocateMusic(const std::string& path) {
		MIX_Audio* audio = MIX_LoadAudio(m_musicMixer, path.c_str(), false);
		return audio ? new Audio(m_musicMixer, audio, path) : nullptr;
	}

	void AudioEngine::destroy(Audio* audio) {
		MIX_DestroyAudio(audio->getRawAudio());
	}
}
