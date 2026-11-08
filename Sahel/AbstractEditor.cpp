#include "AbstractEditor.h"
#include "IFileManager.h"
#include "ObjectManager.h"
#include "TileRenderer.h"
#include "LightRenderer.h"
#include "EntityRenderer.h"
#include "EditorGui.h"

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
	const std::string oldPath = m_fileManager.getPath();
	m_fileManager.setPath(newPath);
	bool saved = save();
	if (saved) {
		m_parent.getObjectManager().remove(oldPath);
	}
	else {
		m_fileManager.setPath(oldPath);
	}
	return saved;
}

bool AbstractEditor::copy(const std::string& newPath)
{
	const std::string oldPath = m_fileManager.getPath();
	m_fileManager.setPath(newPath);
	bool saved = save();
	m_fileManager.setPath(oldPath);
	return saved;
}

void AbstractEditor::draw(TileRenderer& tileRenderer, const glm::vec4& box) {}
void AbstractEditor::draw(LightRenderer & lightRenderer, const glm::vec4& box) {}
void AbstractEditor::draw(EntityRenderer & entityRenderer, const glm::vec4& box) {}

void AbstractEditor::drawDebug(TileRenderer& tileRenderer, const glm::vec4& box) {}
void AbstractEditor::drawDebug(LightRenderer& lightRenderer, const glm::vec4& box) {}
void AbstractEditor::drawDebug(EntityRenderer& entityRenderer, const glm::vec4& box) {}
