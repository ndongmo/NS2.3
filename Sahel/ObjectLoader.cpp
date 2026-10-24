#include "ObjectLoader.h"
#include "ObjectManager.h"
#include "JsonSerialization.h"
#include "Shape2D.h"
#include "BodyPart.h"
#include "Body.h"
#include "Tile.h"
#include "Light.h"
#include "Entity.h"
#include "Level.h"
#include <Logger.h>
#include <TileSheet.h>
#include <ResourceManager.h>

ObjectLoader::ObjectLoader(NS2::ResourceManager& rsrcManager, ObjectManager& objManager)
    : m_rsrcManager(rsrcManager), m_objManager(objManager)
{
}

bool ObjectLoader::load(const json& j, Level& l)
{
	try {
		l.m_name = j.at("name").get<std::string>();
		l.m_path = j.at("path").get<std::string>();
		l.m_ambientColor = j.at("ambientColor").get<glm::vec3>();

		size_t nbLights = j.at("lights").size();
		size_t nbEntities = j.at("entities").size();
		l.m_lights.resize(nbLights);
		l.m_entities.reserve(nbEntities);

		for (int i = 0; i < nbLights; ++i) {
			l.m_lights[i] = std::make_unique<Light>();
			l.m_lights[i]->index = i;
		}

		int width = j.at("width").get<int>();
		int height = j.at("height").get<int>();
		l.m_tileGrid.init(width, height);

		for (const auto& tile : j.at("tiles")) {
			std::unique_ptr<Tile> t = m_objManager.loadUnique<Tile>(tile.at("path").get<std::string>());
			t->setPosX(tile.at("x").get<int>());
			t->setPosY(tile.at("y").get<int>());
			t->setTexWidth(tile.at("width").get<float>());
			t->setTexHeight(tile.at("height").get<float>());
			l.m_tileGrid.addTile(std::move(t));
		}

		for (const auto& light : j.at("lights")) {
			int idx = light.at("index").get<int>();
			Light* lgt = l.m_lights[idx].get();
			m_objManager.loadUnique<Light>(light.at("path").get<std::string>(), lgt);
			lgt->position = light.at("position").get<glm::vec2>();
			lgt->radius = light.value("radius", lgt->radius);
			lgt->active = light.value("active", lgt->active);
			lgt->index = idx;
		}

		for (const auto& entity : j.at("entities")) {
			int idx = entity.at("index").get<int>();
			EntityID id = entity.at("id").get<EntityID>();
			// to do: load entity from path and set its index and id
		}

		return true;
	}
	catch (const std::exception& e) {
		logE << "ObjectLoader[Level]: " << e.what() << "\n" << j.dump(4) << std::endl;
	}
	return false;
}

bool ObjectLoader::load(const json& j, Entity& e)
{
	try {
		e.m_path = j.at("path").get<std::string>();
		e.m_name = j.at("name").get<std::string>();
		e.m_active = j.at("active").get<bool>();
		e.m_collide = j.at("collide").get<bool>();
		e.m_interact = j.at("interact").get<bool>();
		e.m_state = j.at("state").get<EntityState>();
		e.m_filter = j.at("filter").get<long>();
		m_objManager.loadUnique(j.at("body").get<std::string>(), e.m_body);

		return true;
	}
	catch (const std::exception& e) {
		logE << "ObjectLoader[Entity]: " << e.what() << "\n" << j.dump(4) << std::endl;
	}
	return false;
}

bool ObjectLoader::load(const json& j, Light& l)
{
	try {
		l.path = j.at("path").get<std::string>();
		l.type = j.at("type").get<LightType>();
		l.direction = j.at("direction").get<glm::vec2>();
		l.height = j.at("height").get<float>();
		l.color = j.at("color").get<glm::vec3>();
		l.intensity = j.at("intensity").get<float>();
		l.radius = j.at("radius").get<float>();
		l.innerAngle = j.at("innerAngle").get<float>();
		l.outerAngle = j.at("outerAngle").get<float>();

		return true;
	}
	catch (const std::exception& e) {
		logE << "ObjectLoader[Light]: " << e.what() << "\n" << j.dump(4) << std::endl;
	}
	return false;
}

bool ObjectLoader::load(const json& j, Tile& t)
{
	try {
		t.m_path = j.at("path").get<std::string>();
		t.m_material = j.at("material").get<MaterialType>();
		t.m_color = j.at("color").get<NS2::ColorRGBA8>();
		t.m_texture = m_rsrcManager.getTexture(j.at("texture").get<std::string>());

		return true;
	}
	catch (const std::exception& e) {
		logE << "ObjectLoader[Tile]: " << e.what() << "\n" << j.dump(4) << std::endl;
	}
	return false;
}

