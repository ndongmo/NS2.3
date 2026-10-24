#include "ObjectSaver.h"
#include "JsonSerialization.h"
#include "Shape2D.h"
#include "BodyPart.h"
#include "Body.h"
#include "Light.h"
#include "Tile.h"
#include "Entity.h"
#include "Level.h"
#include <TileSheet.h>
#include <fstream>

json ObjectSaver::save(const Level& l)
{
    json j;
	j["path"] = l.m_path;
	j["name"] = l.m_name;
	j["ambientColor"] = l.m_ambientColor;
	
	for (const auto& tile : l.m_tileGrid.getTiles()) {
		json tileJson;
		tileJson["path"] = tile->getPath();
		tileJson["x"] = tile->getPosX();
		tileJson["y"] = tile->getPosY();
		tileJson["width"] = tile->getTexWidth();
		tileJson["height"] = tile->getTexHeight();
		j["tiles"].push_back(tileJson);
	}
	for (const auto& light : l.m_lights) {
		json lightJson;
		lightJson["index"] = light->index;
		lightJson["path"] = light->path;
		lightJson["position"] = light->position;
		lightJson["radius"] = light->radius;
		lightJson["active"] = light->active;
		j["lights"].push_back(lightJson);
	}
    return j;
}

json ObjectSaver::save(const Entity& e)
{
    json j;
    j["path"] = e.m_path;
    j["name"] = e.m_name;
    j["active"] = e.m_active;
    j["collide"] = e.m_collide;
    j["interact"] = e.m_interact;
    j["state"] = e.m_state;
    j["filter"] = e.m_filter;
    j["body"] = e.m_body.getPath();
    return j;
}

json ObjectSaver::save(const Tile& t)
{
    json j;
    j["path"] = t.m_path;
    j["material"] = t.m_material;
    j["color"] = t.m_color;
    j["texture"] = t.m_texture->filePath;
    return j;
}

json ObjectSaver::save(const Light& l)
{
    json j;
    j["path"] = l.path;
    j["type"] = l.type;
    j["direction"] = l.direction;
    j["height"] = l.height;
    j["color"] = l.color;
    j["intensity"] = l.intensity;
    j["radius"] = l.radius;
    j["innerAngle"] = l.innerAngle;
    j["outerAngle"] = l.outerAngle;
    return j;
}

json ObjectSaver::save(const Body& b)
{
    json j;
    j["path"] = b.m_path;
    j["material"] = b.m_material;
    j["density"] = b.m_density;
    j["friction"] = b.m_friction;
    j["restitution"] = b.m_restitution;
    j["mass"] = b.m_mass;
    j["aspeed"] = b.m_angularSpeed;
    j["shape"] = save(b.m_shape);
    j["joints"] = b.m_joints;

    for (const auto& p : b.m_parts) {
        j["parts_path"].push_back(p.getPath());

    }
    for (const auto& p : b.m_parts) {
        json jj;
        jj["angle"] = p.getAngle();
        jj["scale"] = glm::vec2(p.getScaleX(), p.getScaleY());
        jj["pos"] = glm::vec2(p.getPosX(), p.getPosY());

        j["parts_transform"].push_back(jj);
    }

    return j;
}

json ObjectSaver::save(const BodyPart& bp)
{
    json j;
    j["collide"] = bp.m_collide;
	j["scalable"] = bp.m_scalable;
	j["hidden"] = bp.m_hidden;
	j["index"] = bp.m_index;
	j["material"] = bp.m_material; 
	j["path"] = bp.m_path;
	j["sprite"] = save(bp.m_sprite);
	j["shape"] = save(bp.m_shape);
	j["drawshape"] = save(bp.m_drawShape);
	return j;
}

json ObjectSaver::save(const Shape2D& s)
{
    json j;
    j["type"] = s.m_type;
    j["angle"] = s.m_angle;
    j["scale"] = s.m_scale;
    j["center"] = s.m_center;
    j["dim"] = s.m_dimensions;
    j["points"] = s.m_points;
    return j;
}

json ObjectSaver::save(const NS2::TileSheet& ts)
{
    json j;
    j["texture"] = ts.texture->filePath;
    j["color"] = ts.color;
    j["dims"] = ts.getDims();
    j["invs"] = { ts.getInvX(), ts.getInvY() };
    j["depth"] = static_cast<unsigned int>(ts.depth); // Keep casting to avoid ASCII conversion
    return j;
}
