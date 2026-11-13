#include "ObjectSaver.h"
#include "JsonSerialization.h"
#include "Shape2D.h"
#include "Part.h"
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
		lightJson["index"] = light->m_index;
		lightJson["path"] = light->getPath();
		lightJson["pos"] = light->m_shape.getPos();
        lightJson["dims"] = light->m_shape.getDims();
		lightJson["active"] = light->m_active;
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
    j["path"] = l.getPath();
    j["color"] = l.m_color;
    j["dims"] = l.m_shape.getDims();
    j["shape"] = l.m_shape.getType();
    j["texture"] = l.m_texture->filePath;
    return j;
}

json ObjectSaver::save(const Body& b)
{
    json j;
	j["type"] = b.m_type; 
    j["path"] = b.m_path;
    j["material"] = b.m_material;
    j["mass"] = b.m_mass;
    j["aspeed"] = b.m_angularSpeed;
    j["shape"] = b.m_shape.getType();
    j["joints"] = b.m_joints;
    j["nb_parts"] = b.m_parts.size();

    for (const auto& p : b.m_parts) {
        json jj;
        jj["index"] = p.m_index;
        jj["path"] = p.getPath();
        j["parts_path"].push_back(jj);

    }
    for (const auto& p : b.m_parts) {
        json jj;
        jj["index"] = p.m_index;
        jj["angle"] = p.getAngle();
        jj["scale"] = p.getScale();
        jj["pos"] = p.getPos();

        j["parts_transform"].push_back(jj);
    }

    return j;
}

json ObjectSaver::save(const Part& bp)
{
    json j;
	j["type"] = bp.m_type; 
    j["index"] = bp.m_index;
    j["opposite_part"] = bp.m_oppositePart;
    j["parent_joint"] = bp.m_parentJoint;
    j["child_joints"] = bp.m_joints;
    j["collide"] = bp.m_collide;
	j["scalable"] = bp.m_scalable;
	j["hidden"] = bp.m_hidden;
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
    j["texture"] = (ts.texture != nullptr) ? ts.texture->filePath : "";
    j["color"] = ts.color;
    j["dims"] = ts.getDims();
    j["invs"] = { ts.getInvX(), ts.getInvY() };
    j["depth"] = static_cast<unsigned int>(ts.depth); // Keep casting to avoid ASCII conversion
    return j;
}
