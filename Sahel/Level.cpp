#include "Level.h"
#include "Entity.h"
#include "Light.h"
#include <Logger.h>
#include <algorithm>
#include <iostream>

Level::Level() = default;

Level::~Level()
{
    clear();
}

Entity& Level::createEntity()
{
    auto newEntity = std::make_unique<Entity>();
    int assignedIndex = static_cast<int>(m_entities.size());

	newEntity->setLevelIndex(assignedIndex);
	newEntity->setAlive(true);

    Entity& ref = *newEntity.get();
    m_entities.push_back(std::move(newEntity));

    return ref;
}

Light& Level::createLight()
{
    auto newLight = std::make_unique<Light>();
    int assignedIndex = static_cast<int>(m_lights.size());

    newLight->setIndex(assignedIndex);

    Light& ref = *newLight.get();
    m_lights.push_back(std::move(newLight));

    return ref;
}

Entity* Level::findEntity(int index) const
{
    if (index < 0 || index >= m_entities.size()) {
        return nullptr;
    }
    return m_entities[index].get();
}

bool Level::destroyEntity(int index)
{
    if (index < 0 || index >= static_cast<int>(m_entities.size())) {
        return false;
    }

    int lastIndex = static_cast<int>(m_entities.size()) - 1;

    if (index != lastIndex) {
        m_entities[index] = std::move(m_entities[lastIndex]);
        m_entities[index]->setLevelIndex(index);
    }
    m_entities.pop_back();

    return true;
}

bool Level::destroyLight(int index)
{
    if (index < 0 || index >= static_cast<int>(m_lights.size())) {
        return false;
    }

    int lastIndex = static_cast<int>(m_lights.size()) - 1;

    if (index != lastIndex) {
        m_lights[index] = std::move(m_lights[lastIndex]);
        m_lights[index]->setIndex(index);
    }
    m_lights.pop_back();

    return true;
}

void Level::clear()
{
    m_entities.clear();
    m_lights.clear();
}
