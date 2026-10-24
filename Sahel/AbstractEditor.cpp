#include "AbstractEditor.h"
#include "IFileManager.h"
#include "EditorGui.h"
#include "ObjectManager.h"
#include "TileRenderer.h"
#include "LightRenderer.h"
#include "EntityRenderer.h"

AbstractEditor::AbstractEditor(EditorGui& parent, IFileManager& fileManager) : 
	m_parent(parent), m_fileManager(fileManager)
{}

AbstractEditor::~AbstractEditor()
{}

bool AbstractEditor::remove()
{
	return m_parent.getObjectManager().remove(m_fileManager.getPath());
}

bool AbstractEditor::rename(const std::string& newPath)
{
	if (m_parent.getObjectManager().rename(m_fileManager.getPath(), newPath)) {
		m_fileManager.setPath(newPath);
		return true;
	}
	return false;
}

bool AbstractEditor::copy(const std::string& newPath)
{
	m_fileManager.setPath(newPath);
	return save();
}

void AbstractEditor::draw(TileRenderer& tileRenderer, const glm::vec4& box) {}
void AbstractEditor::draw(LightRenderer & lightRenderer, const glm::vec4& box) {}
void AbstractEditor::draw(EntityRenderer & entityRenderer, const glm::vec4& box) {}

void AbstractEditor::drawDebug(TileRenderer& tileRenderer, const glm::vec4& box) {}
void AbstractEditor::drawDebug(LightRenderer& lightRenderer, const glm::vec4& box) {}
void AbstractEditor::drawDebug(EntityRenderer& entityRenderer, const glm::vec4& box) {}
