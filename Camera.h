/*!
* \file Camera2D.h
* \brief Simple 2D camera.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace NS2 {

	class Camera
	{
	public:
		/*!
		* \brief Initialize camera with width and height of screen.
		* \param screenWidth : width of screen
		* \param screenHeight : height of screen
		*/
		void init(float screenWidth, float screenHeight);
		/*!
		* \brief Update camera parameters.
		*/
		void update();
		/*!
		* \brief Convert screen coordinates to world coordinates.
		* \param screenCoords : screen coordinates
		* \return 2D vector coordinate of world
		*/
		glm::vec2 screenToWorld(glm::vec2 screenCoords) const;
		/*! 
		* \brief Test if a point is in the camera view.
		* \param point: point to test
		* \return true if point is in the camera view, otherwise return false
		*/
		bool isPointInView(const glm::vec2& point) const;
		/*!
		* \brief  Simple AABB test to see if a box is in the camera view.
		* \param x: box x position
		* \param y: box y position
		* \param w: box width
		* \param h: box height
		* \return true if box is in the camera view, otherwise return false
		*/
		bool isBoxInView(float x, float y, float w, float h) const;
		/*!
		* \brief Increment camera position with an offset value.
		* \param offset : offset 2D vector
		*/
		void offsetPosition(const glm::vec2& offset) { 
			m_position += offset; m_needsMatrixUpdate = true; 
		}
		/*!
		* \brief Increment camera scale with an offset value.
		* \param offset : offset 2D vector
		*/
		void offsetScale(float offset);

		/* SETTERS */
		void setScreenBox(glm::vec4& box);
		void setScale(float scale);
		void setPosition(glm::vec2 position);

		/* GETTERS */
		float getScreenLength() const { return m_length; }
		float getScale() const { return m_scale; }
		float getScreenWidth() const { return m_screenWidth; }
		float getScreenHeight() const { return m_screenHeight; }
		glm::vec2 getPosition() const { return m_position; }
		glm::mat4 getCameraMatrix() const { return m_cameraMatrix; }
		float getAspectRatio() const { return m_screenWidth / m_screenHeight; }

	private:
		float m_screenWidth = 100.0f;					/* Screen width */
		float m_screenHeight = 100.0f;					/* Screen height */
		float m_scale = 1.0f;							/* Camera scale */
		float m_length = 0.0f;
		bool m_needsMatrixUpdate = true;				/* Keep camera update state */
		glm::vec2 m_scaledDimension;					/* Keep camera scaled dimension */
		glm::vec2 m_position = glm::vec2(0.0f, 0.0f);	/* Camera position */
		glm::mat4 m_cameraMatrix = glm::mat4(1.0f);		/* Camera orthographic matrix */
		glm::mat4 m_orthoMatrix = glm::mat4(1.0f);		/* Orthographic matrix */
	};
}