/*!
* \file ObjectEditor.h
* \brief Object manager mother class for GUI editor.
* \author Ndongmo Silatsa Fabrice
* \date 16-06-2018
* \version 2.0
*/

#pragma once

#include "EditorGui.h"

#include <string>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <CoreTypes.h>

class IFileManager;
class TileRenderer;
class LightRenderer;
class EntityRenderer;

class AbstractEditor
{
public:
	AbstractEditor(EditorGui& parent, IFileManager& fileManager);
	virtual ~AbstractEditor();

	/*!
	* \brief Proceeds the creation of a new object.
	*/
	virtual void create(const std::string& path) = 0;
	/*!
	* \brief Updates the actual ui(s).
	*/
	virtual void update() = 0;
	/*!
	* \brief Draws the editor UI.
	*/
	virtual void onDraw() = 0;
	/*!
	* \brief Returns the name of the editor.
	*/
	virtual std::string getName() const = 0;
	/*!
	* \brief Loads an object given a file path.
	*/
	virtual bool load(const std::string& path) = 0;
	/*!
	* \brief Saves the current manipulated object.
	* \return true if everything is fine, otherwise false.
	*/
	virtual bool save() = 0;
	/*!
	* \brief Deletes the current object.
	*/
	bool remove();
	/*!
	* \brief Renames the current object. It will save the current object.
	*/
	bool rename(const std::string& newPath);
	/*!
	* \brief Proceeds the copy of a target object stored at the given path.
	*/
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
		float& angle, float& scale) {}

protected:
	void drawColor(NS2::ColorRGBA8& color, const char* label = "Color");
	void drawTexture(NS2::GLTexture*& texture, const char* label = "Texture");

	template <typename T, typename Container>
	void drawEnum(T& currentValue, const Container& enumMap, const char* label) {
		std::string previewText = enumMap.at(currentValue);
		if (ImGui::BeginCombo(label, previewText.c_str())) {
			for (const auto& [typeKey, typeName] : enumMap) {
				T enumKey = static_cast<T>(typeKey);
				bool isSelected = (currentValue == enumKey);

				if (ImGui::Selectable(typeName.c_str(), isSelected)) {
					currentValue = enumKey;
					m_parent.setNeedToSave(true);
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}

	IFileManager& m_fileManager;
	EditorGui& m_parent;
};

