#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <any>
#include <box3d/box3d.h>
#include <nlohmann/json.hpp>
#include <Logger.h>
#include <ResourceManager.h>

#include "ObjectLoader.h"
#include "ObjectSaver.h"

namespace NS2 {
    class ResourceManager;
}

using json = nlohmann::json;

/**
 * @brief A generic, template-driven ObjectManager (Asset Cache).
 *
 * This class implements a high-performance blueprint manager.
 * It prevents duplicate disk IO operations by caching parsed JSON blueprints
 * and provides options for shared references, unique pointers, or raw values.
 *
 */
class ObjectManager {
public:
    ObjectManager(NS2::ResourceManager& rsrcManager);
    ~ObjectManager() = default;

    // Delete copy mechanics to prevent cache duplication
    ObjectManager(const ObjectManager&) = delete;
    ObjectManager& operator=(const ObjectManager&) = delete;
    ObjectManager(ObjectManager&&) noexcept = default;
    ObjectManager& operator=(ObjectManager&&) noexcept = default;

	/**
	 * @brief Renames a cached object and updates the internal cache mapping.
	 *
	 * @param oldPath The current path of the cached object.
	 * @param newPath The new path to assign to the cached object.
	 * @return true if rename succeeded, false otherwise.
	 */
    bool rename(const std::string& oldPath, const std::string& newPath)
    {
        std::error_code ec;
        std::filesystem::rename(oldPath, newPath, ec);

        if (ec) {
            logE << "[ObjectManager] Error renaming file: " << ec.message() << std::endl;
			return false;
        }
        else if (m_cache.find(oldPath) != m_cache.end()) {
			m_cache[newPath] = m_cache[oldPath];
			m_cache.erase(oldPath);
        }
        return true;
    }

	/**
	 * @brief Removes a cached object and deletes the corresponding file from disk.
	 *
	 * @param filePath The path of the file to remove.
	 * @return true if removal succeeded, false otherwise.
	 */
	bool remove(const std::string& filePath)
	{
		std::error_code ec;
		std::filesystem::remove(filePath, ec);
		if (ec) {
			logE << "[ObjectManager] Error removing file: " << ec.message() << std::endl;
			return false;
		}
		else if (m_cache.find(filePath) != m_cache.end()) {
			m_cache.erase(filePath);
		}
		return true;
	}

    /**
     * @brief Serializes a game object or level configuration directly back to disk.
     *
     * @param filePath The destination target path on the drive.
     * @param data The reference data class to serialize.
     * @return true if write pipeline succeeded, false otherwise.
     */
    template <typename T>
    bool save(const std::string& filePath, const T& data) {
        try {
            nlohmann::json j = ObjectSaver::save(data);
            std::ofstream file(filePath);
            if (!file.is_open()) return false;

            file << j.dump(4); // Indents nicely for human-readable save files
            return true;
        }
        catch (const std::exception& e) {
            logE << "[ObjectManager] Serialization exception: " << e.what() << std::endl;
            return false;
        }
    }

