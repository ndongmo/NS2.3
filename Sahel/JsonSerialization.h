#pragma once

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <CoreTypes.h>

#include "Move2D.h"
#include "Joint2D.h"

using json = nlohmann::json;

// ==========================================
// GLM Type Mappings (Inside glm namespace)
// ==========================================
namespace glm {
    inline void from_json(const json& j, vec2& v) {
        v.x = j.at(0).get<float>();
        v.y = j.at(1).get<float>();
    }

    inline void to_json(json& j, const vec2& v) {
        j = json{ v.x, v.y };
    }

    inline void from_json(const json& j, ivec2& v) {
        v.x = j.at(0).get<int>();
        v.y = j.at(1).get<int>();
    }

    inline void to_json(json& j, const ivec2& v) {
        j = json{ v.x, v.y };
    }

    inline void from_json(const json& j, vec3& v) {
        v.x = j.at(0).get<float>();
        v.y = j.at(1).get<float>();
        v.z = j.at(2).get<float>();
    }

    inline void to_json(json& j, const vec3& v) {
        j = json{ v.x, v.y, v.z };
    }

    inline void from_json(const json& j, vec4& v) {
        v.x = j.at(0).get<float>();
        v.y = j.at(1).get<float>();
        v.z = j.at(2).get<float>();
        v.w = j.at(3).get<float>();
    }

    inline void to_json(json& j, const vec4& v) {
        j = json{ v.x, v.y, v.z, v.w };
    }
}

// ==========================================
// Custom Engine Type Mappings
// ==========================================
namespace NS2 {
    inline void from_json(const json& j, ColorRGBA8& c) {
        c.r = j.at(0).get<unsigned int>();
        c.g = j.at(1).get<unsigned int>();
        c.b = j.at(2).get<unsigned int>();
        c.a = j.at(3).get<unsigned int>();
    }

    inline void to_json(json& j, const ColorRGBA8& c) {
        // Cast components to unsigned int so they print as numbers, not ASCII text characters
        j = json{
            static_cast<unsigned int>(c.r),
            static_cast<unsigned int>(c.g),
            static_cast<unsigned int>(c.b),
            static_cast<unsigned int>(c.a)
        };
    }
}

// ==========================================
// Custom Game Type Mappings
// ==========================================
inline void from_json(const json& j, Move2D& m) {
    m.x = j.at(0).get<float>();
    m.y = j.at(1).get<float>();
    m.a = j.at(2).get<float>();
}

inline void to_json(json& j, const Move2D& m) {
    j = json{ m.x, m.y, m.a };
}

inline void from_json(const json& j, Joint2D& j2d) {
    j2d.index = j.at("index").get<unsigned int>();
    j2d.parent = j.at("parent").get<unsigned int>();
    j2d.son = j.at("son").get<unsigned int>();
    j2d.point = j.at("point").get<glm::vec2>();
    j2d.lower = j.at("lower").get<Move2D>();
    j2d.upper = j.at("upper").get<Move2D>();
}

inline void to_json(json& j, const Joint2D& j2d) {
    j["index"] = j2d.index;
    j["parent"] = j2d.parent;
    j["son"] = j2d.son;
    j["point"] = j2d.point;
    j["lower"] = j2d.lower;
    j["upper"] = j2d.upper;
}