#include "GameRenderer.h"
#include "Level.h"
#include "Light.h"
#include "PhysicsEngine.h"
#include "EditorGui.h"
#include <Camera.h>

GameRenderer::GameRenderer(PhysicsEngine& physicsEngine) :
    m_physicEngine(physicsEngine),
    m_entityRenderer(m_spriteBatch, m_debugRenderer, physicsEngine),
    m_tileRenderer(m_spriteBatch, m_debugRenderer),
    m_lightRenderer(m_spriteBatch, m_debugRenderer)
{}

void GameRenderer::init(float screenWidth, float screenHeight, bool debug)
{
	m_lineWidth = 2.0f;
	m_debug = debug;

    m_spriteBatch.init((int)screenWidth, (int)screenHeight);
    m_debugRenderer.init();
}

void GameRenderer::dispose()
{
    m_spriteBatch.dispose();
    m_debugRenderer.dispose();
}

void GameRenderer::render(NS2::Camera& camera, Level& level)
{
	const glm::mat4 cameraMatrix = camera.getCameraMatrix();

    camera.setScreenBox(m_screenBox);

	m_spriteBatch.beginLightScene(level.getAmbientColor(), cameraMatrix);
    {
        m_lightRenderer.render(level, m_screenBox);
    }
	m_spriteBatch.endScene();

    m_spriteBatch.beginNormalScene(m_backColor, cameraMatrix);
    {
        m_tileRenderer.render(level, m_screenBox);
        m_entityRenderer.render(level, m_screenBox);
    }
    m_spriteBatch.endScene();

    m_spriteBatch.mergeScenes(cameraMatrix, m_screenBox);

    if (m_debug)
    {
        m_tileRenderer.renderDebug(level, m_screenBox);
        m_lightRenderer.renderDebug(level, m_screenBox);
        m_entityRenderer.renderDebug(level, m_screenBox);

        m_debugRenderer.render(cameraMatrix, m_lineWidth);
    }
}

void GameRenderer::render(NS2::Camera& camera, Level& level, EditorGui& editor)
{
    const glm::mat4 cameraMatrix = camera.getCameraMatrix();

    camera.setScreenBox(m_screenBox);

    m_spriteBatch.beginLightScene(level.getAmbientColor(), cameraMatrix);
    {
        editor.draw(m_lightRenderer, m_screenBox);
    }
    m_spriteBatch.endScene();

    m_spriteBatch.beginNormalScene(m_backColor, cameraMatrix);
    {
        editor.draw(m_tileRenderer, m_screenBox);
        editor.draw(m_entityRenderer, m_screenBox);
    }
    m_spriteBatch.endScene();

    m_spriteBatch.mergeScenes(cameraMatrix, m_screenBox);

    if (editor.debug(DebugType::DEBUG))
    {
        editor.drawDebug(m_tileRenderer, m_screenBox);
        editor.drawDebug(m_lightRenderer, m_screenBox);
        editor.drawDebug(m_entityRenderer, m_screenBox);

        m_debugRenderer.render(cameraMatrix, m_lineWidth);
    }
}
