#include "EditorHelper.h"

bool EditorHelper::drawColor(NS2::ColorRGBA8& color, const char* label)
{
	bool result = false;
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
		result = true;
	}
	return result;
}

bool EditorHelper::drawTexture(NS2::GLTexture*& texture, const char* label)
{
	bool result = false;
	std::string previewText = "-- Select Texture Image --";
	if (texture != nullptr && !texture->filePath.empty()) {
		previewText = texture->filePath;
	}
	if (ImGui::BeginCombo(label, previewText.c_str())) {
		for (const auto& e : m_parent.getAssets()) {
			bool isSelected = texture != nullptr &&
				texture->filePath == e.path;
			if (ImGui::Selectable(e.path.c_str(), isSelected) && !isSelected) {
				if (e.path.empty() || e.path == STR_NONE) {
					texture = nullptr;
				}
				else {
					texture = m_parent.getResourceManager().getTexture(e.path);
				}
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
