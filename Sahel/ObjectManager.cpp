#include "ObjectManager.h"

ObjectManager::ObjectManager(NS2::ResourceManager& rsrcManager)
    : m_loader(rsrcManager, *this)
{
}
