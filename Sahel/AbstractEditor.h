/*!
* \file ObjectEditor.h
* \brief Object manager mother class for GUI editor.
* \author Ndongmo Silatsa Fabrice
* \date 16-06-2018
* \version 2.0
*/

#pragma once

#include <string>
#include <glm/glm.hpp>

class IFileManager;
class EditorGui;
class TileRenderer;
class LightRenderer;
class EntityRenderer;

class AbstractEditor
{
public:
	AbstractEditor(EditorGui& parent, IFileManager& fileManager);
	virtual ~AbstractEditor();

	virtual void create(const std::string& path, unsigned int type) = 0;
	virtual void update() = 0;
	virtual void onDraw() = 0;
	virtual std::string getName() const = 0;
	virtual bool load(const std::string& path) = 0;
	virtual bool save() = 0;
	bool remove();
	bool rename(const std::string& newPath);
	bool copy(const std::string& newPath);

	virtual void draw(TileRenderer& tileRenderer, const glm::vec4& box);
	virtual void draw(LightRenderer& lightRenderer, const glm::vec4& box);
	virtual void draw(EntityRenderer& entityRenderer, const glm::vec4& box);

	virtual void drawDebug(TileRenderer& tileRenderer, const glm::vec4& box);
	virtual void drawDebug(LightRenderer& lightRenderer, const glm::vec4& box);
	virtual void drawDebug(EntityRenderer& entityRenderer, const glm::vec4& box);

	virtual void startAnim() {}
	virtual void stopAnim() {}
	virtual bool hasAnim() const { return false; }

	virtual void getInfo(std::string& name, glm::vec2& pos, glm::vec2& size, 
		glm::vec2& scale, float& angle) {}

	virtual void action(glm::vec2 point) {}
	virtual void select(glm::vec2 point) {}
	virtual void move(glm::vec2 delta) {}
	virtual void drag(glm::vec2 delta) {}
	virtual void dragOver(glm::vec2 point) {}

protected:
	IFileManager& m_fileManager;
	EditorGui& m_parent;
};

