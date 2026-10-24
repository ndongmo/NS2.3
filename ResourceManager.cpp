/**
 * @file ResourceManager.cpp
 * @brief Memory owner managing lifetimes of textures, multi-material models, fonts, and audio tracks.
 * @author Ndongmo Silatsa Fabrice
 */

#include "ResourceManager.h"
#include "ImageLoader.h"

#include <box3d/box3d.h>

namespace NS2 {
	void ResourceManager::purge() {
		for (auto& f : m_fonts) f.second->dispose();
		m_fonts.clear();

		for (auto& s : m_sounds) m_audioEngine.destroy(s.second.get());
		m_sounds.clear();

		for (auto& m : m_music)  m_audioEngine.destroy(m.second.get());
		m_music.clear();

		for (auto& pair : m_textures) {
			GLTexture* tex = pair.second.get();
			if (tex && tex->id != 0) {
				glDeleteTextures(1, &tex->id);
				tex->id = 0;
			}
		}
		m_textures.clear();
	}

	GLTexture* ResourceManager::getTexture(const std::string& path) {
		auto it = m_textures.find(path);
		if (it != m_textures.end()) return it->second.get();
		auto texture = std::make_unique<GLTexture>();
		ImageLoader::load(path, *texture);

		GLTexture* rawPtr = texture.get();
		m_textures[path] = std::move(texture);
		return rawPtr;
	}

	SpriteFont* ResourceManager::getFont(const std::string& path, float size) {
		std::string key = path + "_" + std::to_string(static_cast<int>(size));
		auto it = m_fonts.find(key);
		if (it != m_fonts.end()) return it->second.get();

		auto font = std::make_unique<SpriteFont>();
		font->init(path.c_str(), size);
		return (m_fonts[key] = std::move(font)).get();
	}

	Audio* ResourceManager::getSound(const std::string& path) {
		auto it = m_sounds.find(path);
		if (it != m_sounds.end()) return it->second.get();

		// Request the device subsystem decode our sound data into a raw heap pointer allocation
		Audio* sound = m_audioEngine.allocateSound(path);
		if (!sound) return nullptr;

		// Retain resource ownership inside our safe unique map layers
		return (m_sounds[path] = std::unique_ptr<Audio>(sound)).get();
	}

	Audio* ResourceManager::getMusic(const std::string& path) {
		auto it = m_music.find(path);
		if (it != m_music.end()) return it->second.get();

		Audio* music = m_audioEngine.allocateMusic(path);
		if (!music) return nullptr;
		return (m_music[path] = std::unique_ptr<Audio>(music)).get();
	}
}
