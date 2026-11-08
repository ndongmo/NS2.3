#include "ILinkedBody.h"
#include "MathUtils.h"
#include "Body.h"

void ILinkedBody::lattach(ITransform* link)
{
	if (std::find(m_links.begin(), m_links.end(), link) == m_links.end()) {
		m_links.push_back(link);
	}
}
void ILinkedBody::ldetach(ITransform* link)
{
	for (size_t i = 0; i < m_links.size(); i++) {
		if (m_links[i] == link) {
			m_links[i] = m_links.back();
			m_links.pop_back();
			return;
		}
	}
}

void ILinkedBody::ldetachAll()
{
	m_links.clear();
}

void ILinkedBody::lrotate(float angle, glm::vec2 pos)
{
	for (ITransform* link : m_links) {
		if (link != nullptr) {
			if(dynamic_cast<Body*>(link) != nullptr)
				link->rotate(angle * RADTODEG, pos);
			else 
				link->rotate(angle, pos);
		}
		else ldetach(link);
	}
}
void ILinkedBody::lincrPos(glm::vec2 pos)
{
	for (ITransform* link : m_links) {
		if (link != nullptr) link->incrPos(pos);
		else ldetach(link);
	}
}
void ILinkedBody::lsetPos(glm::vec2 pos)
{
	for (ITransform* link : m_links) {
		if (link != nullptr) link->setPos(pos);
		else ldetach(link);
	}
}
void ILinkedBody::lsetCenter(glm::vec2 center)
{
	for (ITransform* link : m_links) {
		if (link != nullptr) link->setCenter(center);
		else ldetach(link);
	}
}
void ILinkedBody::lsetAngle(float angle, glm::vec2 pos)
{
	for (ITransform* link : m_links) {
		if (link != nullptr) link->setAngle(angle, pos);
		else ldetach(link);
	}
}