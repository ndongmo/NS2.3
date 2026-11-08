/*!
* \file LevelEditor.h
* \brief Level editor.
* \author Ndongmo Silatsa Fabrice
* \date 23-10-2026
* \version 2.0
*/

#pragma once

#include "AbstractEditor.h"
#include <glm/glm.hpp>

class Level;
class EditorGui;
class TileRenderer;

class LevelEditor : public AbstractEditor
{
public:
	LevelEditor(EditorGui& parent);

	virtual void create(const std::string& path, unsigned int type) override;
	virtual void update() override;
	virtual void onDraw() override;
	virtual std::string getName() const override;
	virtual bool load(const std::string& path) override;
	virtual bool save() override;
	virtual void draw(TileRenderer& tileRenderer, const glm::vec4& box) override;
	virtual void drawDebug(TileRenderer& tileRenderer, const glm::vec4& box) override;
	virtual void startAnim() override;
	virtual void stopAnim() override;
	virtual bool hasAnim() const override { return true; }

	void drawLeftSidebar();
	void drawRightSidebar();

private:
	Level& m_level;
};