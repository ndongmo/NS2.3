#include "IOManager.h"
#include "Logger.h"

#include <fstream>
#include <filesystem>

// Namespace alias
namespace fs = std::filesystem;

namespace NS2 
{
	bool IOManager::readFileToBuffer(const std::string filePath, std::vector<unsigned char>& buffer) {
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail()) {
			logE << "Can load file : " << filePath.c_str() << std::endl;
			Logger::exitGame();
		}

		//seek to the end
		file.seekg(0, std::ios::end);

		//get the file size
		int fileSize = (int)file.tellg();

		//seek to the beginning
		file.seekg(0, std::ios::beg);

		//reduce the file size by any header bytes that might be present
		fileSize -= (int)file.tellg();

		buffer.resize(fileSize);
		file.read((char*)&(buffer[0]), fileSize);
		file.close();

		return true;
	}

	bool IOManager::readFileToBuffer(const std::string filePath, std::string& buffer)
	{
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail()) {
			perror(filePath.c_str());
			return false;
		}

		//seek to the end
		file.seekg(0, std::ios::end);

		//get the file size
		int fileSize = (int)file.tellg();

		//seek to the beginning
		file.seekg(0, std::ios::beg);

		//reduce the file size by any header bytes that might be present
		fileSize -= (int)file.tellg();

		buffer.resize(fileSize);
		file.read((char*)&(buffer[0]), fileSize);
		file.close();

		return true;
	}

	bool IOManager::getDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries, bool addDir) {
		auto dpath = fs::path(path);
		// Must be directory
		if (!fs::is_directory(dpath)) return false;

		for (auto it = fs::directory_iterator(dpath); it != fs::directory_iterator(); ++it) {
			bool isDir = is_directory(it->path());
			if (!isDir || addDir) {
				rvEntries.emplace_back();
				rvEntries.back().path = it->path().string();
				std::replace(rvEntries.back().path.begin(), rvEntries.back().path.end(), '\\', '/');
				if (is_directory(it->path())) {
					rvEntries.back().isDirectory = true;
				}
				else {
					rvEntries.back().isDirectory = false;
				}
			}
		}
		return true;
	}

	bool IOManager::getRecDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries, bool addDir)
	{
		auto dpath = fs::path(path);
		// Must be directory
		if (!fs::is_directory(dpath)) return false;

		for (auto it = fs::directory_iterator(dpath); it != fs::directory_iterator(); ++it) {
			DirEntry entry;
			entry.isDirectory = is_directory(it->path());
			entry.path = it->path().string();
			std::replace(entry.path.begin(), entry.path.end(), '\\', '/');
			if (!entry.isDirectory || addDir) {
				rvEntries.push_back(entry);
			}
			if (entry.isDirectory) {
				getDirectoryEntries(entry.path.c_str(), rvEntries, addDir);
			}
		}
		return true;
	}

	bool IOManager::copyFile(const char* from, const char* to)
	{
		fs::copy_file(fs::path(from), fs::path(to));
		return exists(to);
	}

	bool IOManager::makeDirectory(const char* path) {
		if (!fs::exists(fs::path(path))) {
			return fs::create_directory(fs::path(path));
		}
		return false;
	}

	bool IOManager::copyDir(const char* from, const char* to)
	{
		bool result = false;
		std::vector<DirEntry> entries;
		getDirectoryEntries(from, entries);

		if (!entries.empty() && strlen(to) > 0) {
			makeDirectory(to);

			for (auto& e : entries) {
				if (!e.isDirectory) {
					std::string dst(to);
					dst += dst.at(dst.length() - 1) == '/' ? fs::path(e.path).filename().string() : "/" + fs::path(e.path).filename().string();
					fs::copy_file(fs::path(e.path), fs::path(dst));
				}
			}
			result = true;
		}
		return result;
	}

	bool IOManager::exists(const char* path) 
	{
		return fs::exists(fs::path(path));
	}

	bool IOManager::deleteDir(const char* path)
	{
		return fs::remove_all(fs::path(path)) > 0;
	}

	int IOManager::countFiles(const std::string& pathStr, const std::string& extension) {
		fs::path targetPath(pathStr);
		if (!fs::exists(targetPath) || !fs::is_directory(targetPath)) return 0;

		int fileCount = 0;
		for (const auto& entry : fs::directory_iterator(targetPath)) {
			if (entry.is_regular_file() && entry.path().extension() == extension) {
				fileCount++;
			}
		}
		return fileCount;
	}
}