/**
* @file Light.h
* @brief Structural definition for dynamic light.
* @author Ndongmo Silatsa Fabrice
*/

#pragma once

#include "IFileManager.h"
#include "IDraw.h"
#include "Types.h"

#include <CoreTypes.h>
#include <glm/glm.hpp>
#include <ctime>
#include <random>

class ObjectLoader;
class ObjectSaver;
class LightEditor;

class Light : public IDraw, public IFileManager
{
	friend ObjectLoader;
	friend ObjectSaver;
	friend LightEditor;

public:
	Light();

	virtual void draw(NS2::SpriteBatch& spriteBatch) const override;
	virtual void drawDebug(NS2::DebugRenderer& debugRender, const NS2::ColorRGBA8& color) const override;

	/* SETTERS */
	void setIndex(int index) { m_index = index; }
	void setBrokenPercent(int percent) { m_brokenPercent = percent; }
	void setOn(bool on) { m_active = on; }
	void setColor(NS2::ColorRGBA8& color) { m_color = color; }
	void setTexture(NS2::GLTexture* texture) { m_texture = texture; }

	/* GETTERS */
	int getIndex() const { return m_index; }
	int getBrokenPercent() const { return m_brokenPercent; }
	bool isActive() const { return m_active; }
	const NS2::ColorRGBA8& getColor() const { return m_color; }
	const NS2::GLTexture* getTexture() const { return m_texture; }
	const glm::vec4& getDestRect() const { return m_destRect; }

private:
	bool m_active = true;
	int m_index = -1;
	int m_brokenPercent = 0;
	NS2::ColorRGBA8 m_color = NS2::ColorRGBA8(255, 255, 255, 255);
	NS2::GLTexture* m_texture = nullptr;
	glm::vec4 m_destRect = glm::vec4(0.0f);

	static std::mt19937 m_rand;
	static std::uniform_int_distribution<int> m_randPercent;
	static const glm::vec4 m_uvRect;
};
