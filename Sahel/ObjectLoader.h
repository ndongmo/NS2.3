/*!
 * \file Level.h
 * \brief Central world container managing game entity lifecycles, packed storage layout, and physics synchronizations.
 * \author Ndongmo Silatsa Fabrice
 * \date 28-08-2026
 * \version 1.0
 */

#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <box3d/box3d.h>
#include <nlohmann/json.hpp>

class ObjectManager;
class Shape2D;
class Joint2D;
class BodyPart;
class Body;
class Tile;
struct Light;
class Entity;
class Level;

namespace NS2 {
	class TileSheet;
    class ResourceManager;
}

using json = nlohmann::json;

class ObjectLoader {
public:
    /*!
     * \brief Constructor for the ObjectLoader.
     * \param rsrcManager: resource manager
     * \param objManager: object manager
     */
    ObjectLoader(NS2::ResourceManager& rsrcManager, ObjectManager& objManager);

    /* 
	 * \brief Loads a JSON object into the specified target structure.
	 * \param j: JSON object to load
	 * \param any: target structure to populate
     */
	bool load(const json& j, Level& l);
    bool load(const json& j, Entity& e);
    bool load(const json& j, Light& l);
    bool load(const json& j, Tile& t);
    bool load(const json& j, Body& b);
    bool load(const json& j, BodyPart& bp);
    bool load(const json& j, Joint2D& j2d);
    bool load(const json& j, Shape2D& s);
    bool load(const json& j, NS2::TileSheet& ts);

private:
    ObjectManager& m_objManager;          /* Current object manager */
    NS2::ResourceManager& m_rsrcManager;  /* Current resource manager */
};