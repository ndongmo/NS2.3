#include "ObjectManager.h"
#include "PathHelper.h"
#include "Part.h"
#include "Shape2D.h"
#include "Body.h"
#include "Types.h"
#include <IOManager.h>
#include <fstream>
#include <iostream>
#include <regex>

ObjectManager::ObjectManager(NS2::ResourceManager& rsrcManager)
    : m_loader(rsrcManager, *this), m_rsrcManager(rsrcManager)
{
}

void ObjectManager::convertParts(const std::string& folderPath) {
    std::vector<NS2::DirEntry> entries;
    NS2::IOManager::getDirectoryEntries(folderPath.c_str(), entries, false);

    for (auto& entry : entries) {
        std::ifstream file(entry.path);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file.\n";
            return;
        }

        std::regex attr_regex(R"re(([a-zA-Z0-9_]+)="([^"]*)")re");
        std::string line;
        int line_number = 0;
        Part part;

        while (std::getline(file, line)) {
            ++line_number;

            // Detect if this line contains the <part tag
            if (line.find("<part ") != std::string::npos) {
                auto begin = std::sregex_iterator(line.begin(), line.end(), attr_regex);
                auto end = std::sregex_iterator();

                for (auto it = begin; it != end; ++it) {
                    std::string name = (*it)[1].str();   // Group 1: attribute name
                    std::string value = (*it)[2].str();  // Group 2: attribute value

                    if (name == "collide") {
                        part.m_collide = std::stoi(value);
                    }
                    else if (name == "index") {
                        part.m_index = std::stoi(value);
                    }
                    else if (name == "path") {
                        part.m_path = value;
                    }
                    else if (name == "col_r") {
                        part.m_sprite.color.r = std::stoi(value);
                    }
                    else if (name == "col_g") {
                        part.m_sprite.color.g = std::stoi(value);
                    }
                    else if (name == "col_b") {
                        part.m_sprite.color.b = std::stoi(value);
                    }
                    else if (name == "col_a") {
                        part.m_sprite.color.a = std::stoi(value);
                    }
                    else if (name == "dims_x") {
                        part.m_sprite.dims.x = std::stoi(value);
                    }
                    else if (name == "dims_y") {
                        part.m_sprite.dims.y = std::stoi(value);
                    }
                    else if (name == "depth") {
                        part.m_sprite.depth = TypeHelper::getDepthType2(std::stoi(value));
                    }
                    else if (name == "texture") {
                        part.m_sprite.texture = m_rsrcManager.getTexture(value);
                    }
                    else if (name == "uvr_z") {
                        part.m_sprite.uvRect.z = std::stof(value);
                    }
                    else if (name == "uvr_w") {
                        part.m_sprite.uvRect.y = std::stof(value);
                    }
                    else if (name == "inv_x") {
                        part.m_sprite.inverseX(std::stoi(value) != 0);
                    }
                    else if (name == "inv_y") {
                        part.m_sprite.inverseY(std::stoi(value) != 0);
                    }
                    else if (name == "tex_index") {
                        part.m_sprite.index = std::stoi(value);
                    }
                    else if (name == "shape_angle") {
                        part.m_shape.m_angle = std::stof(value);
                    }
                    else if (name == "shape_type") {
                        part.m_shape.m_type = static_cast<Shape2DType>(std::stoi(value));
                    }
                    else if (name == "shape_scale_x") {
                        part.m_shape.m_scale.x = std::stof(value);
                    }
                    else if (name == "shape_scale_y") {
                        part.m_shape.m_scale.y = std::stof(value);
                    }
                    else if (name == "shape_center_x") {
                        part.m_shape.m_center.x = std::stof(value);
                    }
                    else if (name == "shape_center_y") {
                        part.m_shape.m_center.y = std::stof(value);
                    }
                    else if (name == "shape_dim_x") {
                        part.m_shape.m_dimensions.x = std::stof(value);
                    }
                    else if (name == "shape_dim_y") {
                        part.m_shape.m_dimensions.y = std::stof(value);
                    }
                    else if (name == "shape_nbpoint") {
                        part.m_shape.m_points.resize(std::stoi(value));
                    }
                    else if (name == "shape_x_0") {
                        part.m_shape.m_points[0].x = std::stof(value);
                    }
                    else if (name == "shape_y_0") {
                        part.m_shape.m_points[0].y = std::stof(value);
                    }
                    else if (name == "shape_x_1") {
                        part.m_shape.m_points[1].x = std::stof(value);
                    }
                    else if (name == "shape_y_1") {
                        part.m_shape.m_points[1].y = std::stof(value);
                    }
                    else if (name == "shape_x_2") {
                        part.m_shape.m_points[2].x = std::stof(value);
                    }
                    else if (name == "shape_y_2") {
                        part.m_shape.m_points[2].y = std::stof(value);
                    }
                    else if (name == "shape_x_3") {
                        part.m_shape.m_points[3].x = std::stof(value);
                    }
                    else if (name == "shape_y_3") {
                        part.m_shape.m_points[3].y = std::stof(value);
                    }
                    else if (name == "drawshape_angle") {
                        part.m_drawShape.m_angle = std::stof(value);
                    }
                    else if (name == "drawshape_type") {
                        part.m_drawShape.m_type = static_cast<Shape2DType>(std::stoi(value));
                    }
                    else if (name == "drawshape_scale_x") {
                        part.m_drawShape.m_scale.x = std::stof(value);
                    }
                    else if (name == "drawshape_scale_y") {
                        part.m_drawShape.m_scale.y = std::stof(value);
                    }
                    else if (name == "drawshape_center_x") {
                        part.m_drawShape.m_center.x = std::stof(value);
                    }
                    else if (name == "drawshape_center_y") {
                        part.m_drawShape.m_center.y = std::stof(value);
                    }
                    else if (name == "drawshape_dim_x") {
                        part.m_drawShape.m_dimensions.x = std::stof(value);
                    }
                    else if (name == "drawshape_dim_y") {
                        part.m_drawShape.m_dimensions.y = std::stof(value);
                    }
                    else if (name == "drawshape_nbpoint") {
                        part.m_drawShape.m_points.resize(std::stoi(value));
                    }
                    else if (name == "drawshape_x_0") {
                        part.m_drawShape.m_points[0].x = std::stof(value);
                    }
                    else if (name == "drawshape_y_0") {
                        part.m_drawShape.m_points[0].y = std::stof(value);
                    }
                    else if (name == "drawshape_x_1") {
                        part.m_drawShape.m_points[1].x = std::stof(value);
                    }
                    else if (name == "drawshape_y_1") {
                        part.m_drawShape.m_points[1].y = std::stof(value);
                    }
                    else if (name == "drawshape_x_2") {
                        part.m_drawShape.m_points[2].x = std::stof(value);
                    }
                    else if (name == "drawshape_y_2") {
                        part.m_drawShape.m_points[2].y = std::stof(value);
                    }
                    else if (name == "drawshape_x_3") {
                        part.m_drawShape.m_points[3].x = std::stof(value);
                    }
                    else if (name == "drawshape_y_3") {
                        part.m_drawShape.m_points[3].y = std::stof(value);
                    }
                }
            }
        }
        int type = getType(part.m_path, GameType::TYPE_PART);
        const BodyPart& bodypart = TypeHelper::getBodyPart(type);
        auto joints = TypeHelper::getChildren(bodypart.bodyType, bodypart.partIndex);
        for (const auto* joint : joints) {
            part.addJoint(joint->jointIndex);
        }
        const BodyJoint* parentJoint = TypeHelper::getParent(bodypart.bodyType, bodypart.partIndex);
        if (parentJoint != nullptr) {
            part.setParentJoint(parentJoint->jointIndex);
        }
        part.m_type = bodypart.partType;
        part.m_index = bodypart.partIndex;
        part.m_oppositePart = bodypart.oppositeIndex;
        part.m_path = getPath(part.m_path, GameType::TYPE_PART, type);

        if (save(part.m_path, part)) {
            std::cout << part.m_path << " was saved successfully!" << std::endl;
        }
        else {
            std::cout << part.m_path << " failed to save!" << std::endl;
        }
    }
}

void ObjectManager::convertBodies(const std::string& folderPath)
{
    std::vector<NS2::DirEntry> entries;
    NS2::IOManager::getDirectoryEntries(folderPath.c_str(), entries, false);

    for (auto& entry : entries) {
        std::ifstream file(entry.path);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file.\n";
            return;
        }

        std::regex attr_regex(R"re(([a-zA-Z0-9_]+)="([^"]*)")re");
        std::string line;
        int line_number = 0;
        Body body;

        while (std::getline(file, line)) {
            ++line_number;

            // Detect if this line contains the <body tag
            if (line.find("<body ") != std::string::npos) {
                auto begin = std::sregex_iterator(line.begin(), line.end(), attr_regex);
                auto end = std::sregex_iterator();

                for (auto it = begin; it != end; ++it) {
                    std::string name = (*it)[1].str();   // Group 1: attribute name
                    std::string value = (*it)[2].str();  // Group 2: attribute value

                    if (name == "collisionShape") {
                        body.m_shape.m_type = static_cast<Shape2DType>(std::stoi(value));
                    }
                    else if (name == "type") {
                        body.m_type = static_cast<BodyType>(std::stoi(value));
                    }
                    else if (name == "material") {
                        body.m_material = static_cast<MaterialType>(std::stoi(value));
                    }
                    else if (name == "path") {
                        body.m_path = value;
                    }
                    else if (name == "aspeed") {
                        body.m_angularSpeed = std::stof(value);
                    }
                    else if (name == "mass") {
                        body.m_mass = std::stof(value);
                    }
                    else if (name == "nbjoint") {
                        body.m_joints.resize(std::stoi(value));
                    }
                    else if (name == "nbpart") {
                        body.m_parts.resize(std::stoi(value));
                    }
                }
            }
            else if (line.find("<part_path ") != std::string::npos) {
                auto begin = std::sregex_iterator(line.begin(), line.end(), attr_regex);
                auto end = std::sregex_iterator();
                int index = -1;

                for (auto it = begin; it != end; ++it) {
                    std::string name = (*it)[1].str();   // Group 1: attribute name
                    std::string value = (*it)[2].str();  // Group 2: attribute value

                    if (name == "index") {
                        index = std::stoi(value);
                    }
                    else if (name == "path") {
                        int type = getType(value, GameType::TYPE_PART);
                        std::string realPath = getPath(value, GameType::TYPE_PART, type);
                        loadUnique(realPath, body.m_parts[index]);
                    }
                }
            }
            else if (line.find("<joint ") != std::string::npos) {
                auto begin = std::sregex_iterator(line.begin(), line.end(), attr_regex);
                auto end = std::sregex_iterator();
                int index = -1;

                for (auto it = begin; it != end; ++it) {
                    std::string name = (*it)[1].str();   // Group 1: attribute name
                    std::string value = (*it)[2].str();  // Group 2: attribute value

                    if (name == "index") {
                        index = std::stoi(value);
                    }
                    else if (name == "parent") {
                        body.m_joints[index].parent = std::stoi(value);
                    }
                    else if (name == "son") {
                        body.m_joints[index].child = std::stoi(value);
                    }
                    else if (name == "x") {
                        body.m_joints[index].point.x = std::stof(value);
                    }
                    else if (name == "y") {
                        body.m_joints[index].point.y = std::stof(value);
                    }
                    else if (name == "lower_a") {
                        body.m_joints[index].lower.a = std::stof(value);
                    }
                    else if (name == "lower_x") {
                        body.m_joints[index].lower.pos.x = std::stof(value);
                    }
                    else if (name == "lower_y") {
                        body.m_joints[index].lower.pos.y = std::stof(value);
                    }
                    else if (name == "upper_a") {
                        body.m_joints[index].upper.a = std::stof(value);
                    }
                    else if (name == "upper_x") {
                        body.m_joints[index].upper.pos.x = std::stof(value);
                    }
                    else if (name == "upper_y") {
                        body.m_joints[index].upper.pos.y = std::stof(value);
                    }
                }
            }
            else if (line.find("<part ") != std::string::npos) {
                auto begin = std::sregex_iterator(line.begin(), line.end(), attr_regex);
                auto end = std::sregex_iterator();
                int index = -1;
                glm::vec2 pos = glm::vec2(0.0f);
                glm::vec2 scale = glm::vec2(0.0f);

                for (auto it = begin; it != end; ++it) {
                    std::string name = (*it)[1].str();   // Group 1: attribute name
                    std::string value = (*it)[2].str();  // Group 2: attribute value

                    if (name == "index") {
                        index = std::stoi(value);
                        pos = body.m_parts[index].getPos();
                        scale = body.m_parts[index].getScale();
                    }
                    else if (name == "x") {
                        pos.x = std::stof(value);
                        body.m_parts[index].setPos(pos);
                    }
                    else if (name == "y") {
                        pos.y = std::stof(value);
                        body.m_parts[index].setPos(pos);
                    }
                    else if (name == "scale_x") {
                        scale.x = std::stof(value);
                        body.m_parts[index].setScale(scale);
                    }
                    else if (name == "scale_y") {
                        scale.y = std::stof(value);
                        body.m_parts[index].setScale(scale);
                    }
                    else if (name == "angle") {
                        body.m_parts[index].setAngle(std::stof(value));
                    }
                }
            }
        }
        int type = getType(body.m_path, GameType::TYPE_BODY);
        body.m_path = getPath(body.m_path, GameType::TYPE_BODY, type);

        if (save(body.m_path, body)) {
            std::cout << body.m_path << " was saved successfully!" << std::endl;
        }
        else {
            std::cout << body.m_path << " failed to save!" << std::endl;
        }
    }
}

int ObjectManager::getType(const std::string& path, int gameType)
{
    if (gameType == GameType::TYPE_PART) {
        PartType type = PartType::PART_COUNT;
        for (const auto& [typeKey, typeName] : TypeHelper::partTypes) {
            if (path.find(typeName + "_") != std::string::npos) {
                return typeKey;
            }
            else if (typeKey == PartType::PART_TORSO && path.find("BODY_") != std::string::npos) {
                return PartType::PART_TORSO;
            }
        }
    }
    else if (gameType == GameType::TYPE_BODY) {
        for (const auto& [typeKey, typeName] : TypeHelper::bodyTypes) {
            if (path.find(typeName + "_") != std::string::npos) {
                return typeKey;
            }
        }
    }
    throw std::runtime_error("Type not found for " + path);
}

std::string ObjectManager::getPath(const std::string& path, int gameType, int type)
{
    std::string name = PathHelper::getFileName(path);

    if (gameType == GameType::TYPE_PART) {
        size_t pos = name.find("BODY_");
        if (pos != std::string::npos) {
            name.replace(pos, 5, "TORSO_");
        }
        std::string namePrefix = TypeHelper::partTypes[type] + "_";
        pos = name.find(namePrefix);
        if (pos != std::string::npos) {
            name.insert(pos + namePrefix.length(), "PART_");
            return PathHelper::getObjectPath(name, GameType::TYPE_PART);
        }
    }
    else if (gameType == GameType::TYPE_BODY) {
        std::string namePrefix = TypeHelper::bodyTypes[type] + "_";
        size_t pos = name.find(namePrefix);
        if (pos != std::string::npos) {
            name.insert(pos + namePrefix.length(), "BODY_");
            return PathHelper::getObjectPath(name, GameType::TYPE_BODY);
        }
    }
    throw std::runtime_error("Trasform failed for " + path);
}
