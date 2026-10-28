/*!
* \file PathHelper.h
* \brief Keep track on different file and folder path.
* \author Ndongmo Silatsa Fabrice
* \date 07-06-2018
* \version 2.0
*/

#pragma once

#include "Types.h"

#include <string>
#include <IOManager.h>
#include <ResourceManager.h>

#define SPLITTER_STR "_"
#define FILE_EXT ".json"

#define SAVEPATH "../Saves/"
#define UTILSPATH "../Utils/"
#define LEVELPATH "../Levels/"
#define FONTPATH "../Gui/fonts/"
#define MUSICPATH "../Musics/"
#define SOUNDPATH "../Sounds/"
#define OBJECTPATH "../Objects/"
#define TEXTUREPATH "../Assets/"

#define LEVEL_ROOT_NAME "Level"

#define PARTICLES_ASSET "../Assets/particles/particle.png"
#define PARTICLES_SLASH_ASSET "../Assets/particles/pa_sword1.png"
#define PARTICLES_GUNFIRE_ASSET "../Assets/particles/pa_gunfire1.png"
#define TILE_ASSET "../Assets/tiles/ti_floor1.png"
#define LIGHT_ASSET "../Assets/lights/li_point1.png"

#define SAVES_FILE "../Saves/saves.xml"

class PathHelper
{
public:
	static void createDir(const std::string& dirpath) { 
		NS2::IOManager::makeDirectory(dirpath.c_str());
	}
	static bool isFileExist(const std::string& path) {
		return NS2::IOManager::exists(path.c_str());
	}
	static bool isObjectExist(const std::string& name, GameType type) {
		std::string path = PathHelper::getObjectTypePath(TypeHelper::getGameType(type)) + name + FILE_EXT;
		return NS2::IOManager::exists(path.c_str());
	}
	/*!
	* \brief return the absolute path of the given file for the given type.
	*/
	static std::string getObjectPath(const std::string& name, GameType type) { 
		return PathHelper::getObjectTypePath(TypeHelper::getGameType(type)) + name + FILE_EXT;
	}
	static std::string getObjectPath(const std::string& name, const std::string& gameType) {
		return PathHelper::getObjectTypePath(gameType) + name + FILE_EXT;
	}
	static std::string getFileName(const std::string& path)
	{
		std::string file = path;
		if (file.length() > 3) {
			file.erase(0, file.find_last_of('/') + 1);
			file.erase(file.find('.'));
		}
		return file;
	}
	static std::string newObjectName(GameType type) {
		std::string typeName = TypeHelper::getGameType(type);
		std::string typePath = PathHelper::getObjectTypePath(typeName);
		int fileCount = NS2::IOManager::countFiles(typePath, FILE_EXT) + 1;
		return typeName + SPLITTER_STR + std::to_string(fileCount);
	}
	static std::string getAssetFilePath(const std::string& guiAssetName) {
		size_t pos = guiAssetName.find_last_of("DLS/");
		std::string result = "";

		if (pos != std::string::npos) {
			result = TEXTUREPATH + guiAssetName.substr(pos, guiAssetName.size()) + ".png";
		}
		if (NS2::IOManager::exists(result.c_str())) return result;
		return "";
	}
	static std::string getUtilsPath(const std::string& name) { return UTILSPATH + name + FILE_EXT;}
	static std::string getLevelPath(const std::string& name) { return LEVELPATH + name + FILE_EXT; }
	static std::string getLevelName(int index) { return LEVEL_ROOT_NAME + std::string(SPLITTER_STR) + std::to_string(index); }
	static std::string getMusicPath(const std::string& name) { return MUSICPATH + name + ".mp3"; }
	static std::string getSoundMP3Path(const std::string& name) { return SOUNDPATH + name + ".mp3"; }
	static std::string getSoundWAVPath(const std::string& name) { return SOUNDPATH + name + ".wav"; }
	static std::string getSoundOGGPath(const std::string& name) { return SOUNDPATH + name + ".ogg"; }
	static std::string getObjectTypePath(const std::string& name) { return OBJECTPATH + name + "/"; }
	static std::string getTexturePath(const std::string& name) { return TEXTUREPATH + name + ".png"; }
	static std::string getFontPath(const std::string& name) { return FONTPATH + name + ".ttf"; }
};
