/*!
* \file LightEditor.h
* \brief Light editor.
* \author Ndongmo Silatsa Fabrice
* \date 16-04-2020
* \version 2.0
*/

#pragma once

#include "AbstractEditor.h"
#include "Light.h"
#include "Tile.h"

#include <glm/glm.hpp>

class EditorGui;
class LightRenderer;
class TileRenderer;

class LightEditor : public AbstractEditor
{
public:
	LightEditor(EditorGui& parent);

	virtual void create(const std::string& path, unsigned int type) override;
	virtual void update() override;
	virtual void onDraw() override;
	virtual std::string getName() const override;
	virtual bool load(const std::string& path) override;
	virtual bool save() override;
	virtual void draw(LightRenderer& lightRenderer, const glm::vec4& box) override;
	virtual void draw(TileRenderer& tileRenderer, const glm::vec4& box) override;
	virtual void drawDebug(LightRenderer& lightRenderer, const glm::vec4& box) override;
	virtual void getInfo(std::string& name, glm::vec2& pos, glm::vec2& size, glm::vec2& scale, float& angle) override;

private:
	Light m_light;
	Tile m_tile;
};