    /**
     * @brief Loads a configuration file directly from disk into an object reference bypassing the cache.
     * @note Ideal for volatile or single-use structural assets like Levels, Maps, or match configurations.
     *
     * @param filePath Path to the JSON configuration file on disk.
     * @param outData The active container reference target where parsed states will be injected.
     * @return true if the file stream was opened and parsed successfully, false otherwise.
     */
    template <typename T>
    bool loadDirect(const std::string& filePath, T& outData) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            logE << "[ObjectManager] Direct Load Failure: Unable to open file at -> " << filePath << std::endl;
            return false;
        }

        try {
            json j;
            file >> j;

            return m_loader.load(j, outData);
        }
        catch (const json::parse_error& e) {
            logE << "[ObjectManager] Direct Load JSON Syntax Error [" << filePath << "]: " << e.what() << std::endl;
        }
        catch (const json::exception& e) {
            logE << "[ObjectManager] Direct Load Structural Type Mismatch [" << filePath << "]: " << e.what() << std::endl;
        }
        catch (const std::exception& e) {
            logE << "[ObjectManager] Direct Load Generic System Exception [" << filePath << "]: " << e.what() << std::endl;
        }

        return false;
    }

    /**
     * @brief Loads a shared reference to a read-only configuration blueprint.
     * @note Ideal for immutable configuration properties shared across items (Flyweight Pattern).
     *
     * @param filePath Path to the JSON data configuration file.
     * @return std::shared_ptr<const T> A read-only pointer to the cached object.
     */
    template <typename T>
    std::shared_ptr<const T> loadBlueprint(const std::string& filePath) {
        return getOrLoad<T>(filePath);
    }

    /**
     * @brief Loads a unique, isolated instance cloned from the cached JSON blueprint.
     * @note Ideal for polymorphic entity layouts like std::vector<std::unique_ptr<BaseClass>>.
     *
     * @param filePath Path to the JSON data configuration file.
     * @return std::unique_ptr<T> A unique pointer containing a distinct copy of the object.
     */
    template <typename T>
    std::unique_ptr<T> loadUnique(const std::string& filePath) {
        auto blueprint = getOrLoad<T>(filePath);
        if (!blueprint) return nullptr;

        // Dereferencing the shared blueprint invokes the object's copy constructor
        return std::make_unique<T>(*blueprint);
    }

    /**
     * @brief Loads a unique, isolated instance cloned from the cached JSON blueprint.
     * @note Ideal for polymorphic entity layouts like std::vector<std::unique_ptr<BaseClass>>.
     *
     * @param filePath Path to the JSON data configuration file.
     * @param data container object.
     */
    template <typename T>
    bool loadUnique(const std::string& filePath, T* data) {
        auto blueprint = getOrLoad<T>(filePath);
        if (blueprint) {
            *data = *blueprint;
            return true;
        }
        return false;
    }

    /**
     * @brief Loads a unique, isolated instance cloned from the cached JSON blueprint.
     * @note Ideal for polymorphic entity layouts like std::vector<std::unique_ptr<BaseClass>>.
     *
     * @param filePath Path to the JSON data configuration file.
     * @param data container object.
     */
    template <typename T>
    bool loadUnique(const std::string& filePath, T& data) {
        auto blueprint = getOrLoad<T>(filePath);
        if (blueprint) {
            data = *blueprint;
            return true;
        }
        return false;
    }

    /**
     * @brief Loads a raw value instance cloned from the cached configuration.
     * @note Perfect for ultra-fast, cache-friendly layouts like std::vector<T>.
     *
     * @param filePath Path to the JSON data configuration file.
     * @return T A separate, stack-allocated value copy of the object.
     * @throws std::runtime_error If the file fails to open or parse.
     */
    template <typename T>
    T loadValue(const std::string& filePath) {
        auto blueprint = getOrLoad<T>(filePath);
        if (!blueprint) {
            throw std::runtime_error("[ObjectManager] Failed to resolve value instance for: " + filePath);
        }
        return *blueprint;
    }

    /**
     * @brief Removes a single object tracking entry out of the active cache memory.
     * @param filePath The file path tracking key.
     */
    void invalidate(const std::string& filePath) {
        m_cache.erase(filePath);
    }

    /**
     * @brief Purges all loaded configuration templates currently tracked inside the manager.
     */
    void purgeCache() {
        m_cache.clear();
    }

    /**
     * @brief Evaluates whether an explicit object has already been cached.
     * @param filePath The file path tracking key.
     * @return true if the configuration exists inside the cache, false otherwise.
     */
    bool isCached(const std::string& filePath) const {
        return m_cache.find(filePath) != m_cache.end();
    }

private:
    // 
    /**
     * @brief Core engine module ensuring files are hit exactly once across lifetime execution
     * @param filePath The file path tracking key.
     * @return std::shared_ptr<T> A pointer to the cached object.
     */
    template <typename T>
    std::shared_ptr<T> getOrLoad(const std::string& filePath) {
        auto it = m_cache.find(filePath);
        if (it != m_cache.end()) {
            return std::any_cast<std::shared_ptr<T>>(it->second);
        }

        std::ifstream file(filePath);
        if (!file.is_open()) {
            logE << "[ObjectManager] Error: Unable to open file object at: " << filePath << std::endl;
            return nullptr;
        }

        try {
            json j;
            file >> j;

            auto blueprint = std::make_shared<T>();
            m_loader.load(j, *blueprint);
            m_cache[filePath] = std::any(blueprint);
            return blueprint;
        }
        catch (const json::exception& e) {
            logE << "[ObjectManager] JSON Parse Error [" << filePath << "]: " << e.what() << std::endl;
            return nullptr;
        }
        catch (const std::exception& e) {
            logE << "[ObjectManager] Generic Handling Exception: " << e.what() << std::endl;
            return nullptr;
        }
    }

    ObjectLoader m_loader;
    std::unordered_map<std::string, std::any> m_cache; /* Storage bucket caching unique file names to distinct shared pointers */
};
