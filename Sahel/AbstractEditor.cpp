#include "AbstractEditor.h"
#include "IFileManager.h"
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

void AbstractEditor::drawColor(NS2::ColorRGBA8& color, const char* label)
{
	float temp_rgba[4] = {
		color.r / 255.0f,
		color.g / 255.0f,
		color.b / 255.0f,
		color.a / 255.0f
	};
	if (ImGui::ColorEdit4(label, temp_rgba)) {
		color.r = static_cast<GLubyte>(temp_rgba[0] * 255.0f + 0.5f);
		color.g = static_cast<GLubyte>(temp_rgba[1] * 255.0f + 0.5f);
		color.b = static_cast<GLubyte>(temp_rgba[2] * 255.0f + 0.5f);
		color.a = static_cast<GLubyte>(temp_rgba[3] * 255.0f + 0.5f);
		m_parent.setNeedToSave(true);
	}
}

void AbstractEditor::drawTexture(NS2::GLTexture*& texture, const char* label)
{
	std::string previewText = "-- Select Texture Image --";
	if (texture != nullptr && !texture->filePath.empty()) {
		previewText = texture->filePath;
	}
	if (ImGui::BeginCombo(label, previewText.c_str())) {
		for (const auto& e : m_parent.getAssets()) {
			bool isSelected = texture != nullptr &&
				texture->filePath == e.path;
			if (ImGui::Selectable(e.path.c_str(), isSelected)) {
				texture = m_parent.getResourceManager().getTexture(e.path);
				m_parent.setNeedToSave(true);
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}