bool ObjectLoader::load(const json& j, Body& b)
{
	try {
		b.m_path = j.at("path").get<std::string>();
		b.m_material = j.at("material").get<MaterialType>();
		b.m_density = j.at("density").get<float>();
		b.m_friction = j.at("friction").get<float>();
		b.m_restitution = j.at("restitution").get<float>();
		b.m_mass = j.at("mass").get<float>();
		b.m_angularSpeed = j.at("aspeed").get<float>();
		b.m_joints = j.at("joints").get<std::vector<Joint2D>>();

		load(j.at("shape"), b.m_shape);

		// load all body parts
		b.m_parts.clear();
		for (const std::string& partPath : 
			j.at("parts_path").get<std::vector<std::string>>()) {
			b.m_parts.emplace_back();
			m_objManager.loadUnique(partPath, b.m_parts.back());
		}

		// update the part joints
		size_t nbParts = b.m_parts.size();
		for (const auto& j : b.m_joints) {
			if (j.parent != j.son && j.parent >= 0 && j.parent < nbParts) {
				b.m_parts[j.parent].addJoint(j.index);
			}
			if (j.son >= 0 && j.son < nbParts) {
				b.m_parts[j.son].setParentJoint(j.index);
			}
		}

		// finally, update the part transformation
		unsigned int index = 0;
		for (const auto& part : j.at("parts_transform")) {
			float angle = part.at("angle").get<float>();
			glm::vec2 scale = part.at("scale").get<glm::vec2>();
			glm::vec2 pos = part.at("pos").get<glm::vec2>();

			// order is important!!!
			b.initScale(index, scale.x, scale.y);
			b.initAngle(index, angle);
			b.initPos(index, pos.x, pos.y);
			index++;
		}

		b.updateShape(); // update the body shape after all parts are loaded and transformed

		return true;
	}
	catch (const std::exception& e) {
		logE << "ObjectLoader[Body]: " << e.what() << "\n" << j.dump(4) << std::endl;
	}
	return false;
}

bool ObjectLoader::load(const json& j, BodyPart& bp)
{
	try {
		bp.m_collide = j.at("collide").get<bool>();
		bp.m_scalable = j.at("scalable").get<bool>();
		bp.m_hidden = j.at("hidden").get<bool>();
		bp.m_index = j.at("index").get<int>();
		bp.m_material = j.at("material").get<MaterialType>();
		bp.m_path = j.at("path").get<std::string>();

		load(j.at("sprite"), bp.m_sprite);
		load(j.at("shape"), bp.m_shape);
		load(j.at("drawshape"), bp.m_drawShape);

		return true;
	}
	catch (const std::exception& e) {
		logE << "ObjectLoader[BodyPart]: " << e.what() << "\n" << j.dump(4) << std::endl;
	}
	return false;
}

bool ObjectLoader::load(const json& j, Joint2D& j2d)
{
	try {
		j2d.index = j.at("index").get<unsigned int>();
		j2d.parent = j.at("parent").get<unsigned int>();
		j2d.son = j.at("son").get<unsigned int>();
		j2d.point = j.at("point").get<glm::vec2>();
		j2d.lower = j.at("lower").get<Move2D>();
		j2d.upper = j.at("upper").get<Move2D>();

		return true;
	}
	catch (const std::exception& e) {
		logE << "ObjectLoader[Joint2D]: " << e.what() << "\n" << j.dump(4) << std::endl;
	}
	return false;
}

bool ObjectLoader::load(const json& j, Shape2D& s)
{
	try {
		s.m_type = j.at("type").get<Shape2DType>();
		s.m_angle = j.at("angle").get<float>();
		s.m_scale = j.at("scale").get<glm::vec2>();
		s.m_center = j.at("center").get<glm::vec2>();
		s.m_dimensions = j.at("dim").get<glm::vec2>();
		s.m_points = j.at("points").get<std::vector<glm::vec2>>();

		return true;
	}
	catch (const std::exception& e) {
		logE << "ObjectLoader[TileSheet]: " << e.what() << "\n" << j.dump(4) << std::endl;
	}
	return false;
}

bool ObjectLoader::load(const json& j, NS2::TileSheet& ts)
{
	try {
		ts.init(
			m_rsrcManager.getTexture(j.at("texture").get<std::string>()),
			j.at("color").get<NS2::ColorRGBA8>(),
			j.at("dims").get<glm::ivec2>(),
			j.at("invs").at(0).get<bool>(),
			j.at("invs").at(1).get<bool>(),
			j.at("depth").get<unsigned int>()
		);

		return true;
	} catch (const std::exception& e) {
		logE << "ObjectLoader[TileSheet]: " << e.what() << "\n" << j.dump(4) << std::endl;
	}
	return false;
}
