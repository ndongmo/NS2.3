/*!
* \file EditorHelper.h
* \brief Implementation helper for editor classes.
* \author Ndongmo Silatsa Fabrice
* \date 01-11-2026
* \version 2.0
*/

#pragma once

#include "EditorGui.h"

#include <string>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <CoreTypes.h>

class EditorHelper
{
public:
	static bool drawColor(NS2::ColorRGBA8& color, const char* label = "Color");
	static bool drawTexture(NS2::GLTexture*& texture, const char* label = "Texture");

	template <typename T, typename Container>
	static bool drawEnum(T& currentValue, const Container& enumMap, const char* label) {
		bool result = false;
		std::string previewText = enumMap.at(currentValue);
		if (ImGui::BeginCombo(label, previewText.c_str())) {
			for (const auto& [typeKey, typeName] : enumMap) {
				T enumKey = static_cast<T>(typeKey);
				bool isSelected = (currentValue == enumKey);

				if (ImGui::Selectable(typeName.c_str(), isSelected) && !isSelected) {
					currentValue = enumKey;
					m_parent.setNeedToSave(true);
					result = true;
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		return result;
	}
};

