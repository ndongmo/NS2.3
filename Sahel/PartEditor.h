/*!
* \file PartEditor.h
* \brief Body Part editor.
* \author Ndongmo Silatsa Fabrice
* \date 16-04-2020
* \version 2.0
*/

#pragma once

#include "AbstractEditor.h"
#include "Part.h"
#include <glm/glm.hpp>

class EditorGui;
class TileRenderer;
class EntityRenderer;

class PartEditor : public AbstractEditor
{
public:
	PartEditor(EditorGui& parent);

	virtual void create(const std::string& path, unsigned int type) override;
	virtual void update() override;
	virtual void onDraw() override;
	virtual std::string getName() const override;
	virtual bool load(const std::string& path) override;
	virtual bool save() override;
	virtual void draw(EntityRenderer& renderer, const glm::vec4& box) override;
	virtual void drawDebug(TileRenderer& renderer, const glm::vec4& box) override;
	virtual void drawDebug(EntityRenderer& renderer, const glm::vec4& box) override;
	virtual void getInfo(std::string& name, glm::vec2& pos, glm::vec2& size, glm::vec2& scale, float& angle) override;
	virtual void select(glm::vec2 point) override;
	virtual void drag(glm::vec2 delta) override;
	virtual void dragOver(glm::vec2 point) override;

private:
	int m_selected = -1; // selected points on collision shape
	Part m_part;
};