#include "Audio.h"

#include <SDL3_mixer/SDL_mixer.h>

namespace NS2 {
	void Audio::play(const glm::vec3& position, int loops) {
		// 1. Allocate a fresh tracking track node from the parent device lines
		m_track = MIX_CreateTrack(m_mixer);
		if (!m_track) return;

		// 2. Build property descriptor sheets specifying loop instructions
		SDL_PropertiesID props = SDL_CreateProperties();
		if (loops != 0) SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, loops);

		// 3. Process 3D attenuation offsets if spatial vector lengths exceed baseline markers
		if (glm::length(position) > 0.0f) {
			MIX_Point3D pos = { position.x, position.y, position.z };
			MIX_SetTrack3DPosition(m_track, &pos);
		}

		// 4. Map the asset waveforms and pass command sheets directly to audio pipelines
		MIX_SetTrackAudio(m_track, m_audio);
		MIX_PlayTrack(m_track, props);
		SDL_DestroyProperties(props);
	}

	void Audio::setVolume(float vol) { 
		if (m_track) MIX_SetTrackGain(m_track, vol); 
	}
}
