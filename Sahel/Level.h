/*!
 * \file Level.h
 * \brief Central world container managing game entity lifecycles, packed storage layout, and physics synchronizations.
 * \author Ndongmo Silatsa Fabrice
 * \date 28-08-2026
 * \version 1.0
 */

#pragma once

#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>

#include "IFileManager.h"
#include "TileGrid.h"

class ObjectLoader;
class ObjectSaver;
class Entity;
struct Light;

class Level : public IFileManager
{
    friend ObjectLoader;
	friend ObjectSaver;

public:
    /*!
     * \brief Constructs a new level sandbox.
     */
    Level();

    /*!
     * \brief Destructor cleans up all entities and explicitly unbinds related Box3D bodies.
     */
    ~Level();

    // Prevent accidental duplicating or copying of unique simulation instances
    Level(const Level&) = delete;
    Level& operator=(const Level&) = delete;

    // Allow safely transferring ownership of the level state container
    Level(Level&&) noexcept = default;
    Level& operator=(Level&&) noexcept = default;

    /** ENTITY LIFECYCLE PIPELINES */

    /*!
     * \brief Factory method that spawns a raw Entity directly at the end of the cache-packed storage vector.
     * \return Non-owning reference allowing immediate component initialization.
     */
    Entity& createEntity();
	Light& createLight();

    /*!
     * \brief Destroys an entity and cleans up memory instantly using the high-performance Swap-and-Pop routine.
     * \param index: Target 0-based vector index position to purge.
     * \return True if the operation succeeded, false if the requested index was invalid.
     */
    bool destroyEntity(int index);
	bool destroyLight(int index);

    /*!
     * \brief Iterates and purges the entire active entity pipeline, cleanly stripping Box3D rigid bodies.
     */
    void clear();

    /** RUNTIME Ticks & LOOKUPS */

    /*!
     * \brief Direct O(1) memory array access lookup. Safely captures unassigned indices via a -1 sentinel pass.
     * \param index: Target index to find.
     * \return Safe raw pointer to the object, or nullptr if the index is out of bounds or -1.
     */
    Entity* findEntity(int index) const;

    /** ACCESSORS */
    const std::string& getName() const { return m_name; }
    const glm::vec3& getAmbientColor() const { return m_ambientColor; }
    const std::vector<std::unique_ptr<Entity>>& getEntities() const { return m_entities; }
	const std::vector<std::unique_ptr<Light>>& getLights() const { return m_lights; }
    const TileGrid& getTileGrid() const { return m_tileGrid; }

private:
    std::string m_name;                                      
    glm::vec3 m_ambientColor{ 0.1f, 0.1f, 0.1f };
    std::vector<std::unique_ptr<Entity>> m_entities;            /* Level's entities */
    std::vector<std::unique_ptr<Light>> m_lights;               /* Level's dynamic lights */
	TileGrid m_tileGrid;                                        /* Level's tile grid */
};
