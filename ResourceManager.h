/**
 * @file ResourceManager.h
 * @brief Memory owner managing lifetimes of textures, multi-material models, fonts, and audio tracks.
 * @author Ndongmo Silatsa Fabrice
 * @date Updated for NS2 Multi-Mesh Architecture
 */

#pragma once

#include "SpriteFont.h"
#include "AudioEngine.h"
#include "Audio.h"

#include <map>
#include <memory>
#include <string>

namespace NS2
{
	class SpriteFont;
	class StaticMesh;
	struct GLTexture;

	/**
	 * @class ResourceManager
	 * @brief The absolute memory owner of all graphics, text, and sound resources inside the engine.
	 *
	 * It controls memory lifetimes using std::unique_ptr storage maps. Subsystems and scene
	 * nodes request resources from here and receive non-owning observer pointers (T*),
	 * completely decoupling lifetime dependencies.
	 */
	class ResourceManager {
	public:
		ResourceManager() = default;

		/** @brief Cleans out tracking maps, triggering clean deletion cycles across all assets. */
		~ResourceManager() { purge(); }

		// Enforce unique system resource handling rules by banning structural copy tracking operations
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;

		/** @brief Explicitly flushes maps and ensures SDL and OpenGL assets release correctly. */
		void purge();

		/** @brief Fetches a pre-cached texture asset reference. */
		GLTexture* getTexture(const std::string& path);

		/** @brief Fetches a pre-cached text layout font asset reference. */
		SpriteFont* getFont(const std::string& path, float size);

		/** @brief Fetches an un-looped sound sample. Initializes it via AudioEngine if missing. */
		Audio* getSound(const std::string& path);

		/** @brief Fetches a streaming background track asset reference. */
		Audio* getMusic(const std::string& path);

		AudioEngine& getAudioEngine() { return m_audioEngine; }

	private:
		AudioEngine m_audioEngine;											/**< Internal Audio engine instance */

		std::map<std::string, std::unique_ptr<GLTexture>> m_textures;		/**< Cached texture data map storage */
		std::map<std::string, std::unique_ptr<SpriteFont>> m_fonts;			/**< Cached text font map storage */
		std::map<std::string, std::unique_ptr<Audio>> m_sounds;				/**< Cached short sound fx map storage */
		std::map<std::string, std::unique_ptr<Audio>> m_music;				/**< Cached music streaming track storage */
	};
}
