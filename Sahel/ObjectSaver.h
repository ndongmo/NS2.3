/*!
 * \file ObjectSaver.h
 * \brief Object saver. It saves game objects to json files.
 * \author Ndongmo Silatsa Fabrice
 * \date 28-08-2026
 * \version 1.0
 */

#pragma once

#include <nlohmann/json.hpp>

class Shape2D;
class BodyPart;
class Body;
struct Light;
class Tile;
class Entity;
class Level;

namespace NS2 {
    class TileSheet;
}

using json = nlohmann::json;

class ObjectSaver {
public:
	static json save(const Level& l);
	static json save(const Entity& e);
	static json save(const Tile& t);
	static json save(const Light& l);
    static json save(const Body& b);
    static json save(const BodyPart& bp);

private:
    /** Helper save methods **/
    static json save(const Shape2D& s);
    static json save(const NS2::TileSheet& ts);
};
