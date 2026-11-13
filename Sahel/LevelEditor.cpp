#include "LevelEditor.h"
#include "EditorGui.h"
#include "PathHelper.h"
#include "ObjectManager.h"
#include "TileRenderer.h"
#include "Level.h"

LevelEditor::LevelEditor(EditorGui& parent) :
	AbstractEditor(parent, parent.getLevel()), m_level(parent.getLevel())
{}

bool LevelEditor::create(const std::string& path, unsigned int type)
{
	return false;
}

void LevelEditor::update()
{
	
}

void LevelEditor::onDraw()
{

}

std::string LevelEditor::getName() const
{
	return "Level Editor";
}

bool LevelEditor::load(const std::string& path)
{
	return m_parent.getObjectManager().loadDirect(path, m_level);
}

bool LevelEditor::save()
{
	return m_parent.getObjectManager().saveDirect(m_level.getPath(), m_level);
}

void LevelEditor::draw(TileRenderer& tileRenderer, const glm::vec4& box)
{
	tileRenderer.render(m_level, box);
}

void LevelEditor::drawDebug(TileRenderer& tileRenderer, const glm::vec4& box)
{
	tileRenderer.renderDebug(m_level, box);
}

void LevelEditor::startAnim()
{}

void LevelEditor::stopAnim()
{}

void LevelEditor::drawLeftSidebar()
{}

void LevelEditor::drawRightSidebar()
{}
