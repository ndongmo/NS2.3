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

	m_textureShader.compileShaders("Sprite.vert", "Sprite.frag");
    m_textureShader.addAttribute("vertexData");
    m_textureShader.addAttribute("destRect");
    m_textureShader.addAttribute("uvRect");
    m_textureShader.addAttribute("vertexColor");
	m_textureShader.linkShaders();

    m_lightShader.compileShaders("Light.vert", "Light.frag");
    m_lightShader.addAttribute("vertexData");
    m_lightShader.addAttribute("destRect");
    m_lightShader.addAttribute("uvRect");
    m_lightShader.addAttribute("vertexColor");
    m_lightShader.linkShaders();

    m_spriteBatch.init();
    m_debugRenderer.init();
}

void GameRenderer::dispose()
{
    m_debugRenderer.dispose();
	m_textureShader.dispose();
}

void GameRenderer::clearAmbientColor(const glm::vec3& color)
{
	glClearColor(color.r, color.g, color.b, 1.0f);
}

void GameRenderer::render(NS2::Camera& camera, Level& level)
{
    camera.setScreenBox(m_screenBox);

    clearAmbientColor(level.getAmbientColor());
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ZERO, GL_ONE);

    m_lightShader.use();
    {
        m_lightShader.setUniform("u_LightMaskTexture", 0);
        m_lightShader.setUniform("cameraMatrix", camera.getCameraMatrix());

        m_spriteBatch.begin();
        {
            m_lightRenderer.render(level, m_screenBox);
        }
        m_spriteBatch.end();
        m_spriteBatch.render();
    }
    m_lightShader.unuse();

    glBlendFunc(GL_DST_COLOR, GL_ZERO);

    m_textureShader.use();
    {
        m_textureShader.setUniform("ns2TextureSampler", 0);
        m_textureShader.setUniform("cameraMatrix", camera.getCameraMatrix());

        m_spriteBatch.begin();
        {
            m_tileRenderer.render(level, m_screenBox);
            m_entityRenderer.render(level, m_screenBox);
        }
        m_spriteBatch.end();
        m_spriteBatch.render();
    }
    m_textureShader.unuse();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (m_debug)
    {
        m_entityRenderer.renderDebug(level, m_screenBox);
        m_lightRenderer.renderDebug(level, m_screenBox);
        m_tileRenderer.renderDebug(level, m_screenBox);

        m_debugRenderer.end();
        m_debugRenderer.render(camera.getCameraMatrix(), m_lineWidth);
    }
}

void GameRenderer::render(NS2::Camera& camera, Level& level, EditorGui& editor)
{
    camera.setScreenBox(m_screenBox);

    clearAmbientColor(level.getAmbientColor());
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ZERO, GL_ONE);

    m_lightShader.use();
    {
        m_lightShader.setUniform("u_LightMaskTexture", 0);
        m_lightShader.setUniform("cameraMatrix", camera.getCameraMatrix());

        m_spriteBatch.begin();
        {
            editor.draw(m_lightRenderer, m_screenBox);
        }
        m_spriteBatch.end();
        m_spriteBatch.render();
    }
    m_lightShader.unuse();

    glBlendFunc(GL_DST_COLOR, GL_ZERO);

    m_textureShader.use();
    {
        m_textureShader.setUniform("ns2TextureSampler", 0);
        m_textureShader.setUniform("cameraMatrix", camera.getCameraMatrix());

        m_spriteBatch.begin();
        {
            editor.draw(m_tileRenderer, m_screenBox);
            editor.draw(m_entityRenderer, m_screenBox);
        }
        m_spriteBatch.end();
        m_spriteBatch.render();
    }
    m_textureShader.unuse();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (editor.debug(DebugType::DEBUG))
    {
        editor.drawDebug(m_entityRenderer, m_screenBox);
        editor.drawDebug(m_lightRenderer, m_screenBox);
        editor.drawDebug(m_tileRenderer, m_screenBox);

        m_debugRenderer.end();
        m_debugRenderer.render(camera.getCameraMatrix(), m_lineWidth);
    }
}
