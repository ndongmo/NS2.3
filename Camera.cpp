#include "Camera.h"
#include <cmath>

namespace NS2 {

	void Camera::init(float screenWidth, float screenHeight) {
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;
		m_orthoMatrix = glm::ortho(0.0f, m_screenWidth, 0.0f, m_screenHeight);
		m_scaledDimension = glm::vec2(m_screenWidth, m_screenHeight);
		m_length = std::hypot(m_screenWidth, m_screenHeight);
		m_needsMatrixUpdate = true;
	}

	void Camera::update() {
		if (m_needsMatrixUpdate) {
			// camera translation
			glm::vec3 translate(-m_position.x + m_screenWidth / 2.0f, -m_position.y + m_screenHeight / 2.0f, 0.0f);
			m_cameraMatrix = glm::translate(m_orthoMatrix, translate);

			// camera scale (Using 1.0f for Z-scale ensures depth planes are preserved)
			glm::vec3 scale(m_scale, m_scale, 1.0f);
			m_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_cameraMatrix;

			m_needsMatrixUpdate = false;
		}
	}

	glm::vec2 Camera::screenToWorld(glm::vec2 screenCoords) const {
		// invert y direction
		screenCoords.y = m_screenHeight - screenCoords.y;
		// Make it so that 0 is the center
		screenCoords -= glm::vec2(m_screenWidth / 2.0f, m_screenHeight / 2.0f);

		// Scale the coordinates
		screenCoords /= m_scale;

		// Translate with the camera position
		screenCoords += m_position;

		return screenCoords;
	}

	void Camera::setScreenBox(glm::vec4& box) {
		box.z = m_scaledDimension.x;
		box.w = m_scaledDimension.y;
		box.x = m_position.x - (box.z / 2.0f);
		box.y = m_position.y - (box.w / 2.0f);
	}

	bool Camera::isPointInView(const glm::vec2& point) const {
		const float x = m_position.x - (m_scaledDimension.x / 2.0f);
		const float y = m_position.y - (m_scaledDimension.y / 2.0f);

		return (point.x >= x && point.x <= x + m_scaledDimension.x &&
			point.y >= y && point.y <= y + m_scaledDimension.y);
	}

	bool Camera::isBoxInView(float x, float y, float w, float h) const {
		const float camX = m_position.x - (m_scaledDimension.x / 2.0f);
		const float camY = m_position.y - (m_scaledDimension.y / 2.0f);

		return (camX < x + w && camX + m_scaledDimension.x > x &&
			camY < y + h && camY + m_scaledDimension.y > y);
	}

	void Camera::offsetScale(float offset) {
		if (m_scale + offset >= 0.001f) {
			setScale(m_scale + offset);
		}
	}

	void Camera::setScale(float scale) {
		if (scale != m_scale) {
			m_scale = scale;
			m_needsMatrixUpdate = true;
			m_scaledDimension = glm::vec2(m_screenWidth, m_screenHeight) / m_scale;
			m_length = std::hypot(m_scaledDimension.x, m_scaledDimension.y);
		}
	}

	void Camera::setPosition(glm::vec2 position) {
		if (position.x != m_position.x || position.y != m_position.y) {
			m_position = position;
			m_needsMatrixUpdate = true;
		}
	}
}